CC=gcc
CFLAGS=-Wall -std=c99

reverse : A1Q5.o
	$(CC) -o reverse A1Q5.o -lm
A1Q5.o : A1Q5.c
	$(CC) $(CFLAGS) -c A1Q5.c

clean:
	rm reverse A1Q5.o

