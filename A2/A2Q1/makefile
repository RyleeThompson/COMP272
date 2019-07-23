CC=gcc
CFLAGS=-Wall -std=c99

btree : A2Q1.o
	$(CC) -o btree A2Q1.o -lm
A2Q1.o : A2Q1.c
	$(CC) $(CFLAGS) -c A2Q1.c

clean:
	rm btree A2Q1.o

