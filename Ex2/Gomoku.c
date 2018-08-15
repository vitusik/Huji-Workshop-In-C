// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Board.h"

// -------------------------- const definitions -------------------------
/**
 * @def PLAYER1 'X'
 * @brief A macro that sets player 1 to be X
 */
#define PLAYER1 'X'

/**
 * @def PLAYER2 'O'
 * @brief A macro that sets player 2 to be O
 */
#define PLAYER2 'O'

/**
 * @def EMPTY_SQUARE ' '
 * @brief A macro that sets the empty square
 */
#define EMPTY_SQUARE ' '


/**
 * @def MOVE 1
 * @brief A macro that sets move option to be 1
 */
#define MOVE 1

/**
 * @def DEL_MOVE 1
 * @brief A macro that sets delete move option to be 2
 */
#define DEL_MOVE 2

/**
 * @def CHANGE_PRINT_LOC 3
 * @brief A macro that sets print location change option to be 3
 */
#define CHANGE_PRINT_LOC 3

/**
 * @def EXIT 4
 * @brief A macro that sets exit option to be 4
 */
#define EXIT 4

/**
 * @def INPUT_FILE 1
 * @brief A macro that sets the input file to be 1
 */
#define INPUT_FILE 1

/**
 * @def OUTPUT_FILE 2
 * @brief A macro that sets the output file to be 2
 */
#define OUTPUT_FILE 2

/**
 * @def AMOUNT_OF_FILES 2
 * @brief A macro that sets the amount of files the func receives
 */
#define AMOUNT_OF_FILES 2

typedef struct Board
{
    // for the size of the board
    int _numOfRows;
    int _numOfCols;
    // the current coordinate for print board func
    int _curRow;
    int _curCol;
    int _lastTurnRow;
    int _lastTurnCol;
    char _whosTurn;
    char **ptrBoardArr;

}Board;

/**
 * @brief switches between the current player to the next.
 * @param boardP the board
 */
static void swtichPlayer(BoardP boardP)
{
    if(boardP->_whosTurn == PLAYER1)
    {
        boardP->_whosTurn = PLAYER2;
    }
    else
    {
        boardP->_whosTurn = PLAYER1;
    }
}

/**
 * @brief function that checks if the line in the file is in this format (int,int).
 * @param x pointer to an int that represents the x coordinate
 * @param y pointer to an int that represents the y coordinate
 * @param inputStream the stream that used
 * @param lineNum the current number of the line in the file
 * @param board the board
 */
static void coordinateCheck(int *x, int *y, FILE *inputStream, int lineNum, BoardP boardP)
{
    // fscanf returns 2 when it successfully scanned two integers
    if(fscanf(inputStream, "(%d,%d)\n", x, y) != 2)
    {
        fprintf(stderr, "Wrong format input file at line %d", lineNum);
        freeBoard(boardP);
        exit(1);
    }
}

/**
 * @brief makes a move.
 * @param boardP the board
 * @param lineNum the current line in the file
 * @param inputStream the stream thats used
 */
void makeMove(BoardP boardP, int lineNum, FILE *inputStream)
{
    int x, y;
    coordinateCheck(&x, &y, inputStream, lineNum, boardP);
    if(putBoardSquare(boardP, x, y, boardP->_whosTurn))
    {
        //after a successful turn, there is need to swtich players, and to update
        //the last used x,y coordinates
        boardP->ptrBoardArr[x][y] = boardP->_whosTurn;
        boardP->_lastTurnRow = x;
        boardP->_lastTurnCol = y;
    }

}

/**
 * @brief deletes a move.
 * @param boardP the board
 * @param lineNum the current line in the file
 * @param inputStream the stream thats used
 */
void delMove(BoardP boardP, int lineNum, FILE *inputStream)
{
    int x, y;
    coordinateCheck(&x, &y, inputStream, lineNum, boardP);
    if(cancelMove(boardP, x, y))
    {
        //if its possible to remove the char then its changed to an empty space
        boardP->ptrBoardArr[x][y] = EMPTY_SQUARE;
    }

}

/**
 * @brief changes the printing location.
 * @param boardP the board
 * @param lineNum the current line in the file
 * @param inputStream the stream thats used
 */
void changePrintLoc(BoardP board, int lineNum, FILE *inputStream)
{
    int x, y;
    coordinateCheck(&x, &y, inputStream, lineNum, board);
    //the x,y coordinates are positive numbers
    assert(x >= 0 && y >= 0);
    board->_curRow = x;
    board->_curCol = y;
}

/**
 * @brief exits the game.
 * @param boardP the board
 * @param inputStream the stream thats used
 */
void exitPrint(BoardP board, FILE *outputStream)
{
    fprintf(outputStream, "\nPrinting current state of the board for location (row,col)=(%d,%d)\n\n"
            , board->_curRow, board->_curCol);
    printBoard(outputStream, board, board->_curRow, board->_curCol);
    fprintf(outputStream, "\nThe game ended with a tie...\n");
    freeBoard(board);
}

/**
 * @brief checks if the given files in the arg line could be open.
 * @param f1 the first file's pointer
 * @param f1Name the first file's name
 * @param f2 the second's file's pointer
 * @param f2Name the second's file's name
 */
void fileCheck(FILE *f1, char *f1Name, FILE *f2, char *f2Name)
{
    if(f1 == NULL)
    {
        fprintf(stderr, "Can not open file: %s", f1Name);
        exit(1);
    }
    else if(f2 == NULL)
    {
        fprintf(stderr, "Can not open file: %s", f2Name);
        exit(1);
    }
}

/**
 * @brief the main function that runs the Gomoku game.
 * @param argc amount of arguments
 * @param argv array of those args that stores thiers name
 */
int main(int argc, char *argv[])
{
    // argc include's the exe file as an argument
    if(argc != AMOUNT_OF_FILES + 1)
    {
        fprintf(stderr, "Wrong parameters. Usage:\nGomoku <input_file> <output_file>");
        exit(1);
    }
    FILE *input = fopen(argv[INPUT_FILE], "r");
    FILE *output = fopen(argv[OUTPUT_FILE], "w");
    fileCheck(input, argv[INPUT_FILE], output, argv[OUTPUT_FILE]);
    BoardP boardP = createNewDefaultBoard();
    int lineNum = 0;
    do
    {
        int choice;
        // if we reached EOF when we supposed to receive an int representing the next action
        // then the game ends with a tie
        if(feof(input))
        {
            exitPrint(boardP, output);
            return 0;
        }
        // if fscanf couldn't scan one int it means that there is an illegal command
        if(fscanf(input, "%d\n", &choice) != 1)
        {
            fprintf(stderr, "Illegal command at lineNum %d", lineNum);
            freeBoard(boardP);
            exit(1);
        }
        else
        {
            // this else is accessed only once
            lineNum++;
        }
        lineNum++;
        // if we reached EOF when we supposed to receive two int representing the coordinate
        // then the game ends with a tie
        if(feof(input))
        {
            exitPrint(boardP, output);
            return 0;
        }
        switch (choice)
        {
            case MOVE:
                makeMove(boardP, lineNum + 1, input);
                if(getWinner(boardP) != EMPTY_SQUARE)
                {
                    printBoard(output, boardP, boardP->_curRow, boardP->_curCol);
                    fprintf(output, "\nPlayer ‘%c’ won!!!\n", getWinner(boardP));
                    return 0;
                }
                swtichPlayer(boardP);
                lineNum ++;
                break;
            case DEL_MOVE:
                delMove(boardP, lineNum + 1, input);
                lineNum ++;
                break;
            case CHANGE_PRINT_LOC:
                changePrintLoc(boardP, lineNum + 1, input);
                lineNum ++;
                break;
            case EXIT:
                exitPrint(boardP, output);
                return 0;
            default:
                fprintf(stderr, "Illegal command at lineNum %d", lineNum);
                freeBoard(boardP);
                exit(1);
        }

    }while(true);
}
