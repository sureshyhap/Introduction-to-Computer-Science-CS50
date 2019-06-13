import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    # First checks for missing fields
    if not request.form.get("name"):
        return render_template("error.html", message="No name submitted!")
    elif not request.form.get("house"):
        return render_template("error.html", message="No house submitted!")
    elif not request.form.get("player_position"):
        return render_template("error.html", message="No position submitted!")
    # If all the fields are given, write them to the csv file
    file = open("survey.csv", "a")
    writer = csv.writer(file)
    writer.writerow((request.form.get("name"), request.form.get("house"), request.form.get("player_position")))
    file.close()
    return redirect("/sheet")


@app.route("/sheet", methods=["GET"])
def get_sheet():
    # Read through csv file and pass them to sheet.html
    file = open("survey.csv", "r")
    reader = csv.reader(file)
    players = list(reader)
    file.close()
    return render_template("sheet.html", players=players)
