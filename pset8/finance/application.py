import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    id = session["user_id"]
    # The entire transaction table corresponding to the user
    all_transactions = db.execute("SELECT * FROM :id", id=str(id))
    sum = 0
    totals = []
    # Get the total value of all shares of all stocks owned
    for transaction in all_transactions:
        sum += transaction["total"]
        totals.append(usd(transaction["total"]))
        transaction["price"] = usd(transaction["price"])
        transaction["total"] = usd(transaction["total"])
    # The user's remaining money
    money = db.execute("SELECT cash FROM users WHERE id = :id", id=id)
    money = money[0]["cash"]
    # all should be 10,000
    all = money + sum
    money = usd(money)
    all = usd(all)
    return render_template("index.html", all_transactions=all_transactions, money=money, all=all)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("No stock chosen!")
        quote = lookup(request.form.get("symbol"))
        if not quote:
            return apology("Incorrect symbol!")

        # Valid company
        if not request.form.get("shares"):
            return apology("No number of shares entered!")
        elif not request.form.get("shares").isnumeric():
            if len(request.form.get("shares")) > 1:
                # Negative number
                if request.form.get("shares")[0] == '-' and request.form.get("shares")[1:].isnumeric():
                    return apology("Please enter a positive number!")
                else:
                    # Non-numeric string
                    return apology("Please enter a positive integer!")
                    # Single character that is not a number was entered
            return apology("Please enter a number!")
        elif int(request.form.get("shares")) == 0:
            return apology("Please enter a positive number!")

        # Valid company and share number
        # Present total user's cash
        total_user_cash = db.execute("SELECT cash FROM users WHERE id = :id",
                                     id=session["user_id"])
        total_user_cash = total_user_cash[0]["cash"]
        # Total cost of the purchase
        cost = quote["price"] * int(request.form.get("shares"))
        # Remaining cash
        remaining = total_user_cash - cost
        if remaining < 0:
            return apology("Insufficient funds!")
        # Inserting transaction into user's table
        id = session["user_id"]
        in_db = db.execute("SELECT * FROM :id WHERE symbol = :symbol", id=str(id), symbol=quote["symbol"])
        # If symbol was not found in the database, make a new insert
        if len(in_db) == 0:
            db.execute("INSERT INTO :id (symbol, name, shares, price, total) VALUES (:symbol, :name, :shares, :price, :total)",
                       id=str(id), symbol=quote["symbol"], name=quote["name"], shares=int(request.form.get("shares")), price=quote["price"],
                       total=cost)
        # Else if symbol was found in the database, make an update
        else:
            old_values = db.execute("SELECT shares, price FROM :id WHERE symbol = :symbol", id=str(id), symbol=quote["symbol"])
            # New amount of shares is the old value + the new amount
            new_shares = old_values[0]["shares"] + int(request.form.get("shares"))
            new_total = new_shares * old_values[0]["price"]
            # Update shares and total
            db.execute("UPDATE :id SET shares = :new_shares WHERE symbol = :symbol",
                       id=str(id), new_shares=new_shares, symbol=quote["symbol"])
            db.execute("UPDATE :id SET total = :new_total WHERE symbol = :symbol",
                       id=str(id), new_total=new_total, symbol=quote["symbol"])
        # Update user's table with new cash total for user
        db.execute("UPDATE users SET cash = :remaining WHERE id = :id", remaining=remaining, id=id)
        hist = str(id) + " history"
        # Insert into history
        db.execute("INSERT INTO :hist (buy_or_sell, symbol, price, shares) VALUES ('buy', :symbol, :price, :shares)",
                   hist=hist, symbol=quote['symbol'], price=quote['price'], shares=int(request.form.get('shares')))
        return redirect("/")
    elif request.method == "GET":
        return render_template("buy2.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    input_name = request.args.get("username")
    if len(input_name) < 1:
        return jsonify("Username not available")
    query = db.execute("SELECT username FROM users WHERE username = :input_name", input_name=input_name)
    if len(query) == 0:
        return jsonify(True)
    else:
        return jsonify(False)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    id = session["user_id"]
    hist = str(id) + " history"
    full_hist = db.execute("SELECT * from :hist", hist=hist)
    for hist in full_hist:
        hist["price"] = usd(hist["price"])
    return render_template("history.html", full_hist=full_hist)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("No symbol entered!")
        quote = lookup(request.form.get("symbol"))
        # No company found
        if not quote:
            return apology("No matching company!")

        # Company found
        quote["price"] = usd(quote["price"])
        return render_template("quoted.html", quote=quote)
    elif request.method == "GET":
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # If no username entered
        if not request.form.get("username"):
            return apology("Missing username!")
        # Checks if username is already used
        user = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))
        # If username is already found in the database
        if len(user) != 0:
            return apology("Username already taken!")

        # At this point, username is valid
        if not request.form.get("password") or not request.form.get("confirmation"):
            return apology("Missing password!")
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords do not match!")

        # At this point, username and password are valid so insert into table
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :password)",
                   username=request.form.get("username"),
                   password=generate_password_hash(request.form.get("password")))
        # Creating a transaction table for the user
        id = db.execute("SELECT id FROM users WHERE username = :username",
                        username=request.form.get("username"))
        id = id[0]["id"]
        db.execute("CREATE TABLE :id ('symbol' char(10) PRIMARY KEY NOT NULL, 'name' varchar(100) NOT NULL UNIQUE, \
        'shares' integer NOT NULL, 'price' real NOT NULL, 'total' real NOT NULL)", id=str(id))
        # Create a history table
        history_table_name = str(id) + " history"
        db.execute("CREATE TABLE :history_table_name ('buy_or_sell' char(4) NOT NULL, 'symbol' char(10) NOT NULL, \
        'price' real NOT NULL, 'shares' integer NOT NULL, 'when' datetime DEFAULT CURRENT_TIMESTAMP NOT NULL)", history_table_name=history_table_name)
        return redirect("/")
    elif request.method == "GET":
        return render_template("register.html")


@app.route("/reset", methods=["GET", "POST"])
@login_required
def reset():
    if request.method == "GET":
        return render_template("reset_pass.html")
    elif request.method == "POST":
        if not request.form.get("password") or not request.form.get("confirmation"):
            return apology("Missing password!")
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords do not match!")

        # Valid password
        id = session["user_id"]
        db.execute("UPDATE users SET hash = :password WHERE id = :id",
                   password=generate_password_hash(request.form.get("password")), id=id)
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    id = session["user_id"]
    if request.method == "GET":
        stocks = db.execute("SELECT symbol FROM :id", id=str(id))
        return render_template("sell.html", stocks=stocks)
    elif request.method == "POST":
        if not request.form.get("symbol"):
            return apology("No stock chosen!")
        elif not request.form.get("shares").isnumeric():
            if len(request.form.get("shares")) > 1:
                # Negative number
                if request.form.get("shares")[0] == '-' and request.form.get("shares")[1:].isnumeric():
                    return apology("Please enter a positive number!")
                else:
                    # Non-numeric string
                    return apology("Please enter a positive integer!")
                    # Single character that is not a number was entered
            return apology("Please enter a number!")
        shares = db.execute("SELECT shares FROM :id WHERE symbol = :symbol", id=str(id), symbol=request.form.get("symbol"))
        shares = shares[0]["shares"]
        if shares < int(request.form.get("shares")):
            return apology("You don't have that many shares!")

        # If reached here, it is valid
        # Get price of stock
        price = db.execute("SELECT price FROM :id WHERE symbol = :symbol", id=str(id), symbol=request.form.get("symbol"))
        price = price[0]["price"]
        # Amount the user's cash will be raised by
        refund = price * int(request.form.get("shares"))
        # Get user's present cash amount
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=id)
        cash = cash[0]["cash"]
        # Calculate new total amount of cash after the sale
        new_total = refund + cash
        # Update user's cash to new total
        db.execute("UPDATE users SET cash = :new_total WHERE id = :id", new_total=new_total, id=id)
        # Selling all shares
        if shares == int(request.form.get("shares")):
            # Delete company from transaction database
            db.execute("DELETE FROM :id WHERE symbol = :symbol", id=str(id), symbol=request.form.get("symbol"))
        # Not selling all shares
        elif shares > int(request.form.get("shares")):
            new_amt_shares = shares - int(request.form.get("shares"))
            # Update the amount of shares
            db.execute("UPDATE :id SET shares = :new_amt_shares WHERE symbol = :symbol", id=str(id),
                       new_amt_shares=new_amt_shares, symbol=request.form.get("symbol"))
            new_total_stock = new_amt_shares * price
            # Update total value of your investment in the company
            db.execute("UPDATE :id SET total = :new_total_stock WHERE symbol = :symbol",
                       id=str(id), new_total_stock=new_total_stock, symbol=request.form.get("symbol"))
        hist = str(id) + " history"
        # Insert into history
        db.execute("INSERT INTO :hist (buy_or_sell, symbol, price, shares) VALUES ('sell', :symbol, :price, :shares)",
                   hist=hist, symbol=request.form.get("symbol"), price=price, shares=int(request.form.get("shares")))
        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)