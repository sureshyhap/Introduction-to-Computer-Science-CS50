#include <stdio.h>
#include <cs50.h>

long long _10_to_power(int power);

int main(void)
{
  long long credit_num = get_long_long("Number: ");

  // Checks if the number is in range
  if (credit_num >= 1e17 || credit_num < 1e12)
    {
      printf("INVALID\n");
      return 0;
    }

  // Powers of 10 that will be used to check the amount of digits in the credit card number
  long long _16_zeros = 1e16;
  long long _15_zeros = 1e15;
  long long _14_zeros = 1e14;
  long long _13_zeros = 1e13;
  long long _12_zeros = 1e12;

  if ((credit_num % _14_zeros) == credit_num && (credit_num % _13_zeros) != credit_num)
    {
      // 14 digits
      printf("INVALID\n");
      return 0;
    }

  int length;

  if ((credit_num % _13_zeros) == credit_num && (credit_num % _12_zeros) != credit_num)
    {
      length = 13;
    }
  else if ((credit_num % _15_zeros) == credit_num && (credit_num % _14_zeros) != credit_num)
    {
      length = 15;
    }
  else
    {
      length = 16;
    }

  int second_dig_sum = 0;
  // Loop for extracting and summing the digits starting from the second to last
  for (int present_dig_prod = 0, pow_of_10 = 2, pres_dig = pow_of_10; pres_dig <= length; pow_of_10 += 2, pres_dig += 2)
    {
      // Extracts the particular digit and multiplies it by 2
      present_dig_prod = 2 * (((credit_num % _10_to_power(pow_of_10)) - (credit_num % _10_to_power(pow_of_10 - 1))) / _10_to_power(
																   pow_of_10 - 1));
      // If after the multiplication the product is greater than 10, extract and sum the digits of the number that is
      // between 10 and 18 inclusive (if the original digit was in the range 5 - 9 inclusive)
      if (present_dig_prod >= 10)
        {
	  second_dig_sum += (1 + (present_dig_prod - 10));
	  continue;
        }
      second_dig_sum += present_dig_prod;
    }

  int first_dig_sum = 0;
  for (int present_dig_prod = 0, pow_of_10 = 1, pres_dig = pow_of_10; pres_dig <= length; pow_of_10 += 2, pres_dig += 2)
    {
      // Prevents division by zero in the else block
      if (pow_of_10 - 1 == 0)
        {
	  present_dig_prod = 1 * (credit_num % _10_to_power(pow_of_10));
        }
      else
        {
	  // Extracts the particular digit and multiplies it by 2
	  present_dig_prod = 1 * (((credit_num % _10_to_power(pow_of_10)) - (credit_num % _10_to_power(pow_of_10 - 1))) / _10_to_power(
																       pow_of_10 - 1));
        }
      first_dig_sum += present_dig_prod;

    }

  int total_sum = second_dig_sum + first_dig_sum;
  bool possibly_valid;
  if (total_sum % 10 == 0)
    {
      // The number is valid with respect to Luhn's algorithm but the specific card number must still be checked to see if it
      // matches any of the card companies' standards
      possibly_valid = true;
    }
  else
    {
      possibly_valid = false;
    }

  if (possibly_valid == false)
    {
      printf("INVALID\n");
      return 0;
    }

  int first_dig = 0, second_dig = 0;
  switch (length)
    {
      // Possible AMEX
    case 15:
      first_dig = ((credit_num % _15_zeros) - (credit_num % _14_zeros)) / _14_zeros;
      second_dig = ((credit_num % _14_zeros) - (credit_num % _13_zeros)) / _13_zeros;
      if (first_dig == 3)
	{
	  if (second_dig == 4 || second_dig == 7)
	    {
	      printf("AMEX\n");
	    }
	  else
	    {
	      printf("INVALID\n");
	    }
	}
      else
	{
	  printf("INVALID\n");
	}
      break;
      // Possible VISA
    case 13:
      first_dig = ((credit_num % _13_zeros) - (credit_num % _12_zeros)) / _12_zeros;
      if (first_dig == 4)
	{
	  printf("VISA\n");
	}
      else
	{
	  printf("INVALID\n");
	}
      break;
      // Possibly MASTERCARD or VISA
    case 16:
      first_dig = ((credit_num % _16_zeros) - (credit_num % _15_zeros)) / _15_zeros;
      if (first_dig == 4)
	{
	  printf("VISA\n");
	}
      else if (first_dig == 5)
	{
	  second_dig = ((credit_num % _15_zeros) - (credit_num % _14_zeros)) / _14_zeros;
	  If (second_dig >= 1 && second_dig <= 5)
	    {
	      printf("MASTERCARD\n");
	    }
	  else
	    {
	      printf("INVALID\n");
	    }
	}
      else
	{
	  printf("INVALID\n");
	}
      break;
    }

  return 0;
}

// Returns 10 to a power
long long _10_to_power(int power)
{
  long long product = 1;
  for (; power > 0; --power)
    {
      product *= 10;
    }
  return product;
}