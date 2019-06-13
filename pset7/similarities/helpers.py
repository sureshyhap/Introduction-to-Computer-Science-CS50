import nltk
from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""
    results = set()
    # Extracts single lines
    for line_a in a.splitlines():
        for line_b in b.splitlines():
            if line_a == line_b:
                results.add(line_a)
    return list(results)


def sentences(a, b):
    """Return sentences in both a and b"""
    results = set()
    # Extracts single sentences
    sent_a = nltk.tokenize.sent_tokenize(a)
    sent_b = nltk.tokenize.sent_tokenize(b)
    for sent1 in sent_a:
        for sent2 in sent_b:
            if sent1 == sent2:
                results.add(sent1)
    return list(results)


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    # Extracts individual words
    word_list_a = a.split()
    word_list_b = b.split()
    # These are the list of substrings
    a_substrings = list()
    b_substrings = list()
    # Find all possible substrings of a string
    for word_a in word_list_a:
        a_substrings[len(a):] = substr(word_a, n)
    # Find all possible substrings of b string
    for word_b in word_list_b:
        b_substrings[len(b):] = substr(word_b, n)
    a_substrings = set(a_substrings)
    b_substrings = set(b_substrings)
    results = set()
    # Compares the sets for commonalities
    for sub_a in a_substrings:
        for sub_b in b_substrings:
            if sub_a == sub_b:
                results.add(sub_a)
    return list(results)


def substr(a, n):
    results = set()
    length = len(a)
    # If n corresponds to whole string, return it
    if n == length:
        results.add(a)
        return list(results)
        # If n is too large, return nothing
    elif n > length:
        return list()
    # Making a list of all aubstrings of a with length n
    for i in range(length - n + 1):
        results.add(a[i:i+n])
    return list(results)
