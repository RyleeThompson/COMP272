CC=gcc
CFLAGS=-Wall -std=c99

sort : A3Q4qs.o
	$(CC) -o sort A3Q4qs.o -lm
A3Q4qs.o : A3Q4qs.c
	$(CC) $(CFLAGS) -c A3Q4qs.c

clean:
	rm sort A3Q4qs.o

