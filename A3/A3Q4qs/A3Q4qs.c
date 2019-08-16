/*
 ============================================================================
 Name        : A3Q4qs.c
 Author      : Rylee Thompson
 Version     :
 Copyright   :
 Description : Solution to Assignment 3 question 4: 
 Given sequence 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, sort the sequence using
 quick sort, and illustrate the details of the execution of the 
 algorithm.
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void quickSort(int arr[], int lower, int higher);
int partition(int arr[], int lower, int higher);
void printSet(int set[], int setSize);
void swapElements(int arr[], int ix1, int ix2);
#define maxNumElements 15

int main(void)
{
	int testSet[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
	srand(time(NULL));
	int numElements = (rand() % maxNumElements) + 1;
	int randomSet[numElements];

	//Make a random set to test as well
	for(int i = 0; i < numElements; i++)
		randomSet[i] = rand() % (numElements + 5);

	printf("sorting following set using quick sort: \n");
	printSet(testSet, sizeof(testSet)/sizeof(int));
	quickSort(testSet, 0, sizeof(testSet)/sizeof(int) - 1);
	printf("sorted: \n");
	printSet(testSet, sizeof(testSet)/sizeof(int));

	printf("sorting following set using quick sort: \n");
	printSet(randomSet, numElements);
	quickSort(randomSet, 0, numElements - 1);
	printf("sorted: \n");
	printSet(randomSet, numElements);

	return 0;
}
/*
 * Function: quickSort
 * ------------------------------------------------------
 * Summary: Uses quick sort to sort the given array.
 * Parameters: A pointer to the array to be sorted, as well
 * as the lowest and highest indices of the array.
 * Returns: Nothing
*/
void quickSort(int arr[], int lower, int higher)
{
	if(lower < higher)
	{
		int loc = partition(arr, lower, higher);
		quickSort(arr, lower, loc);
		quickSort(arr, loc + 1, higher);
	}
}
/*
 * Function: partition
 * -------------------------------------------------
 * Summary: Partitions the array for use in quick sort algorithm.
 * Chooses the lowest indices as the pivot. 
 * Parameters: A pointer to the array, and its lowest and highest
 * indices. 
 * Returns: The final index of the pivot.
*/
int partition(int arr[], int lower, int higher)
{
	int loc = lower;
	int left = lower;
	int right = higher;

	while(!(left == loc && left == right))
	{
		while(arr[loc] <= arr[right] && right > left)
			right--;
		swapElements(arr, right, loc);
		loc = right;

		while(arr[loc] >= arr[left] && left < right)
			left++;
		swapElements(arr, left, loc);
		loc = left;
	}
	return loc;
}

/*
 * Function: swapElements
 * -----------------------------------------------------
 * Summary: Swaps the two elements at the given indices in
 * the array.
 * Parameters: A pointer to the array and indices to be swapped.
 * Returns: Nothing
 * Notes: Modifies the array in place.
*/
void swapElements(int arr[], int ix1, int ix2)
{
	int temp = arr[ix1];
	arr[ix1] = arr[ix2];
	arr[ix2] = temp;
}

/*
 * Function: printSet
 * -----------------------------------------
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
