CC=gcc
CFLAGS=-Wall -std=c99

pqueue : A1Q1a.o
	$(CC) -o pqueue A1Q1a.o -lm
A1Q1a.o : A1Q1a.c
	$(CC) $(CFLAGS) -c A1Q1a.c

clean:
	rm pqueue A1Q1a.o

