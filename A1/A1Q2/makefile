CC=gcc
CFLAGS=-Wall -std=c99

swap : A1Q2.o
	$(CC) -o swap A1Q2.o -lm
A1Q2.o : A1Q2.c
	$(CC) $(CFLAGS) -c A1Q2.c

clean:
	rm swap A1Q2.o

