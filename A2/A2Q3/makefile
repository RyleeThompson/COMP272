CC=gcc
CFLAGS=-Wall -std=c99

sgoat : A2Q3.o
	$(CC) -o sgoat A2Q3.o -lm -g
A2Q3.o : A2Q3.c
	$(CC) $(CFLAGS) -c A2Q3.c

clean:
	rm sgoat A2Q3.o

