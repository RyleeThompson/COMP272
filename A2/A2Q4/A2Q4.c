/*
 ============================================================================
 Name        : A2Q4.c
 Author      : Rylee Thompson
 Version     :
 Copyright   :
 Description : Solution to Assignment 2 question 4: Implement a commonly used 
 hash table in a program that handles collision using linear probing. Using 
 (K mod 13) as the hash function, store the following elements in the table: 
 {1, 5, 21, 26, 39, 14, 15, 16, 17, 18, 19, 20, 111, 145, 146}.
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#define hTableSize 30

void addElement(int *hTable, int key, int (hash)(int key, int i));
int hashFunction(int key, int i);
int findElement(int *hTable, int key, int (hash)(int key, int i));

int main(void){
    int *hTable = malloc(sizeof(int) * hTableSize);
    int elements[] = {1, 5, 21, 26, 39, 14, 15, 16, 17, 18, 19, 20, 111, 145, 146};
    int i;
    int numElements = sizeof(elements)/sizeof(int);

    memset(hTable, -1, sizeof(int) * hTableSize);

    for(i = 0; i < numElements; i++)
        addElement(hTable, elements[i], hashFunction);

    for(i = 0; i < numElements; i++)
        printf("The key %d was found at index %d\n", elements[i], findElement(hTable, elements[i], hashFunction));

    free(hTable);

}
/*
 * Function: addElement
 * --------------------------------------------
 * Summary: Adds the given key to the given pointer to a hash table
 * using the given hash function.
 * Parameters: A pointer to the hash table, the key to be added, and a pointer
 * to the hash function to be used.
 * Returns: Nothing
 */
void addElement(int *hTable, int key, int (hash)(int key, int i))
{
    int i = 0;
    int val;
    val = hash(key, i);
    for(;hTable[val] != -1; i++, val = hash(key, i))
        ;
    hTable[val] = key;
}

/*
 * Function: hashFunction
 * ----------------------------------------------
 * Summary: Hashes the given key using k mod 13 has the hash
 * function and supports linear probing.
 * Parameters: The key to be hashed and the number of iterations.
 * Returns: The resulting hashed value.
*/
int hashFunction(int key, int i)
{
    return ((key % 13) + i) % hTableSize;
}

/*
 * Function: findElement
 * ------------------------------------------
 * Summary: Finds the index of the given key in the hash table (using the
 * given hash function) and returns it.
 * Parameters: A pointer to the hash table, the key to be found, and a pointer
 * to the hash function to be used.
 * Returns: The index the key is found at.
*/
int findElement(int *hTable, int key, int (hash)(int key, int i))
{
    int i = 0;
    int val;
    val = hash(key, i);
    for(;hTable[val] != key; i++, val = hash(key, i))
        ;
    return val;
}
