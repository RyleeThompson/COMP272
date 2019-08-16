CC=gcc
CFLAGS=-Wall -std=c99

check : A3Q3.o
	$(CC) -o check A3Q3.o -lm
A2Q3.o : A3Q3.c
	$(CC) $(CFLAGS) -c A3Q3.c

clean:
	rm check A3Q3.o

