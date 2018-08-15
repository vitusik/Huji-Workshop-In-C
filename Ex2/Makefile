CC=gcc

CFLAGS=-Wextra -Wall -Wvla -std=c99 

PlayBoard: PlayBoard.o Board.o ErrorHandle.o 
	$(CC) $(CFLAGS) PlayBoard.c Board.c ErrorHandle.c
	./PlayBoard

PlayBoard.o: PlayBoard.c Board.c  ErrorHandle.c 
	$(CC) $(CFLAGS) PlayBoard.c Board.c ErrorHandle.c

Gomoku: Gomoku.o Board.o ErrorHandle.o 
	$(CC) $(CFLAGS) Gomoku.c Board.c ErrorHandle.c -o Gomoku 
	./Gomoku

Gomoku.o: Gomoku.c Board.c ErrorHandle.c
	$(CC) $(CFLAGS) Gomoku.c Board.c ErrorHandle.c 

Board.o: Board.c Board.h ErrorHandle.c ErrorHandle.h
	$(CC) -c $(CFLAGS) Board.c ErrorHandle.c

ErrorHandle.o: ErrorHandle.c ErrorHandle.h 
	$(CC) -c $(CFLAGS) ErrorHandle.c 


clean:
	rm Board.o ErrorHanbdle.o Gomoku.o 


