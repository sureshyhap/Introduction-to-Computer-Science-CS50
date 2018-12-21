#include <stdio.h>
#include <cs50.h>
#include <string.h>

#define ASCII_VAL_0 48
#define LEN_ALPHABET 26

int pow_10(int power);

int main(int argc, string argv[])
{
  if (argc == 1 || argc > 2)
    {
      printf("Error: Incorrect number of command line arguments.\n");
      return 1;
    }

  string k = argv[argc - 1];
  string original = get_string("plaintext:");
  int sum = 0;

  // Does not consider the possibility that the command line argument is not
  // a non-negative integer
  for (int len = strlen(k), i = len - 1, present_num = 0, j = 0; i >= 0; --i, ++j)
    {
      int ascii_val = (int) k[i];

      // Translates digits in the string to numerical digits
      switch (ascii_val)
        {
	case ASCII_VAL_0:
	  present_num = 0;
	  break;
	case ASCII_VAL_0 + 1:
	  present_num = 1;
	  break;
	case ASCII_VAL_0 + 2:
	  present_num = 2;
	  break;
	case ASCII_VAL_0 + 3:
	  present_num = 3;
	  break;
	case ASCII_VAL_0 + 4:
	  present_num = 4;
	  break;
	case ASCII_VAL_0 + 5:
	  present_num = 5;
	  break;
	case ASCII_VAL_0 + 6:
	  present_num = 6;
	  break;
	case ASCII_VAL_0 + 7:
	  present_num = 7;
	  break;
	case ASCII_VAL_0 + 8:
	  present_num = 8;
	  break;
	case ASCII_VAL_0 + 9:
	  present_num = 9;
	  break;
        }
      sum += (present_num * pow_10(j));
    }

  int key = sum % LEN_ALPHABET;
  int len = strlen(original);
  char changed[len];

  // The actual changing of the word using the key
  for (int i = 0; i < len; ++i)
    {
      char present_char = original[i];

      if ((present_char < 'A') || (present_char > 'Z' && present_char < 'a') || (present_char > 'z'))
        {
	  changed[i] = present_char;
	  continue;
        }
      if (present_char >= 'A' && present_char <= 'Z')
        {
	  changed[i] = 'A' + ((present_char - 'A' + key) % LEN_ALPHABET);
        }
      else
        {
	  changed[i] = 'a' + ((present_char - 'a' + key) % LEN_ALPHABET);
        }

    }

  printf("ciphertext:");
  for (int i = 0; i < len; ++i)
    {
      printf("%c", changed[i]);
    }
  printf("\n");

  return 0;
}

int pow_10(int power)
{
  int product = 1;
  for (int i = 0; i < power; ++i)
    {
      product *= 10;
    }
  return product;
}
