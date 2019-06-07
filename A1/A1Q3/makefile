CC=gcc
CFLAGS=-Wall -std=c99

bag : A1Q3.o
	$(CC) -o bag A1Q3.o -lm
A1Q3.o : A1Q3.c
	$(CC) $(CFLAGS) -c A1Q3.c

clean:
	rm bag A1Q3.o

