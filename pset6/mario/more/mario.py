from cs50 import get_int

# Get a height from the user
while True:
    h = get_int("Height: ")
    if h >= 1 and h <= 8:
        break

# Number of lines
for i in range(h):
    # Number of spaces padded to the front
    for j in range(h - i - 1):
        print(" ", end="")
    # Number of hashes at the left
    for j in range(i + 1):
        print("#", end="")
    # Two spaces in the middle
    print("  ", end="")
    # Number of hashes at the right
    for j in range(i + 1):
        print("#", end="")
    print("")
