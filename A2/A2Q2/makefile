CC=gcc
CFLAGS=-Wall -std=c99

bst : A2Q2.o
	$(CC) -o bst A2Q2.o -lm
A2Q2.o : A2Q2.c
	$(CC) $(CFLAGS) -c A2Q2.c

clean:
	rm bst A2Q2.o

