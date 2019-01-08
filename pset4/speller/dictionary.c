// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Recursively deletes the trie
bool unload_recursive(node *temp);

// Represents a trie
node *root;
// Kepps track of the size of the dictionary
unsigned int size_dict = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // TODO
        node *temp = root;
        for (int i = 0; word[i] != '\0'; ++i)
        {
            // index becomes 1 to 26 for 'a' to 'z' and 0 for apostrophes
            int index = (word[i] == '\'' ? 0 : (word[i] - 'a' + 1));
            // If the path for this character HAS NOT been made already
            if (temp->children[index] == NULL)
            {
                // Create a new node for the new character
                temp->children[index] = malloc(sizeof(node));
                // If malloc could not find memory
                if (temp->children[index] == NULL)
                {
                    return false;
                }
                // temp now points to the new node
                temp = temp->children[index];
                // Initializes the new node to be "empty"
                temp->is_word = false;
                for (int j = 0; j < N; ++j)
                {
                    temp->children[j] = NULL;
                }
            }
            // If the path for this character HAS been made already
            // just follow the path
            else
            {
                temp = temp->children[index];
            }
        }
        // The end of the path
        temp->is_word = true;
        ++size_dict;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return size_dict;
    //return 0;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    int len = strlen(word);
    // + 1 because of '\0'
    char *lower_word = malloc(len + 1);
    if (lower_word == NULL)
    {
        return false;
    }
    for (int i = 0; word[i] != '\0'; ++i)
    {
        // lower_word will be word just lower cased
        lower_word[i] = tolower(word[i]);
    }
    lower_word[len] = '\0';
    node *temp = root;
    for (int i = 0; lower_word[i] != '\0'; ++i)
    {
        // index same as in load()
        int index = (lower_word[i] == '\'' ? 0 : (lower_word[i] - 'a' + 1));
        // If there is not path the word cannot be in the dictionary
        if (temp->children[index] == NULL)
        {
            free(lower_word);
            return false;
        }
        // If not, follow the path
        temp = temp->children[index];
    }
    free(lower_word);
    // Can only reach here if you finished the path.
    // The final check is if is_word is true
    if (temp->is_word == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    bool result = unload_recursive(root);
    size_dict = 0;
    return result;
    //return false;
}

bool unload_recursive(node *temp)
{
    for (int i = 0; i < N; ++i)
    {
        // If there is a path through a child
        if (temp->children[i] != NULL)
        {
            // Recursively free the sub-trie
            unload_recursive(temp->children[i]);
        }
    }
    // If this point is reached all the node's pointers are NULL
    // so safe to free
    free(temp);
    return true;
}