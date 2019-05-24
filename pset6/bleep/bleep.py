from cs50 import get_string
from sys import argv


def main():
    # Checking for exactly 1 command line argument
    if len(argv) != 2:
        exit("Usage: python bleep.py dictionary")
    # Set of banned words
    banned = set()
    bw_file = open(argv[1], "r")
    for line in bw_file:
        # Populate the list of banned words from the file
        banned.add(line.rstrip("\n"))
    bw_file.close()
    s = get_string("What message would you like to censor")
    for token in s.split():
        is_banned = False
        # Check for matches in set of banned words
        for b_word in banned:
            if b_word == token.lower():
                is_banned = True
                break
        if is_banned == True:
            # If banned replace with stars
            for character in token:
                print("*", end="")
            print(" ", end="")
        else:
            # If not banned just print the token
            print(token + " ", end="")
    print("")


if __name__ == "__main__":
    main()
