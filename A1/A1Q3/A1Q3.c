/*
 ============================================================================
 Name        : A1Q3.c
 Author      : Rylee Thompson
 Version     :
 Copyright   :
 Description : Solution to Assignment 1 question 3: Using a USet, implement a
 bag that supports the add(x), remove(x), find(x), and findAll(x) methods.
 Unlike a USet, duplicate elements can be stored in a bag.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

struct bagNode{
	int key;
	char value;
	struct bagNode *next;
};

struct LList{
	int size;
	struct bagNode *front;
	struct bagNode *rear;
};

struct LList initializeList(void);
void add(struct bagNode newNode, struct LList *bag);
struct bagNode *removeNode(int x, struct LList *bag);
void printBag(struct LList bag);
struct bagNode *find(int x, struct LList *bag);
struct LList findAll(int x, struct LList *bag);
void freeBag(struct bagNode *node);

#define debug 0

int main(void) {
	struct LList bag; //The linked list struct - used to create a bag
	int i;
	char c;

	//Initliaze queue to null/default values
	bag = initializeList();

	struct bagNode newNode; //New node to be added to the bag
	//Add data to the bag
	for(i = 0, c = '<'; i < 5; i++, c++)
	{
		newNode.key = i;
		newNode.value = c;
		add(newNode, &bag);
	}
	for(; i >= 0; i--, c++)
	{
		newNode.key = i;
		newNode.value = c;
		add(newNode, &bag);
	}
	printBag(bag);

	struct LList findAllBag;
	int x = 0;
	printf("Finding all keys = %d in the bag\n", x);
	findAllBag = findAll(x, &bag);
	printf("Found the following entries: ");
	printBag(findAllBag);

	struct bagNode *removedNode;
	removedNode = removeNode(x, &bag);
	free(removedNode);
	removedNode = removeNode(x, &bag);
	free(removedNode);

	struct bagNode *foundNode;
	int y = 3;
	printf("finding %d in the bag\n", y);
	foundNode = find(y, &bag);
	if(foundNode != NULL)
		printf("found key %d with value %c\n", y, foundNode->value);
	else
		printf("Key %d not in bag\n", y);

	freeBag(bag.front);
	freeBag(findAllBag.front);
}
/* Function: initializeList
 * ----------------------------------------
 * Summary: Initializes a linked list to default values (null, zero)
 * Parameters: None
 * Returns: The initialized list
 */
struct LList initializeList(void)
{
	struct LList init;
	init.size = 0;
	init.front = NULL;
	init.rear = NULL;
	return init;
}

/*
 * Function: add
 * --------------------------------------------
 * Summary: Adds a given node to the given bag
 * Parameters: A pointer to the node to be added to the bag
 * and a pointer to the bag itself
 * Returns: Nothing
 *
 * This function is constant time
 */
void add(struct bagNode newNode, struct LList *bag)
{
	struct bagNode *tmp;
	tmp = malloc(sizeof(struct bagNode));
	bag->size += 1;
	tmp->key = newNode.key;
	tmp->value = newNode.value;
	if(bag->front == NULL) //If the bag was empty
		bag->front = tmp;
	//Update the rear
	else
		bag->rear->next = tmp;
	tmp->next = NULL;
	bag->rear = tmp;
}

/*
 * Function: removeNode
 * ---------------------------------------------
 * Summary: Deletes the first node with a key equal to the given value
 * from the bag
 * Parameters: A key (int), and a pointer to the bag
 * Returns: The removed node, or NULL if a node with the given key is not found
 * Note: The returned node will need to be free'd outside of this function to
 * prevent memory leaks.
 */
struct bagNode *removeNode(int x, struct LList *bag)
{
	struct bagNode *removedNode;
	struct bagNode *previousNode = NULL;
	int found;
	printf("Removing %d from the bag\n", x);
	
	//Find the node to be removed and keep a copy of the previous node
	for(removedNode = bag->front; removedNode != NULL && removedNode->key != x; removedNode = removedNode->next)
		previousNode = removedNode;
	found = (removedNode != NULL) ? 1 : 0;
	if(found == 1)
	{
		if(previousNode != NULL)
			previousNode->next = removedNode->next;
		else //The node to be removed is the front node
			bag->front = bag->front->next;

		//Update rear if necessary
		bag->rear = (bag->rear == removedNode) ? previousNode : bag->rear;
		if(bag->rear != NULL)
			bag->rear->next = NULL;

		bag->size -= 1;
		printf("Removed key %d with value %c from the bag\n", x, removedNode->value);
		printBag(*bag);
	}
	else
		printf("Key not found\n");
	return removedNode;
}

/*
 * Function: printBag
 * Summary: Prints the keys and values of the given bag
 * Parameters: The bag to be printed
 * Returns: Nothing
*/
void printBag(struct LList bag)
{
	struct bagNode *currentNode;
	printf("The bag is: \n");
	printf("keys : values\n");
	for(currentNode = bag.front; currentNode != NULL; currentNode = currentNode->next)
	{
#if debug == 0
		printf("%d : %c\n", currentNode->key, currentNode->value);
#endif
#if debug == 1
		printf("current node: %d\n", currentNode->key);
		if(currentNode->next != NULL)
			printf("next: %d\n", currentNode->next->key);
		else
			printf("next: NULL\n");
#endif
	}
}

/*
 * Function: find
 * Summary: Finds a node in the given bag with a key equal to the key given
 * Parameters: The key to be found, and a pointer to the bag to be searched
 * Returns: A pointer to the node (not a copy)
*/
struct bagNode *find(int x, struct LList *bag)
{
	struct bagNode *currentNode;
	
	//Iterate until the bag is found or the end of the list is reached
	for(currentNode = bag->front; currentNode != NULL && currentNode->key != x; currentNode = currentNode->next)
		;
	return currentNode;
}

/*
 * Function: findAll
 * Summary: Finds all nodes in the given bag with a key equal to the key given
 * Parameters: The key and a pointer to the bag to be searched
 * Returns: Another bag that contains all nodes with a key equal to the given key
 * Note: The new bag contains a copy of the nodes from the original bag, and thus needs
 * to be free'd separately from the original bag
*/
struct LList findAll(int x, struct LList *bag)
{
	struct bagNode *currentNode;
	struct LList newBag; //All found nodes with the matching key are added to this bag
	newBag = initializeList();
	
	//Iterate through the entire bag, and add nodes with the correct key to the bag
	for(currentNode = bag->front; currentNode != NULL; currentNode = currentNode->next)
		if(currentNode->key == x)
			add(*currentNode, &newBag);
	return newBag;
}

/*
 * Function: freeBag
 * Summary: Recursively frees a bag starting at the given node
 * Parameters: A node in a bag
 * Returns: Nothing
 * Note: To free the entire bag, bag->front should be passed to this function
*/
void freeBag(struct bagNode *node)
{
	struct bagNode *next;
	if(node != NULL)
	{
#if debug == 1
		printf("Freeing : %d\n", node->key);
#endif
		next = node->next;
		free(node);
		freeBag(next);
	}
}
