#include <stdio.h>
#include <cs50.h>

int main(void)
{
  int size;
  // Keep prompting the user for a pyramid size until it is a valid size
  do
    {
      size = get_int("Height: ");
    }
  while (size < 0 || size > 23);

  // Main loop for pyramid
  for (int i = 1; i <= size; ++i)
    {
      // Loop for left side spaces
      for (int j = 0; j < size - i; ++j)
        {
	  printf(" ");
        }
      // Loop for left side hashes
      for (int j = 0; j < i; ++j)
        {
	  printf("#");
        }
      // Separating spaces
      printf("  ");
      // Right side hashes
      for (int j = 0; j < i; ++j)
        {
	  printf("#");
        }
      printf("\n");
    }
}
