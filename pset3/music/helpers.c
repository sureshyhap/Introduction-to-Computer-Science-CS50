// Helper functions for music

#include <cs50.h>
#include "helpers.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

const int OCTAVE_LENGTH = 12;
const int A4_OCT = 4;

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    // Numerator is first character
    char num[2] = {fraction[0], '\0'};
    // Denominator is third character (after '/')
    char den[2] = {fraction[2], '\0'};
    int numerator = atoi(num);
    int denominator = atoi(den);

    switch (denominator)
    {
        // The breaks aren't necessary because of the returns
        case 8:
            return numerator;
        case 4:
            return numerator * 2;
        case 2:
            return numerator * 4;
        case 1:
            return numerator * 8;
    }
    return -1;
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    int n = strlen(note);
    // n - 1 characters for the key and a null ('\0') character as well
    char key[n];
    key[0] = note[0];
    key[n - 1] = '\0';
    bool sharp_or_flat = false;
    int octave;

    // The key has a sharp or a flat
    if (n == 3)
    {
        key[1] = note[1];
        char note_oct_2[2] = {note[2], '\0'};
        octave = atoi(note_oct_2);
        sharp_or_flat = true;
    }
    // The key has no sharps or flats
    else
    {
        char note_oct[2] = {note[1], '\0'};
        octave = atoi(note_oct);
    }

    bool sharp = false, flat = false;

    if (sharp_or_flat)
    {
        if (key[1] == '#')
        {
            sharp = true;
        }
        else if (key[1] == 'b')
        {
            flat = true;
        }
    }

    // Corrects for edge cases of octaves
    if (sharp == true && key[0] == 'B')
    {
        ++octave;
        ++(key[0]);
        sharp = false;
        sharp_or_flat = false;
    }

    if (flat == true && key[0] == 'C')
    {
        --octave;
        --(key[0]);
        flat = false;
        sharp_or_flat = false;
    }

    int distance_from_oct_4 = octave - A4_OCT;

    // Will find distance from A in an octave soon
    int distance_from_A = 0;

    // A or B
    if (key[0] < 'C')
    {
        if (sharp_or_flat == false)
        {
            // A or B
            distance_from_A = (key[0] - 'A') * 2;
        }
        else if (sharp)
        {
            // Must be A#
            distance_from_A = 1;
        }
        else if (flat)
        {
            // Can be Ab or Bb
            if (key[0] == 'A')
            {
                distance_from_A = -1;
            }
            else if (key[0] == 'B')
            {
                distance_from_A = 1;
            }
        }
    }
    // C, D, or E
    else if (key[0] < 'F')
    {
        // Extra - 1 is because there is no black key between a B and a C.
        // OCTAVE_LENGTH is a correction so that the keys considered are in the same octave
        if (sharp_or_flat == false)
        {
            distance_from_A = ((key[0] - 'A') * 2) - 1 - OCTAVE_LENGTH;
        }
        else if (flat)
        {
            // One key closer than the white keys in this case (before subtracting OCTAVE_LENGTH)
            distance_from_A = ((key[0] - 'A') * 2) - 1 - 1 - OCTAVE_LENGTH;
        }
        else if (sharp)
        {
            // One key farther than the white keys in this case (before subtracting OCTAVE_LENGTH)
            distance_from_A = ((key[0] - 'A') * 2) - 1 + 1 - OCTAVE_LENGTH;
        }
    }
    // F or G
    else
    {
        if (sharp_or_flat == false)
        {
            // Second - 1 is because there is not black key between F and G
            distance_from_A = ((key[0] - 'A') * 2) - 1 - 1 - OCTAVE_LENGTH;
        }
        else if (sharp)
        {
            // Added 1 because sharps are onke key farther away (before subtracting OCTAVE_LENGTH)
            distance_from_A = ((key[0] - 'A') * 2) - 1 - 1 + 1 - OCTAVE_LENGTH;
        }
        else if (flat)
        {
            // Subtracted 1 at the end because sharps are onke key closer (before subtracting OCTAVE_LENGTH)
            distance_from_A = ((key[0] - 'A') * 2) - 1 - 1 - 1 - OCTAVE_LENGTH;
        }
    }

    int distance_from_A440 = ((distance_from_oct_4) * OCTAVE_LENGTH) + distance_from_A;

    int freq = round(pow(2, distance_from_A440 / 12.0) * 440);
    return freq;

}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    if (!strcmp(s, ""))
    {
        return true;
    }
    else
    {
        return false;
    }
}
