CC=gcc
CFLAGS=-Wall -std=c99

stack : A1Q1b.o
	$(CC) -o stack A1Q1b.o -lm
A1Q1b.o : A1Q1b.c
	$(CC) $(CFLAGS) -c A1Q1b.c

clean:
	rm stack A1Q1b.o

