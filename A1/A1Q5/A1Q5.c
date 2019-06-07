/*
 ============================================================================
 Name        : A1Q2.c
 Author      : Rylee Thompson
 Version     :
 Copyright   :
 Description : Solution to COMP 272 Assignment 1 Q5: Write a method to reverse
 a doubly linked list.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DoubleLinkedListNode{
	int data;
	struct DoubleLinkedListNode *next;
	struct DoubleLinkedListNode *previous;
}DLLN;

typedef struct LinkedList{
	int size;
	DLLN *front;
	DLLN *rear;
}LL;

LL initializeList();
void addDLL(int data, LL *list);
void printDLL(LL DLL);
void reverse(DLLN *node1, DLLN *node2, LL *DLL);
void freeDLL(DLLN *currentNode);

#define debug 0

int main(void) {
	LL DLL;

	int i;
	DLL = initializeList();
	for(i = 0; i < 10; i++)
		addDLL(i, &DLL);
	printf("The doubly linked list is: ");
	printDLL(DLL);

	reverse(DLL.rear, DLL.front, &DLL);
	printf("The reversed list is: ");
	printDLL(DLL);
	freeDLL(DLL.front);
}

/*
 * Function: initializeList
 * Summary: Creates a new list and initalizes it to default values (0, NULL)
 * Parameters: None
 * Returns: The new list
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
 * Function: addDLL
 * Summary: Adds a node with the given data to the given list
 * Parameters: The data to be added (int), and a pointer to the list it is to be added to
 * Returns: None
*/
void addDLL(int data, LL *list)
{
	DLLN *tmp;
	DLLN *oldRear;
	tmp = malloc(sizeof(DLLN));
	list->size += 1;
	tmp->data = data;
	oldRear = (DLLN*)list->rear;
	if(list->front == NULL) //If the list is empty
	{
		list->front = tmp;
	}
	//Update the rear
	else
		oldRear->next = tmp;
	tmp->next = NULL;
	tmp->previous = list->rear;
	list->rear = tmp;
}

/*
 * Function: printDLL
 * Summary: Prints the given doubly-linked list to stdout.
 * Parameters: The DLL to be printed
 * Returns: None
*/
void printDLL(LL DLL)
{
	DLLN *currentNodeDLLN;
	for(currentNodeDLLN = (DLLN*)DLL.front; currentNodeDLLN != NULL; currentNodeDLLN = currentNodeDLLN->next)
	{
#if debug == 0
		printf("%d ", currentNodeDLLN->data);
#endif
#if debug == 1
		printf("current node: %d\n", currentNodeDLLN->data);
		if(currentNodeDLLN->previous != NULL)
			printf("previous: %d\n", currentNodeDLLN->previous->data);
		else
			printf("previous: NULL\n");
		if(currentNodeDLLN->next != NULL)
			printf("next: %d\n", currentNodeDLLN->next->data);
		else
			printf("next: NULL\n");
#endif
	}
	printf("\n");
}

/*
 * Function: reverse
 * Summary: Recursively reverses the given list starting at one given node (node2) and
 * ending at the other given node (node1). Swaps the two given nodes, and then moves in towards 
 * the middle until the nodes cross/meet.
 * Parameters: A pointer to two nodes in a doubly linked list and the list itself.
 * Returns: Nothing.
*/
void reverse(DLLN *node1, DLLN *node2, LL *DLL)
{
	DLLN *temp;
	DLLN *nextNode1;
	DLLN *nextNode2;
#if debug == 1
	printf("node1: %d\n", node1->data);
	printf("node2: %d\n", node2->data);
	printf("list before swapping these nodes: ");
	printDLL(*DLL);
#endif
	if(node1 != node2) //Stop case for lists with odd number of elements
	{
		//Update front and rear
		DLL->front = (node2 == DLL->front) ? node1 : DLL->front;
		DLL->rear = (node1 == DLL->rear) ? node2 : DLL->rear;

		//Variables for the next recursive call
		nextNode1 = node1->previous;
		nextNode2 = node2->next;

		//Update the nodes adjacent to the nodes to be swapped
		if(node1->next != NULL)
			node1->next->previous = node2;
		if(node2->previous != NULL)
			node2->previous->next = node1;
		if(node1->previous != NULL && node1->previous != node2)
			node1->previous->next = node2;
		if(node2->next != NULL && node2->next != node1)
			node2->next->previous = node1;

		//Update the nodes pointers to complete the swap
		temp = (node1->previous == node2) ? node1 : node1->previous;
		node1->previous = node2->previous;
		node2->previous = temp;

		temp = node1->next;
		node1->next = (node2->next == node1) ? node2 : node2->next;
		node2->next = temp;
#if debug == 1
		printDLL(*DLL);
#endif
		if(node2->previous != node1)//Stop case for lists with an even number of elements
			reverse(nextNode1, nextNode2, DLL);
	}
}


/*
 * Function: freeDLL
 * Summary: Recursively frees a DLL starting at the given node
 * Parameters: A pointer to the node to start at
 * Returns: Nothing
*/
void freeDLL(DLLN *currentNode)
{
	DLLN *next;
	if(currentNode != NULL)
	{
		next = currentNode->next;
		free(currentNode);
		freeDLL(next);
	}
}
