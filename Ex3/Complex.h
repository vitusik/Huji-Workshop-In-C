
#ifndef COMPLEX_H
#define COMPLEX_H

#include "Epsilon.h"

//********types and functions types

/**
 * @struct defines a complex number
 */
typedef struct Complex
{
    double real;
    double imag;
}Complex;

/**
 * A pointer to Complex number.
 */
typedef struct Complex* ComplexP;

/**
 * A struct of a Complex table.
 */
typedef const struct Complex* ConstComplexP;

/**
 * A pointer to a compare function that receives two Constant Complex Numbers abd returns an Int
 */
typedef int (*ComFunc)(ConstComplexP, ConstComplexP);

/**
 * @brief create a new complex number
 *   
 * @param s the string that contain the number. the syntax for Complex number is
 * '<double number>,<double number>i'
 * for example : "0,0i","1.878,78i","-768,1i".
 * you can use atof for converting the <double_number>
 * @return ComplexP that is repesneted by the string. if the string do not match the
 * syntax return NULL;
 */
 ComplexP fromCharFunc(char * s);
 
 /**
 * @brief a string repesnets the complex number
 *   
 * @param num is the number.
 * @ str - Pointer to a buffer where the resulting C-string is stored.
 *    The buffer should have a size of at least n characters
 * @ n - Maximum number of bytes to be used in the buffer.
 *     The generated string has a length of at most n-1, leaving space for the additional
 *     terminating null character.
 * the syntax of the string should be '<double number>,<double number>i'
 * for example : "0,0i","1.878,78i","-768,1i".
 * @return  The number of characters that would have been written if n had been sufficiently large, 
 * not counting the terminating null character. If an encoding error occurs, a negative number
 * is returned.
 */
int toChar(char * str, int n, ConstComplexP num);

/**
 * @brief free the complex object
 *   
 * @param num is the number. 
 */
void freeComplex(ComplexP num);

/**
 * @brief create a new complex number which is the result of addition num2 to num1.
 *   
 * @param num1 - the first Complex
 * @param num1 - the second Complex
 * @return ComplexP that = num1+num2 . if num1/num2 is NULL return NULL;
 */
 ComplexP add(ConstComplexP num1, ConstComplexP num2);

 /**
 * @brief create a new complex number which is the result of multiplication of scalar*num1
 *   
 * @param num1 - scalar double number
 * @param num1 - the  Complex
 * @return ComplexP that = scalar*num . if num is NULL return NULL;
 */
 ComplexP multScalar(double scalar, ConstComplexP num);

 /**
 * @brief create a new complex number which is the result of multiplication of num1*num2
 *   
 * @param num1 - the first Complex
 * @param num1 - the second Complex
 * @return ComplexP that = num1*num2 . if num1/num2 is NULL return NULL;
 */
ComplexP mult(ConstComplexP num1, ConstComplexP num2);


/**
 * @brief create a pointer to a function that compare complex numbers with EPSILON
 *  function name:getCompareFunc
 * use the lexicographic order, comparing the real parts and then comparing the imaginary ones if the real parts are equal
 * @return a pointer to a function that gets 2 Complex pointers (num1,num2) and return int as following:
 *        0 - iff |num1-num2| <EPSILON
 *        otherwize:
 *                >0 if num1>num2
 *                <0 if num2<num1
 */

ComFunc getCompareFunc();

#endif // COMPLEX_H

