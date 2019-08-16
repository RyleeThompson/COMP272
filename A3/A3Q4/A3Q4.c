/*
 ============================================================================
 Name        : A3Q4.c
 Author      : Rylee Thompson
 Version     :
 Copyright   :
 Description : Solution to Assignment 3 question 4: 
 Given sequence 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, sort the sequence using
 merge sort, and illustrate the details of the execution of the 
 algorithm.
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mergeSort(int arr[], int lower, int higher);
void merge(int arr[], int lower, int middle, int higher);
void printSet(int set[], int setSize);
#define maxNumElements 15

int main(void)
{
	int testSet[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
	srand(time(NULL));
	int numElements = (rand() % maxNumElements) + 1;
	int randomSet[numElements];

	//Create a random set to test as well
	for(int i = 0; i < numElements; i++)
		randomSet[i] = rand() % (numElements + 5);

	printf("sorting following set using merge sort: \n");
	printSet(testSet, sizeof(testSet)/sizeof(int));
	mergeSort(testSet, 0, sizeof(testSet)/sizeof(int) - 1);
	printf("sorted: \n");
	printSet(testSet, sizeof(testSet)/sizeof(int));

	printf("sorting following set using merge sort: \n");
	printSet(randomSet, numElements);
	mergeSort(randomSet, 0, numElements - 1);
	printf("sorted: \n");
	printSet(randomSet, numElements);

	return 0;
}

/*
 * Function: mergeSort
 * -------------------------------------------------------
 * Summary: Sorts the given array using merge sort.
 * Parameters: A pointer to the array to be sorted and its
 * lowest and highest indices.
 * Returns: Nothing
 * Notes: Modifies array in place.
*/
void mergeSort(int arr[], int lower, int higher)
{
	if(lower < higher)
	{
		int middle = (lower + higher)/2;
		mergeSort(arr, lower, middle);
		mergeSort(arr, middle + 1, higher);
		merge(arr, lower, middle, higher);
	}
}

void merge(int arr[], int lower, int middle, int higher)
{
	int copy[higher - lower + 1];
	int i = lower;
	int j = middle + 1;
	int index = 0;

	//Adds elements in ascending order to copy
	while(i <= middle && j <= higher)
		copy[index++] = arr[i] < arr[j] ? arr[i++] : arr[j++];
	
	//Finish adding elements if j condition broke above loop
	while(i <= middle)
		copy[index++] = arr[i++];
	
	//Finish adding elements if i condition broke first loop
	while(j <= higher)
		copy[index++] = arr[j++];

	//Copy sorted array into original
	for(index = 0, i = lower; i <= higher; index++, i++)
		arr[i] = copy[index];
}

/*
 * Function: printSet
 * -------------------------------------------------
 * Summary: Prints the given set to stdout.
 * Parameters: The set to be printed and the number of 
 * elements in the set.
 * Returns: Nothing
*/
void printSet(int set[], int setSize)
{
	for(int i = 0; i < setSize; i++)
		printf("%d ", set[i]);
	putchar('\n');
}
