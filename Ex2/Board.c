// ------------------------------ includes ------------------------------
#include "Board.h"
#include "ErrorHandle.h"
#include <stdlib.h>
#include <assert.h>

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
 * @def ERROR 'E'
 * @brief A macro that sets the error to be E
 */
#define ERROR 'E'

/**
 * @var int DEFAULT_ROW_SIZE
 * @brief Sets the default number of rows.
 */
int const DEFAULT_ROW_SIZE = 10;
/**
 * @var int DEFAULT_COL_SIZE
 * @brief Sets the default number of cols.
 */
int const DEFAULT_COL_SIZE = 10;

/**
 * @var int DEFAULT_ROW_STARTING_COORDINATE
 * @brief Sets the default row coordinate for printing the board.
 */
int const DEFAULT_ROW_STARTING_COORDINATE = 0;

/**
 * @var int DEFAULT_COL_STARTING_COORDINATE
 * @brief Sets the default row coordinate for printing the board.
 */
int const DEFAULT_COL_STARTING_COORDINATE = 0;

/**
 * @var int AMOUNT_TO_WIN
 * @brief Sets the amount vals needed to win.
 */
int const AMOUNT_TO_WIN = 5;

/**
 * @var int DEFAULT_PRINT
 * @brief Sets the amount of rows and cols that will be printed when printing the board.
 */
int const DEFAULT_PRINT = 10;

/**
 * @var int SIZE_MULTIPLIER
 * @brief Sets the amount that a row\col will be multiplied by when exceeding the boadrs bounds.
 */
int const SIZE_MULTIPLIER = 2;

/**
 * @struct defines struc in the name of Board.
 */
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
 * @brief creates new playing board.
 * @param rows the amount of rows in the board
 * @param cols the amount of cols in the board
 * @return pointer to a Board struct
 */
BoardP createNewBoard(int rows, int cols)
{
    BoardP p = malloc(sizeof(Board));
    if(p == NULL)
    {
        reportError(MEM_OUT);
        return NULL;
    }
    p->_curRow = DEFAULT_ROW_STARTING_COORDINATE;
    p->_curCol = DEFAULT_COL_STARTING_COORDINATE;
    p->_numOfRows = rows;
    p->_numOfCols = cols;
    p->_whosTurn = PLAYER1;
    char** arrayPointer = (char**)malloc(sizeof(char*) * p->_numOfRows);
    if(arrayPointer == NULL)
    {
        p->_numOfRows = 0;
        freeBoard(p);
        reportError(MEM_OUT);
        return NULL;
    }
    for (int i = 0; i < p->_numOfRows; i++)
    {
        arrayPointer[i] = (char*)malloc(sizeof(char) * p->_numOfCols);
        if(arrayPointer[i] == NULL)
        {
            p->_numOfRows = i;
            freeBoard(p);
            reportError(MEM_OUT);
            return NULL;
        }
    }
    p->ptrBoardArr = arrayPointer;
    for (int i = 0; i < p->_numOfRows; i++)
    {
        for (int j = 0; j < p->_numOfCols; j++)
        {
            p->ptrBoardArr[i][j] = EMPTY_SQUARE;
        }
    }
    return p;
}

/**
 * @brief creates new playing board with default arguments.
 * @return pointer to a Board struct
 */
BoardP createNewDefaultBoard()
{
    return createNewBoard(DEFAULT_ROW_SIZE, DEFAULT_COL_SIZE);
}

/**
 * @brief creates exact copy of the given board.
 * @param originalBoard the board that is copied
 * @return pointer to a Board struct
 */
BoardP duplicateBoard(ConstBoardP originalBoard)
{
    assert(originalBoard != NULL);
    BoardP p = createNewBoard(originalBoard->_numOfRows, originalBoard->_numOfCols);
    p->_curCol = originalBoard->_curCol;
    p->_curRow = originalBoard->_curRow;
    p->_lastTurnCol = originalBoard->_lastTurnCol;
    p->_lastTurnRow = originalBoard->_lastTurnRow;
    p->_whosTurn = originalBoard->_whosTurn;
    for (int i = 0; i < originalBoard->_numOfRows; i++)
    {
        for (int j = 0; j < originalBoard->_numOfCols; j++)
        {
            p->ptrBoardArr[i][j] = originalBoard->ptrBoardArr[i][j];
        }
    }
    return p;
}

/**
 * @brief func that decides the new boundary of the board.
 * @param oldSizeBound the board's original row\col size
 * @param coordinate another boundary that may replace the old one
 * @return the original boundary if the new ones is smaller, otherwise will return the new one
 *         with twice the size
 */
static int chooseSize(int oldSizeBound, int coordinate)
{
    if(oldSizeBound > coordinate)
    {
        return oldSizeBound;
    }
    else
    {
        return coordinate * SIZE_MULTIPLIER;
    }
}

/**
 * @brief creates a copy of the board and increases its boundaries.
 * @param originalBoard the board that will be copied
 * @param rows the amount of rows in the board
 * @param cols the amount of cols in the board
 * @return pointer to a Board struct
 */
static BoardP resizeBoard(BoardP originalBoard, int row, int col)
{
    assert(originalBoard != NULL);
    int newRow = chooseSize(originalBoard->_numOfRows, row);
    int newCol = chooseSize(originalBoard->_numOfCols, col);
    BoardP enlargedBoard = createNewBoard(newRow, newCol);
    if(enlargedBoard != NULL)
    {
        enlargedBoard->_curCol = originalBoard->_curCol;
        enlargedBoard->_curRow = originalBoard->_curRow;
        enlargedBoard->_lastTurnCol = originalBoard->_lastTurnCol;
        enlargedBoard->_lastTurnRow = originalBoard->_lastTurnRow;
        enlargedBoard->_whosTurn = originalBoard->_whosTurn;
        for (int i = 0; i < originalBoard->_numOfRows; i++)
        {
            for (int j = 0; j < originalBoard->_numOfCols; j++)
            {
                enlargedBoard->ptrBoardArr[i][j] = originalBoard->ptrBoardArr[i][j];
            }
        }
        return enlargedBoard;
    }
    else
    {
        return NULL;
    }

}


/**
 * @brief checks if the row is a negative num.
 * @param x the row
 * @return true\false
 */
static bool isBadRowCol(int x)
{
    if (x < 0)
    {
        return true;
    }
    return false;
}

/**
 * @brief checks char of the player is bad.
 * @param v the char
 * @return true\false
 */
static bool isBadVal(char v)
{
    if(v != PLAYER1 && v != PLAYER2)
    {
        return true;
    }
    return false;
}

/**
 * @brief checks if its the right player's turn.
 * @param board the gaming board
 * @param val the char representing the number of the player
 * @return true\false
 */
static bool isYourTurn(BoardP board, char val)
{
    assert(board != NULL);
    if(board->_whosTurn == val)
    {
        return true;
    }
    return false;
}

/**
 * @brief returns the char located at [row][col].
 * @param theBoard the board
 * @param row the x coordinate
 * @param col the y coordinate
 * @return the char 'X','O',' '
 */
char getBoardSquare(ConstBoardP theBoard, int row, int col)
{
    assert(theBoard != NULL);
    if(isBadRowCol(row) || isBadRowCol(col))
    {
        reportError(OUT_OF_BOUND);
        return ERROR;
    }
    if( row > theBoard->_numOfRows || col > theBoard->_numOfCols)
    {
        return EMPTY_SQUARE;
    }
    switch(theBoard->ptrBoardArr[row][col])
    {
        case (PLAYER1):
            return PLAYER1;
        case (PLAYER2):
            return PLAYER2;
        default:
            return EMPTY_SQUARE;
    }
}

/**
 * @brief tries to put the given char at [row][col].
 * @param theBoard the board
 * @param row the x coordinate
 * @param col the y coordinate
 * @param val the char
 * @return true\false
 */
bool putBoardSquare(BoardP theBoard, int row, int col, char val)
{
    assert(theBoard != NULL);
    if(isBadVal(val))
    {
        reportError(BAD_VAL);
        return false;
    }
    if(isBadRowCol(row) || isBadRowCol(col))
    {
        reportError(OUT_OF_BOUND);
        return false;
    }
    if(!isYourTurn(theBoard, val))
    {
        reportError(WRONG_TURN);
        return false;
    }
    if(row > theBoard->_numOfRows - 1 || col > theBoard->_numOfCols - 1)
    {
        BoardP temp = resizeBoard(theBoard, row, col);
        if(temp != NULL)
        {
            freeBoard(theBoard);
            theBoard = duplicateBoard(temp);
            freeBoard(temp);

            return true;
        }
        else
        {
            // mem alloc failed
            return false;
        }
    }
    if(theBoard->ptrBoardArr[row][col] != EMPTY_SQUARE)
    {
        reportError(SQUARE_FULL);
        return false;
    }
    return true;
}

/**
 * @brief tries to remove char located at [row][col].
 * @param theBoard the board
 * @param row the x coordinate
 * @param col the y coordinate
 * @return true\false
 */
bool cancelMove(BoardP theBoard, int x, int y)
{
    assert(theBoard != NULL);
    if(isBadRowCol(x) || isBadRowCol(y))
    {
        reportError(OUT_OF_BOUND);
        return false;
    }
    if(theBoard->ptrBoardArr[x][y] == EMPTY_SQUARE ||
       theBoard->ptrBoardArr[x][y] == theBoard->_whosTurn)
    {
        reportError(ILLEGAL_CANCELLATION);
        return false;
    }
    if(x > theBoard->_numOfRows || y > theBoard->_numOfCols)
    {
        return false;
    }
    return true;
}

/**
 * @brief function that tries to find sequence that will allow a player to win.
 * @param theBoard the board
 * @param startingXIndex the x coordinate that the search will begin from
 * @param endXIndex the x coordinate that the search will end at
 * @param startingYIndex the y coordinate that the search will begin from
 * @param endYIndex the y coordinate that the search will end at
 * @param val the char of the sequence
 * @return true\false
 */
static bool checkSequence(ConstBoardP board, int startingXIndex, int endXIndex,
                          int startingYIndex, int endYIndex, char val)
{
    int count = 0;
    int x = startingXIndex;
    int y = startingYIndex;
    while(x < endXIndex || y < endYIndex)
    {
        if(board->ptrBoardArr[x][y] == val)
        {
            count ++;
        }
        else
        {
            count = 0;
        }
        if(count == AMOUNT_TO_WIN)
        {
            return true;
        }
        if(x != endXIndex)
        {
            x++;
        }
        if(y != endYIndex)
        {
            y++;
        }

    }
    return false;
}

/**
 * @brief tries to find a winner.
 * @param theBoard the board
 * @return true\false
 */
char getWinner(ConstBoardP board)
{
    assert(board != NULL);
    int x = board->_lastTurnRow;
    int y = board->_lastTurnCol;
    int minXCoordinate = x - AMOUNT_TO_WIN;
    int maxXCoordinate = x + AMOUNT_TO_WIN;
    int minYCoordinate = y - AMOUNT_TO_WIN;
    int maxYCoordinate = y + AMOUNT_TO_WIN;
    if(minXCoordinate < 0)
    {
        minXCoordinate = 0;
    }
    if(minYCoordinate < 0)
    {
        minYCoordinate = 0;
    }
    if(maxXCoordinate  > board->_numOfRows)
    {
        maxXCoordinate = board->_numOfRows - 1;
    }
    if(maxYCoordinate  > board->_numOfCols)
    {
        minYCoordinate = board->_numOfCols - 1;
    }
    if(checkSequence(board, minXCoordinate, maxXCoordinate, y, y, board->_whosTurn) ||
       checkSequence(board, x, x, minYCoordinate, maxYCoordinate, board->_whosTurn) ||
       checkSequence(board, minXCoordinate, maxXCoordinate,
       minYCoordinate, maxYCoordinate, board->_whosTurn))
    {

        return board->_whosTurn;
    }
    return EMPTY_SQUARE;
}

/**
 * @brief frees all the memory allocated to a BoardP struct.
 * @param theBoard the board
 */
void freeBoard(BoardP board)
{
    if(board != NULL)
    {
        for (int i = 0; i < board->_numOfRows; i++)
        {
            if(board->ptrBoardArr[i] != NULL)
            {
                free(board->ptrBoardArr[i]);
            }
        }
        if(board->ptrBoardArr != NULL)
        {
            free(board->ptrBoardArr);
        }
        free(board);
    }

}


/**
 * @brief function that prints the board.
 * @param stream the stream in which the output will be given
 * @param board the board
 * @param row the x coordinate from which the print starts
 * @param col the y coordinate from which the print starts
 */
void printBoard(FILE * stream, ConstBoardP board, int row, int col)
{
    assert(board != NULL);
    assert(stream != NULL);
    if(isBadRowCol(row) || isBadRowCol(col))
    {
        reportError(OUT_OF_BOUND);
        return;
    }
    fprintf(stream, "   ");
    for (int i = 0; i < DEFAULT_PRINT; i++)
    {
        fprintf(stream, "+%d ", i);
    }
    fprintf(stream, "\n");
    for (int i = row; i < row + DEFAULT_PRINT; i++)
    {
        fprintf(stream, "+%d ", i - row);
        for (int j = col; j < col + DEFAULT_PRINT; j++)
        {
            if(i < board->_numOfRows && j < board->_numOfCols)
            {
                fprintf(stream, " %c ", board->ptrBoardArr[i][j]);
            }
        }
        fprintf(stream, "\n");
    }
}

/**
 * @brief function that gets the amount of mem allocated to a board struct.
 * @param board the board
 * @return amount in bytes
 */
int getAllocatedSize(ConstBoardP board)
{
    assert(board != NULL);
    int size = 0;
    size += sizeof(board->_numOfCols);
    size += sizeof(board->_numOfRows);
    size += sizeof(board->_curRow);
    size += sizeof(board->_curCol);
    size += sizeof(board->_lastTurnRow);
    size += sizeof(board->_lastTurnCol);
    size += sizeof(board->_whosTurn);
    size += sizeof(board->ptrBoardArr);
    for (int i = 0; i < board->_numOfRows; i++)
    {
        for (int j = 0; j < board->_numOfCols; j++)
        {
            size += sizeof(board->ptrBoardArr[i][j]);
        }
        size += sizeof(board->ptrBoardArr[i]);
    }
    size += sizeof(board);
    return size;
}
