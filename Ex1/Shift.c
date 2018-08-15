/**
 * @file Shift.c
 * @author  Vitaly Frolov vitaly.frolov@mail.huji.ac.il
 * @version 1.0
 * @date 1 Nov 2015
 *
 * @brief System encrypts\decrypts user input.
 *

 * @section DESCRIPTION
 * The system encrypts\decrypts user input.
 * Input  : Input by the user.
 * Process: encrypts\decrypts user input by shifting chars.
 * Output : Encrypted\Decrypted input.
 */

// ------------------------------ includes -----------------------------
#include <stdio.h>
#include "Shift.h"

// -------------------------- const definitions -------------------------
/**
 * @def LOW_CASE_LOW_BORDER
 * @brief A macro which is the letter 'a' in ASCII code.
 */
#define LOW_CASE_LOW_BORDER 97

/**
 * @def LOW_CASE_UPPER_BORDER
 * @brief A macro which is the letter 'z' in ASCII code.
 */
#define LOW_CASE_UPPER_BORDER 122

/**
 * @def UPPER_CASE_LOW_BORDER
 * @brief A macro which is the letter 'A' in ASCII code.
 */
#define UPPER_CASE_LOW_BORDER 65

/**
 * @def UPPER_CASE_UPPER_BORDER
 * @brief A macro which is the letter 'Z' in ASCII code.
 */
#define UPPER_CASE_UPPER_BORDER 90

/**
 * @def NUM_LOW_BORDER
 * @brief A macro which is the number '0' in ASCII code.
 */
#define NUM_LOW_BORDER 48

/**
 * @def NUM_UPPER_BORDER
 * @brief A macro which is the number '9' in ASCII code.
 */
#define NUM_UPPER_BORDER 57

/**
 * @def DECRYPT
 * @brief A macro sets the letter 'd'.
 */
#define DECRYPT 'd'

/**
 * @def ENCRYPT
 * @brief A macro sets the letter 'e'.
 */
#define ENCRYPT 'e'

/**
 * @def EOS
 * @brief A macro which sets the end of string.
 */
#define EOS '\0'

// ------------------------------ functions -----------------------------

/**
 * @brief Shifts the character that was received by the desired amount.
 * @param lowerBound The lower boundary of the characters set
 * @param upperBound The upper boundary of the characters set
 * @param shift The desired amount for the character to be shifted
 * @param c The character that needs to be shifted
 * @return The character c after it was shifted
 */
char charShifter(int lowerBound, int upperBound, int shift, char c)
{
    // cleanShift is needed in case the shift amount exceeds the amount of characters more than once
    int cleanShift = shift - (shift / (upperBound - lowerBound)) * (upperBound - lowerBound);
    if(upperBound < c + shift)
    {
        // In case the shift amount exceeds the upper boundary
        return (lowerBound + cleanShift - ( upperBound - c + 1));
    }
    else
    {
        return c + cleanShift;
    }
}

/**
 * @brief Decrypts the character that was received by the desired amount.
 * @param shift The desired amount for the character to be shifted
 * @param c The character that needs to be shifted
 * @return The character c after it was decrypted
 */
char decrypt(int shift, char c)
{
    /* Decryption is actually encryption in the other direction, so the only thing needs to be
     * changed is the shift
     */
    int decryptedChar;
    if(LOW_CASE_LOW_BORDER <= c && c <= LOW_CASE_UPPER_BORDER)
    {
        decryptedChar = charShifter(LOW_CASE_LOW_BORDER, LOW_CASE_UPPER_BORDER,
                                    LOW_CASE_UPPER_BORDER - LOW_CASE_LOW_BORDER - shift + 1, c);
    }
    else if(UPPER_CASE_LOW_BORDER <= c && c <= UPPER_CASE_UPPER_BORDER)
    {
        decryptedChar = charShifter(UPPER_CASE_LOW_BORDER, UPPER_CASE_UPPER_BORDER,
                                    UPPER_CASE_UPPER_BORDER - UPPER_CASE_LOW_BORDER - shift + 1, c);
    }
    else if(NUM_LOW_BORDER <= c && c <= NUM_UPPER_BORDER)
    {
        decryptedChar = charShifter(NUM_LOW_BORDER, NUM_UPPER_BORDER,
                                    NUM_UPPER_BORDER - NUM_LOW_BORDER - shift + 1, c);
    }
    else
    {
        // in case the char that was given ain't a letter or a number
        return c;
    }
    return decryptedChar;
}

/**
 * @brief Encrypts the character that was received by the desired amount.
 * @param shift The desired amount for the character to be shifted
 * @param c The character that needs to be shifted
 * @return The character c after it was encrypted
 */
char encrypt(int shift, char c)
{
    char encryptedChar;
    if(LOW_CASE_LOW_BORDER <= c && c <= LOW_CASE_UPPER_BORDER)
    {
        encryptedChar = charShifter(LOW_CASE_LOW_BORDER, LOW_CASE_UPPER_BORDER, shift, c);
    }
    else if(UPPER_CASE_LOW_BORDER <= c && c <= UPPER_CASE_UPPER_BORDER)
    {
        encryptedChar = charShifter(UPPER_CASE_LOW_BORDER, UPPER_CASE_UPPER_BORDER, shift, c);
    }
    else if(NUM_LOW_BORDER <= c && c <= NUM_UPPER_BORDER)
    {
        encryptedChar = charShifter(NUM_LOW_BORDER, NUM_UPPER_BORDER, shift, c);
    }
    else
    {
        return c;
    }
    return encryptedChar;
}

/**
 * @brief Gets the desired shift from the user.
 * @return The desired shift.
 */
int desiredShift()
{
    int shift;
    printf("Please enter the shift amount:\n");
    scanf("%d", &shift);
    getchar();
    // getchar is used so when pressing the Enter key it won't be collected by the next scanf
    while(shift < 0 || shift > 50)
    {
        printf("ERROR: Shift amount should be number between 0 to 50\n");
        printf("Please enter the shift amount:\n");
        scanf("%d", &shift);
    }
    return shift;
}

/**
 * @brief Gets the desired action from the user encryption or decryption.
 * @return 1 for encryption, 0 for decryption.
 */
int desiredAction()
{
    char action;
    printf("Would you like to encrypt (e) or decrypt (d)?\n");
    scanf("%c", &action);
    getchar();
    while(action != DECRYPT && action != ENCRYPT)
    {
        printf("ERROR: You should type e or d\n");
        printf("Would you like to encrypt (e) or decrypt (d)?\n");
        action = getchar();
        getchar();
    }
    if (action == ENCRYPT)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief The main function. the function prints the encrypted\decrypted users input.
 * @return 0, to tell the system the execution ended without errors.
 */
int main()
{
    int shift, i = 0, action;
    char string[101];
    /* the size of the array is 101 in case the user input is 100 chars long, so the function can
     * add the EOS without deleting the last char
     */
    char alteredString[101];
    shift = desiredShift();
    action = desiredAction();
    fgets(string, sizeof(string), stdin);
    while(string[i] && i <= 100)
    {
        if(string[i] == '\n')
        {
            string[i] = EOS;
        }
        switch (action)
        {
            case 1:
                alteredString[i] = encrypt(shift, string[i]);
                break;
            case 0:
                alteredString[i] = decrypt(shift, string[i]);
                break;
        }
        i++;
    }

    printf("\"%s\" -> \"%s\"\n", string, alteredString);
    return 0;
}

