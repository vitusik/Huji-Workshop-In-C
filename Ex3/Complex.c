// ------------------------------ includes ------------------------------
#include "Complex.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

// -------------------------- const definitions -------------------------
/*
 *  @var char SEPARATOR
 *  @brief set the char ',' to be the char that seperates the real and imaginary parts in the
 *   complex number string
 */
const char SEPARATOR = ',';

/*
 * @var char I
 * @brief the char whose purpose is to point to the imaginary part of the complex number
 */
const char I = 'i';

/*
 * @var char DOT
 * @brief char that represents the decimal mark in a string of a double
 */
const char DOT = '.';
/*
 * @var DIGITS
 * @brief pointer to a string of the only characters that can create a number
 */
const char* DIGITS = "0123456789.";

/*
 * @var char MINUS
 * @brief char that represent negative number
 */
const char MINUS = '-';



// ------------------------------ functions -----------------------------
/*
 * @brief function that count amount of appearances of a char in a given string
 * @param string pointer to the string in which the func counts the amount of appearances by a char
 * @param ch the character in whose appearances we are interested
 * @return amount of the times ch appeared in string
 */
static int countChars(const char* string, const char ch)
{
	int count = 0;
	int length = strlen(string);
	for (int i = 0; i < length; i++)
	{
		if (string[i] == ch)
		{
			count++;
		}
	}

	return count;
}

/*
 * @brief function that checks if a string ends with a specific char
 * @param string the string in which the check happens
 * @param ch the char 
 * @return true if ch is the last character in string otherwise false
 */
static bool endsWith(const char* string, const char ch)
{
	size_t length = strlen(string);
	// strlen returns the length including the \0 
	if(string[length - 1] == ch)
	{
		return true;
	}
	return false;
}

/*
 * @brief function that checks if a string starts with a specific char
 * @param string the string in which the check happens
 * @param ch the char
 * @return true if ch is the last character in string otherwise false
 */
static bool startsWith(const char* string, const char ch)
{
	if(string[0] == ch)
	{
		return true;
	}
	return false;
}

/*
 * @brief function that checks if a given string is a string of digits, and has at most one decimal
 *  mark
 * @param string the tested string
 * @return true if the string consists only of digits and maybe one decimal mark or less 
 */
static bool digitsCheck(const char* string)
{
	if(countChars(string, MINUS) == 1)
	{
		if(startsWith(string, MINUS))
		{
			return true;
		}
		return false;
	}
	else if(countChars(string, MINUS) > 1)
	{
		return false;
	}
	else
	{
		return true;
	}

	// verifying that there is at most one decimal mark in the string 
	if(countChars(string, DOT) > 1)
	{
		return false;
	}
	// strspn returns the length of the sub string of the original string that consits entirely 
	// from the chars in DIGITS, and compare its to the length of the original string
	// if the length is identical it means that the string is a representation of a number
	if(strspn(string, DIGITS) == strlen(string))
	{
		return true;
	}
	return false;
}

/*
 * @brief function that does a basic syntax check to a string that suppose to represent a 
 *  complex number
 * @param string the tested string
 * @return true if the string matches the format "<someString>,<someString>i"
 */
static bool syntaxCheck(char* string)
{
	if(!endsWith(string, I) || countChars(string, SEPARATOR) != 1)
	{
		return false;
	}
	string[strlen(string) - 1] = '\0';
	return true;
}

/**
 * @brief function that receives a string and returns a pointer to a complex number struct
 * @param s string representation of the complex number
 * @return pointer to a complex number struct
 */
ComplexP fromCharFunc(char * s)
{
	if(s == NULL)
	{
		return NULL;
	}
	if(!syntaxCheck(s))
	{
		return NULL;
	}
	ComplexP comNum = malloc(sizeof(Complex));
	// memory allocation has failed
	if(comNum == NULL)
	{
		return NULL;
	}
	char *temp;
	char search[] = {SEPARATOR};
	temp = strtok(s, search);
	// the string until the ',' is empty string thus bad format
	if(temp == NULL)
	{
		return NULL;
	}
	// checking the real part of the string is made from digits
	if(!digitsCheck(temp))
	{
		return NULL;
	}
	comNum->real = atof(temp);
	temp = strtok(NULL, search);
	// the string after the ',' is an empty string
	if(temp == NULL)
	{
		return NULL;
	}
	if(!digitsCheck(temp))
	{
		return NULL;
	}
	comNum->imag = atof(temp);
	return comNum;
}

/**
 * @brief function that checks if a complex muber object can be stored in a string buffer
 * @param str the string that supposed to be the container for the complex number
 * @param n is the amount of char allowed in str
 * @param num is a pointer to complex number struct
 * @return int that represents amount of bytes the complex number string representation will take in
 *  the buffer, neggative -1 if the container is too small for the string representation of the num
 */
int toChar(char * str, int n, ConstComplexP num)
{
	// assumption: the length of string is a positive number and that the complex num pointer is
	// initialized
	assert(n > 0 && num != NULL);
	char* string1 = malloc(strlen(str));
	char* string2 = malloc(strlen(str));
	int realAmountOfDigits = snprintf(string1, n, "%f", num->real);
	string1[realAmountOfDigits] = SEPARATOR;
	string1[realAmountOfDigits + 1] = '\0';
	int imagAmountOfDigits = snprintf(string2, n, "%f", num->imag);
	string2[imagAmountOfDigits] = I;
	string2[imagAmountOfDigits + 1] = '\0';
	int lengthOfString = realAmountOfDigits + imagAmountOfDigits + 2;
	// one for the SEPARATOR that separates the real and the imaginary, and another one for the i
	if(lengthOfString > n -1)
	{
		free(string1);
		free(string2);
		return -1;
	}
	else
	{
		strcat(str, string1);
		strcat(str, string2);
		free(string1);
		free(string2);
		return lengthOfString;
	}
}

/**
 * @brief frees the memory alloc for complex num
 * @param num the complex number pointer that needs to be free'd
 */
void freeComplex(ComplexP num)
{
	free(num);
}

/**
 * @brief function that represents the addition operation in the complex numbers
 * @param num1 the first complex number pointer
 * @param num2 the second complex number pointer
 * @return the result of the operation, in case one of the pointer is NULL, NULL is returned
 */
ComplexP add(ConstComplexP num1, ConstComplexP num2)
{
	if(num1 == NULL || num2 == NULL)
	{
		return NULL;
	}
	else
	{
		ComplexP complexP = malloc(sizeof(Complex));
		complexP->real = num1->real + num2->real;
		complexP->imag = num1->imag + num2->imag;
		return complexP;
	}
}

/**
 * @brief function that represents multiplication of a complex number by a scalar
 * @param scalar the amount the num should be multiplied by
 * @param num the complex number pointer
 * @return the result of the operation, in case of that the pointer is NULL, NULL is returned
 */
ComplexP multScalar(double scalar, ConstComplexP num)
{
	if(num == NULL)
	{
		return NULL;
	}
	else
	{
		ComplexP complexP = malloc(sizeof(Complex));
		complexP->real = num->real * scalar;
		complexP->imag = num->imag * scalar;
		return complexP;
	}
}

/**
 * @brief function that represents the multiplication operation between two complex numbers
 * @param num1 the first complex number pointer
 * @param num2 the second complex number pointer
 * @return the result of the operation, in case one of the pointer is NULL, NULL is returned
 */
ComplexP mult(ConstComplexP num1, ConstComplexP num2)
{
	if(num1 == NULL || num2 == NULL)
	{
		return NULL;
	}
	else
	{
		ComplexP complexP = malloc(sizeof(Complex));
		complexP->real = (num1->real * num2->real) - (num1->imag * num2->imag);
		complexP->imag = (num1->real * num2->imag) + (num1->imag * num2->real);
		return complexP;
	}
}

/*
 * @brief function that compare two complex numberes, first by their real pars, and then by thier
 *  imaginary parts
 * @param num1 the first complex number pointer
 * @param num2 the second complex number pointer
 * @return 1 if num1 is greater by EPSILON then num2, 0 in case they are even, -1 if num2 is greater
 */
static int compare(ConstComplexP num1, ConstComplexP num2)
{
	if(num1->real - num2->real <= EPSILON && num1->real - num2->real >= -EPSILON)
	{
		if(num1->imag - num2->imag <= EPSILON && num1->imag - num2->imag >= -EPSILON)
		{
			return 0;
		}
		else
		{
			if(num1->imag > num2->imag)
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
	}
	else
	{
		if(num1->real > num2->real)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
}

/**
 * @brief function that returns pointer to a compare function
 * @return pointer to a compare function
 */
ComFunc getCompareFunc()
{
	// if there were more compare function then ther would be a swtich statemnet, and the function
	// would receive some kind of argument
	return &compare;
}

