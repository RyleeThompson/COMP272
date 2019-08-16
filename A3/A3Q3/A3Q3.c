/*
 ============================================================================
 Name        : A3Q3.c
 Author      : Rylee Thompson
 Version     :
 Copyright   :
 Description : Solution to Assignment 3 question 3:  Suppose you are given 
 two sequences S1 and S2 of n elements, possibly containing duplicates, on 
 which a total order relation is defined. Describe an efficient algorithm for
 determining if S1 and S2 contain the same set of elements (10 marks). 
 ============================================================================
 */
 /*
  My solution adds the first set to a hash table, then searches the hash table
  for the second set.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void printSet(int set[]);
int hashFunction(int key, int i);
void addElement(int *htable, int key, int (hash)(int key, int i));
int findElement(int *htable, int key, int (hash)(int key, int i));
int findSetInHtable(int *htable, int set[], int (hash)(int key, int i));

#define numElements 5
#define hTableSize (numElements * 3)
#define maxVal 100
#define countHashes 0 //set to 1 if you want to see number of iterations
int main(void)
{
	int *htable = malloc(sizeof(int) * hTableSize);
	int set1[numElements];
	int set2[numElements];
	srand(time(NULL));
	//Create two random sets
	for(int i = 0; i < numElements; i++)
	{
		set1[i] = rand() % numElements;
		set2[i] = rand() % numElements;
	}

	printf("Set 1 is: \n");
	printSet(set1);
	printf("\nSet 2 is: \n");
	printSet(set2);

	memset(htable, -1, sizeof(int) * hTableSize);
	
	//Add first set to hash table
	for(int i = 0; i < numElements; i++)
		addElement(htable, set1[i], hashFunction);

	if(findSetInHtable(htable, set2, hashFunction))
		printf("\nSet 1 contains set 2.\n");
	else
		printf("\nSet 1 does not contain set 2.\n");

	return 0;
}

/*
 * Function: addElement
 * -----------------------------------------------------------------------
 * Summary: Hashes the key using the function pointer given and adds it to
 * the given hash table.
 * Parameters: A pointer to the hash table, the key to be added, and a
 * a function pointer to the hash function to be used.
 * Returns: Nothing
*/
void addElement(int *hTable, int key, int (hash)(int key, int i))
{
	int i = 0;
	int val;
	val = hash(key, i);
	for(; hTable[val] != -1; i++, val = hash(key, i))
		;
	hTable[val] = key;
}

/*
 * Function: hashFunction
 * ------------------------------------------------------
 * Summary: The hash function used for this program. Uses
 * key % 17 as the hash function, and deals with collisions
 * using linear probing.
 * Parameters: The key to be hashed, and the number of probes
 * for the given key.
 * Returns: The hashed value.
*/ 
int hashFunction(int key, int i)
{
	#if countHashes
	static int counter = 0;
	printf("Number of hashes: %d\n", ++counter);
	#endif
	return ((key % 17) + i) % hTableSize;
}

/*
 * Function: findSetInHtable
 * ---------------------------------------------------------------
 * Summary: Determines if the given set is in the given hash table
 * Parameters: A pointer to the hash table, the set, and the hash function
 * to be used (should be same function used in addElement)
 * Returns: 1 if the set is found in the hash table else 0.
*/
int findSetInHtable(int *htable, int set[], int (hash)(int key, int i))
{
	int i;
	for(i = 0; i < numElements && findElement(htable, set[i], hash) != 0; i++)
		;
	return i >= numElements;
}

/*
 * Function: findElement
 * ----------------------------------------------------------------
 * Summary: Finds the given key in the given hash table, where the key
 * is hashed using the given hash function.
 * Parameters: A pointer to the hash table, the key, and a pointer to the 
 * hash function. 
 * Returns: 1 if the key is found else 0.
*/
int findElement(int *htable, int key, int (hash)(int key, int i))
{
	int i = 0;
	int val;
	val = hash(key, i);
	for(;htable[val] != key && htable[val] != -1 && i < hTableSize; i++, val = hash(key, i))
		;
	if(htable[val] == key)
		return 1;
	return 0;
}

/*
 * Function: printSet
 * ---------------------------------------------------
 * Summary: Prints the given set to stdout
 * Parameters: The set to be printed
 * Returns: Nothing.
*/
void printSet(int set[])
{
	int i;
	for(i = 0; i < numElements; i++)
		printf("%d ", set[i]);
	putchar('\n');
}
