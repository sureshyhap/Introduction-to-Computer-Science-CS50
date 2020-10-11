from sys import argv
from cs50 import get_string

# Acquire correct command line arguments
if len(argv) != 2 or not argv[1].isalpha():
    exit("Usage: python vigenere.py k")

# Relevant strings being considered
k = argv[1]
p = get_string("plaintext: ")
out = ""

i = 0
n = len(p)

while i < n:
    for j in k:
        # Calculate the letter shift based on the letter in k
        if j.islower():
            change = ord(j) - ord('a')
        elif j.isupper():
            change = ord(j) - ord('A')
        # Only apply shift to alphabetical characters
        while not p[i].isalpha():
            out += p[i]
            i += 1
            # Position in p has gone past the end so exit
            if i >= n:
                print("ciphertext: " + out)
                exit()
        # Getting raw value of new character by subtracting off the value of 'a' or 'A'
        # Then add the change and mod 26 so that the value will correspond to a letter
        # Then add back the value of 'a' or 'A' to get the final result
        if p[i].islower():
            out += chr((((ord(p[i]) - ord('a')) + change) % 26) + ord('a'))
        elif p[i].isupper():
            out += chr((((ord(p[i]) - ord('A')) + change) % 26) + ord('A'))
        i += 1
        if i >= n:
            print("ciphertext: " + out)
            exit()

print("ciphertext: " + out)