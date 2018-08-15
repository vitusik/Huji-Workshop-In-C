#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Complex.h"
#include "GenGroup.h"

/*
 * @var DEFAULT_STRING_LENGTH
 * @brief int that represents default string length
 */
const int DEFAULT_STRING_LENGTH = 256;

/*
 * @brief func that checks if a given set of complex numbers is an abelian group
 * @param complexStrings array of strings that represents the complex number
 * @param identityElem the string of the complex num that is the identity element in the set
 * @parm numOfElements amount of elements in the group
 */
static void complexIsAbelianGroup(char** complexStrings, char* identityElem, const int numOfElements)
{
    ComplexP iden = fromCharFunc(identityElem);
    ComplexP* members = (ComplexP*)malloc(sizeof(Complex)*numOfElements);
    char* string = malloc(sizeof(char) * DEFAULT_STRING_LENGTH);
    string[0] = '\0';
    for (int j = 0; j < numOfElements; ++j)
    {
        members[j] = fromCharFunc(complexStrings[j]);
    }
    GroupMemberOperFuncP oper = (GroupMemberOperFuncP) mult;
    GroupMemberFreeFuncP freeCom = (GroupMemberFreeFuncP)freeComplex;
    GroupMemberCompareFuncP com = (GroupMemberCompareFuncP )getCompareFunc();
    printf("Complex Group with the regular multi func:\n");
    toChar(string, DEFAULT_STRING_LENGTH, iden);
    printf("Identity Numer : %s\n", string);
    free(string);
    printf("Group Members: ");
    for (int i = 0; i < numOfElements; i++)
    {
        string = malloc(sizeof(char) * DEFAULT_STRING_LENGTH);
        string[0] = '\0';
        toChar(string, DEFAULT_STRING_LENGTH, members[i]);
        printf("%s ", string);
        free(string);
        if(i + 1 == numOfElements)
        {
            printf("\n");
        }
        else
        {
            printf("; ");
        }

    }
    bool isAbelG = isAbelianGroup((GroupMemberP)iden, (GroupMemberP) members, numOfElements,
                                  oper, freeCom, com);
    if(isAbelG)
    {
        printf("This is Abelian Group\n\n");
    }
    else
    {
        printf("This is not Abelian Group\n\n");
    }
    for (int j = 0; j < numOfElements; j++)
    {
        free(members[j]);
    }
    free(iden);
    free(members);
}

/*
 * @brief func that calculates the result of A*B, when A,B are integers
 * @param a pointer to the first integer
 * @param b pointer to the second integer
 * @return pointer to the result of the multiplication between a and b
 */
static int* multInt(const int* a, const int* b)
{
    int* ptr = malloc(sizeof(int));
    *ptr = (*a) * (*b);
    return ptr;
}

/*
 * @brief func the caompare integers
 * @param a pointer to the first integer
 * @param b pointer to the second integer
 * @return 1 if a > b, -1 if a < b, 0 if a = b
 */
static int compareInt(const int* a, const int* b)
{
    if(*a > *b)
    {
        return 1;
    }
    else if(*a < *b)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/*
 * @brief func that transforms int to a pointer of int
 * @param a the intger that the func will create a pointer to
 * @return pointer to the value that a has stored
 */
static int* intToPtr(const int a)
{
    int* ptr = malloc(sizeof(int));
    *ptr = a;
    return ptr;
}

/*
 * @brief func that checks if a given set of integers is an abelian group
 * @param intString array of strings that represents the integers
 * @param identityElem the string of the integer that is the identity element in the set
 * @parm numOfElements amount of elements in the group
 */
static void integersIsAbelianGroup(char** intString, char* identityElem, const int numOfElements)
{
    int* iden = intToPtr(atoi(identityElem));
    int** intArr = (int**)malloc(sizeof(int*) * numOfElements);
    for (int i = 0; i < numOfElements; ++i)
    {

        intArr[i] = intToPtr(atoi(intString[i]));
    }
    printf("Integer Group with the regular multi func:\n");
    printf("Identity Numer : %d\n", *iden);
    printf("Group Members: ");
    for (int i = 0; i < numOfElements; i++)
    {
        printf("%d ", *intArr[i]);
        if(i + 1 == numOfElements)
        {
            printf("\n");
        }
        else
        {
            printf("; ");
        }
    }
    GroupMemberOperFuncP oper = (GroupMemberOperFuncP) multInt;
    GroupMemberFreeFuncP freeInt = (GroupMemberFreeFuncP)free;
    GroupMemberCompareFuncP com = (GroupMemberCompareFuncP )compareInt;
    bool isAbelG = isAbelianGroup((GroupMemberP )iden, (GroupMemberP) intArr, numOfElements,
                                  oper, freeInt, com);
    if(isAbelG)
    {
        printf("This is Abelian Group\n\n");
    }
    else
    {
        printf("This is not Abelian Group\n\n");
    }
    for (int j = 0; j < numOfElements; j++)
    {
        free(intArr[j]);
    }
    free(iden);
    free(intArr);
}

/**
 * @brief the main function, for each group of strings prints out whether they are abelian group
 *        or not
 * @return 0 , the run of the program ended with no errors
 */
int main()
{
    char** numStrings;
    // I've decided to go with 2-member groups
    int variableNumberOfElements = 2;
    // initializing array of strings
    numStrings = malloc(variableNumberOfElements * sizeof(char*));
    for (int i = 0; i < variableNumberOfElements; i++)
    {
        numStrings[i] = malloc((DEFAULT_STRING_LENGTH) * sizeof(char));
    }
    // first group
    char complex1[] = "1,0i";
    char complex2[] = "0,0i";
    strcpy(numStrings[0], complex1);
    strcpy(numStrings[1], complex2);
    complexIsAbelianGroup(numStrings, complex1, variableNumberOfElements);
    // second group
    char complex3[] = "1,0i";
    char complex4[] = "-1,0i";
    strcpy(numStrings[0], complex3);
    strcpy(numStrings[1], complex4);
    complexIsAbelianGroup(numStrings, complex3, variableNumberOfElements);
    // third group
    char int1[] = "5";
    char int2[] = "7";
    strcpy(numStrings[0], int1);
    strcpy(numStrings[1], int2);
    integersIsAbelianGroup(numStrings, int1, variableNumberOfElements);
    // fourth group
    char int3[] = "1";
    char int4[] = "-1";
    strcpy(numStrings[0], int3);
    strcpy(numStrings[1], int4);
    integersIsAbelianGroup(numStrings, int3, variableNumberOfElements);

    // freeing the numStrings array
    for (int j = 0; j < variableNumberOfElements; j++)
    {
        free(numStrings[j]);
    }
    free(numStrings);

    return 0;
}