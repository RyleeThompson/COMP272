CC=gcc
CFLAGS=-Wall -std=c99

btree : A2Q5.o
	$(CC) -o btree A2Q5.o -lm
A2Q5.o : A2Q5.c
	$(CC) $(CFLAGS) -c A2Q5.c

clean:
	rm btree A2Q5.o

