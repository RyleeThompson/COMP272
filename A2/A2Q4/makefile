CC=gcc
CFLAGS=-Wall -std=c99

htable : A2Q4.o
	$(CC) -o htable A2Q4.o -lm
A2Q4.o : A2Q4.c
	$(CC) $(CFLAGS) -c A2Q4.c

clean:
	rm htable A2Q4.o

