CC = gcc
CFLAGS = -Wall -Wextra

all: tetris

tetris: main.o tetromino.o board.o input.o
	$(CC) $(CFLAGS) -o tetris main.o tetromino.o board.o input.o

main.o: main.c tetromino.h board.h input.h
	$(CC) $(CFLAGS) -c main.c

tetromino.o: tetromino.c tetromino.h input.h
	$(CC) $(CFLAGS) -c tetromino.c

board.o: board.c board.h tetromino.h
	$(CC) $(CFLAGS) -c board.c

input.o: input.c input.h
	$(CC) $(CFLAGS) -c input.c

clean:
	rm -f *.o tetris
