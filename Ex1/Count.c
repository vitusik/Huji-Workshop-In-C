/**
 * @file Count.c
 * @author  Vitaly Frolov vitaly.frolov@mail.huji.ac.il
 * @version 1.0
 * @date 1 Nov 2015
 * 
 * @brief System to track amount of characters used in a input.
 *
 
 * @section DESCRIPTION
 * The system track amount of characters used in a input.
 * Input  : Input by the user.
 * Process: Counts amount of characters used, words and rows.
 * Output : Notification about the amount used.
 */

 // ------------------------------ includes -----------------------------
#include <stdio.h>

// -------------------------- const definitions -------------------------
/** 
 * @def NL 10
 * @brief A macro that sets the new line ASCII code.
 */
#define NL 10
/** 
 * @def SPACE 32
 * @brief A macro that sets the space key ASCII code.
 */
#define SPACE 32
/** 
 * @def TRUE 1
 * @brief A macro that sets true value to be 1.
 */
#define TRUE 1
/** 
 * @def TRUE 1
 * @brief A macro that sets false value to be 0.
 */
#define FALSE 0

// ------------------------------ functions -----------------------------
/**
 * @brief The main function. the function prints out amount of characters,words and rows used.
 * @return 0, to tell the system the execution ended without errors.
 */
int main()
{
	int c = 0;
	int rows = 1;
	int words = 0;
	int characters = 0;
	int startedWord = FALSE;
	/* startedWord is an indicator that track whether the function is in the middle of the word 
	 * or after pressing space or enter keys
	 */
	while((c = getchar()) != EOF)
	{
		if(c == NL)
		{
			rows ++;
			characters ++;
            startedWord = FALSE;
		}
		else if (c == SPACE)
		{
			startedWord = FALSE;
			characters ++;
		}
		else
		{
            if(!startedWord)
            {
				 words ++;
			}
			startedWord = TRUE;
			characters ++;
		}	
	}	
	printf("Num of Rows:%d words:%d characters:%d\n", rows, words, characters);
	return 0;
}