from cs50 import get_float

# Get a valid amount of change owed
while True:
    amt = get_float("Change owed: ")
    if amt >= 0:
        break

# Getting an integer equivalent of the change
amt = int(amt * 100)

# Values of valid coins
quarter_val = 25
dime_val = 10
nickel_val = 5
penny_val = 1

# Calculating the amount of each coin needed
# (trying to get the minimum amount of coins)
num_quarters = amt // quarter_val
amt %= quarter_val

num_dimes = amt // dime_val
amt %= dime_val

num_nickels = amt // nickel_val
amt %= nickel_val

num_pennies = amt // penny_val
amt %= penny_val

min_num_of_coins = num_quarters + num_dimes + num_nickels + num_pennies
print(f"{min_num_of_coins}")