CC=gcc
CFLAGS=-Wall -std=c99

sort : A3Q4.o
	$(CC) -o sort A3Q4.o -lm
A2Q4.o : A3Q4.c
	$(CC) $(CFLAGS) -c A3Q4.c

clean:
	rm sort A3Q4.o

