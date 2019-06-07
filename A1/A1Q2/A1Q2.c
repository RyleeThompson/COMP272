/*
 ============================================================================
 Name        : A1Q2.c
 Author      : Rylee Thompson
 Version     :
 Copyright   : 
 Description : Solution to COMP272 Assignment 1 question 2: Write a method to swap
 adjacent nodes in both a singly and doubly linked list.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SingleLinkedListNode{
	int data;
	struct SingleLinkedListNode *next;
}SLLN;

typedef struct DoubleLinkedListNode{
	int data;
	struct DoubleLinkedListNode *next;
	struct DoubleLinkedListNode *previous;
}DLLN;

typedef struct LinkedList{
	int size;
	void *front;
	void *rear;
}LL;

LL initializeList();
void add(int data, LL *list, char listType[]);
void addDLL(int data, LL *list);
void addSLL(int data, LL *list);
void printDLL(LL DLL);
void printSLL(LL SLL);
void swapDLL(int index1, int index2, LL *DLL);
void swapSLL(int index1, int index2, LL *DLL);

int main(void) {
	LL DLL; //Doubly linked list
	LL SLL; //Singly linked list

	int i;
	DLL = initializeList();
	SLL = initializeList();
	
	//Add elements to the list
	for(i = 0; i < 6; i++)
	{
		add(i, &DLL, "doubly");
		add(i, &SLL, "singly");
	}
	printf("The doubly linked list is: ");
	printDLL(DLL);
	fflush(stdout);

	printf("\nSwapping elements in the doubly linked list");
	for(i = 0; i < DLL.size - 1; i++)
	{
		printf("Swapping nodes %d and %d (indexing starts at zero).\n", i, i + 1);
		fflush(stdout);
		swapDLL(i, i + 1, &DLL);
		printf("The new list is: ");
		printDLL(DLL);
	}

	printf("\nThe singly linked list is: ");
	printSLL(SLL);

	printf("\nSwapping elements in the singly linked list\n");
	for(i = 0; i < SLL.size - 1; i++)
	{
		printf("Swapping nodes %d and %d (indexing starts at zero).\n", i, i + 1);
		swapSLL(i, i + 1, &SLL);
		printf("The new list is: ");
		printSLL(SLL);
	}
}
/*
 * Function: initializeList
 * --------------------------------
 * Summary: Initializes a list to initial values (null, 0) and returns it
 * Parameters: None
 * Returns: The initialized list
*/
LL initializeList(void)
{
	LL init;
	init.size = 0;
	init.front = NULL;
	init.rear = NULL;
	return init;
}

/*
 * Function: add
 * -----------------------------------
 * Summary: Calls addDLL or addSLL based on the string parameters. 
 * Parameters: The data to be added to the list, a pointer to the list itself,
 * and a string ("doubly" or "singly")
 * Returns: None
 * Note: This function only exists because I wanted to play around with function pointers
*/
void add(int data, LL *list, char listType[])
{
	void (*addPtr[])(int, LL*) = {addDLL, addSLL};
	addPtr[strcmp(listType, "singly") == 0](data, list);
}

/*
 * Function: addDLL
 * ----------------------------------------
 * Summary: Adds the given data to the given doubly linked list
 * Parameters: The data to be added and a pointer to the list.
 * Returns: Nothing
*/
void addDLL(int data, LL *list)
{
	DLLN *tmp;
	DLLN *oldRear;
	tmp = malloc(sizeof(DLLN));
	list->size += 1;
	tmp->data = data;
	oldRear = (DLLN*)list->rear;
	if(list->front == NULL) //If the list was empty
	{
		list->front = tmp;
	}
	//New node becomes the rear
	else
		oldRear->next = tmp;
	tmp->next = NULL;
	tmp->previous = list->rear;
	list->rear = tmp;
}

/*
 * Function: addSLL
 * --------------------------------------
 * Summary: Adds the given data to the given singly linked list
 * Parameters: The data to be added and a pointer to the list
 * Returns: Nothing
*/
void addSLL(int data, LL *list)
{
	SLLN *tmp;
	SLLN *oldRear;
	tmp = malloc(sizeof(SLLN));
	list->size += 1;
	tmp->data = data;
	oldRear = (SLLN*)list->rear;
	if(list->front == NULL) //If the list was empty
		list->front = tmp;
	//New node becomes the rear
	else
		oldRear->next = tmp;
	tmp->next = NULL;
	list->rear = tmp;
}

/*
 * Function: printDLL
 * ----------------------------------------
 * Summary: Prints the given doubly linked list to stdout
 * Parameters: The doubly linked list to be printed
 * Returns: Nothing
*/
void printDLL(LL DLL)
{
	DLLN *currentNodeDLLN;
	int i;
	for(currentNodeDLLN = (DLLN*)DLL.front, i = 0; currentNodeDLLN != NULL && i < 15; currentNodeDLLN = currentNodeDLLN->next, i++)
			printf("%d ", currentNodeDLLN->data);
	printf("\n");
}

/*
 * Function: printSLL
 * ----------------------------------------------
 * Summary: Prints the given singly linked list to stdout
 * Parameters: The singly linked list to be printed
 * Returns: Nothing
*/
void printSLL(LL SLL)
{
	SLLN *currentNodeSLLN;
	int i;
	for(i = 0, currentNodeSLLN = (SLLN*)SLL.front; currentNodeSLLN != NULL && i < 15; currentNodeSLLN = currentNodeSLLN->next, i++)
		printf("%d ", currentNodeSLLN->data);
	printf("\n");
}

/*
 * Function: swapDLL
 * --------------------------------------
 * Summary: Swaps the adjacent nodes at the given indices in a doubly linked list
 * Parameters: The indices to be swapped, and a pointer to the list
 * Returns: Nothing
*/
void swapDLL(int index1, int index2, LL *DLL)
{
	int i;
	DLLN *currentNode;
	DLLN *node1 = NULL;
	DLLN *node2 = NULL;
	DLLN *temp;
	int tempIndex;

	//Simply ensures that index1 is the greater of the 2 indices (closer to rear)
	tempIndex = index1;
	index1 = (index1 > index2) ? index1 : index2;
	index2 = (index2 > tempIndex) ? tempIndex : index2;

	if(index1 <= DLL->size && index2 <= DLL->size) //Make sure the indices are in bounds
	{
		//Find the node at the given indices
		for(i = 0, currentNode = (DLLN*)DLL->front; (node1 == NULL || node2 == NULL) && i <= DLL->size; i++, currentNode = currentNode->next)
		{
			node1 = (i == index1) ? currentNode : node1;
			node2 = (i == index2) ? currentNode : node2;
		}

		//Update front and rear if necessary
		DLL->front = (node2 == DLL->front) ? node1 : DLL->front;
		DLL->rear = (node1 == DLL->rear) ? node2: DLL->rear;

		//Update the nodes adjacent to the ones being swapped
		if(node1->next != NULL)
			node1->next->previous = node2;
		if(node2->previous != NULL)
			node2->previous->next = node1;

		//Swap the nodes pointers
		temp = node1->previous;
		node1->previous = node2->previous;
		node2->previous = node1;

		temp = node1->next;
		node1->next = node2;
		node2->next = temp;
	}
	else
		printf("Index out of bounds\n");
}

/*
 * Function: swapSLL
 * -----------------------------------------
 * Summary: Swaps the adjacentnodes at the given indices in a singly linked list
 * Parameters: The indices to be swapped, and a pointer to the list
 * Returns: Nothing
*/
void swapSLL(int index1, int index2, LL *SLL)
{
	int i;
	SLLN *currentNode;
	SLLN *node1 = NULL;
	SLLN *node2 = NULL;
	SLLN *node2Previous = NULL;
	SLLN *temp;
	int tempIndex;

	//Simply ensures that index1 is the greater of the 2 indices (closer to rear)
	tempIndex = index1;
	index1 = (index1 > index2) ? index1 : index2;
	index2 = (index2 > tempIndex) ? tempIndex : index2;

	if(index1 <= SLL->size && index2 <= SLL->size) //Ensures indices are within bounds
	{
		//Find the node at each indices
		for(i = 0, currentNode = (SLLN*)SLL->front; (node1 == NULL || node2 == NULL) && i <= SLL->size; i++, currentNode = currentNode->next)
		{
			node1 = (i == index1) ? currentNode : node1;
			node2 = (i == index2) ? currentNode : node2;
			node2Previous = (i == index2 - 1) ? currentNode : node2Previous;
		}

		//Update front and rear if necessary
		SLL->front = (node2 == SLL->front) ? node1 : SLL->front;
		SLL->rear = (node1 == SLL->rear) ? node2: SLL->rear;

		//Update the node pointers to swap positions
		if(node2Previous != NULL)
			node2Previous->next = node1;

		temp = node1->next;
		node1->next = node2;
		node2->next = temp;
	}
	else
		printf("Index out of bounds\n");
}
