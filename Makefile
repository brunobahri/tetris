CC = gcc
CFLAGS = -Wall -Wextra

all: tetris

tetris: main.o tetromino.o board.o
	$(CC) $(CFLAGS) -o tetris main.o tetromino.o board.o

main.o: main.c tetromino.h board.h
	$(CC) $(CFLAGS) -c main.c

tetromino.o: tetromino.c tetromino.h
	$(CC) $(CFLAGS) -c tetromino.c

board.o: board.c board.h tetromino.h
	$(CC) $(CFLAGS) -c board.c

clean:
	rm -f *.o tetris
