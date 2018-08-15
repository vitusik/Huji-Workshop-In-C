CC=gcc

CFLAGS=-Wextra -Wall -Wvla -std=c99 

main: Complex.o MyGroupMain.o libgenGroup.a GenGroup.o
	$(CC) $(CFLAGS) Complex.c MyGroupMain.c -L. -lgenGroup -o MyGroupMain
	

MyGroupMain: Complex.o MyGroupMain.o libgenGroup.a GenGroup.o
	$(CC) $(CFLAGS) Complex.c MyGroupMain.c -L. -lgenGroup -o MyGroupMain
	./MyGroupMain
	
GenGroup: GenGroup.o
	ar rcs libgenGroup.a GenGroup.o

GenGroup.o: GenGroup.c GenGroup.h
	$(CC) $(CFLAGS) -c GenGroup.c -o GenGroup.o
		
Complex.o: Complex.c Complex.h
	$(CC) $(CFLAGS) -c Complex.c


clean:
	rm Complex.o MyGroupMain.o GenGroup.o libgenGroup.a


