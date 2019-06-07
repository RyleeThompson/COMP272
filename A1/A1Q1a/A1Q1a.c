/*
 ============================================================================
 Name        : A1Q1a.c
 Author      : Rylee Thompson
 Version     :
 Copyright   :
 Description : Solution to Assignment 1 question 1a: Describe the meaning of the essential methods add(x), deleteMin(), and
			   size() that are supported by the priority queue interface (5 marks).
			   Implement those methods using a singly-linked list (5 marks for each method).
			   Analyze the running time of the add(x) and deleteMin() operations based on this
			   implementation (5 marks).
 ============================================================================
 */

/*
 * add(x): Adds a node x to the queue
 * size(): Returns the number of nodes in the queue
 * deleteMin(): Removes the element with the lowest priority from the queue
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

struct node{
	int data;
	int priority;
	struct node *next;
};

struct PQueue{
	int size;
	int16_t min;
	struct node *front;
	struct node *rear;
};

#define maxVal 32767 //Max signed 16 bit value - max priority

struct PQueue initializeQueue(void);
char *getInput(void);
void add(struct node *newNode, struct PQueue *q);
int size(struct PQueue q);
struct node *deleteMin(struct PQueue *q);
void freeQueue(struct node *currentNode);
void printQ(struct PQueue q);

int main(void) {
	struct PQueue Queue; //The priority Queue struct
	char *response; //Stores user input
	int exit = 0; //Flag to exit the program
	char *endptr = NULL; //for strtol
	struct node *newNode; //Node being added to priority queue
	struct node *removedNode; //Removed node
	long int newVal = 0; //Data value for the new node - converted from string by strtol
	int newPriority = 0; //Priority value for the new node - converted from string by strtol
	int conversionError = 0; //Used to flag a conversion error from strtol
	//Initliaze queue to null/default values
	Queue = initializeQueue();
	newNode = malloc(sizeof(struct node));
	do{
		do{
			printf("Enter a number to add to the queue, S to call size(), D to call deleteMin(), or E to exit: ");
			fflush(stdout);
			//Retrieve user input
			response = getInput();
			//Attempt to convert user input to numerical and detect errors
			newVal = strtol(response, &endptr, 10);
			conversionError = (endptr == response) ? 1 : 0;
			conversionError = (*endptr != '\0') ? 1 : conversionError;
		}while(strcmp(response, "E") && strcmp(response, "S") && strcmp(response, "D") && conversionError == 1);

		//Add new node to queue
		if(strcmp(response, "E") && strcmp(response, "S") && strcmp(response, "D"))
		{
			do{
				free(response);
				printf("Enter the priority (numerical) of this data: ");
				fflush(stdout);
				//Retrieve user input
				response = getInput();
				//Attempt to convert user input to numerical and detect errors
				newPriority = strtol(response, &endptr, 10);
				conversionError = (endptr == response) ? 1 : 0;
				conversionError = (*endptr != '\0') ? 1 : conversionError;
			}while(conversionError == 1);
			Queue.min = (Queue.min < newPriority) ? Queue.min : newPriority;
			newNode->priority = newPriority;
			newNode->data = (int)newVal;
			//Add the new node to the queue
			add(newNode, &Queue);
		}

		//Call size()
		else if(!strcmp(response, "S"))
		{
			printf("The size of the priority queue is %d node(s).\n", size(Queue));
			fflush(stdout);
		}

		//Call deleteMin()
		else if(!strcmp(response, "D"))
		{
			if((removedNode = deleteMin(&Queue)) != NULL)
			{
				printf("Removed %d with priority %d from the queue\n", removedNode->data, removedNode->priority);
				free(removedNode);
			}
			else
				printf("Queue empty\n");
		}
		exit = (strcmp(response, "E")) ? 0 : 1;
	free(response);
	}while(exit == 0);
	free(newNode);
	freeQueue(Queue.front);
}
/*
 * Function: initializeQueue
 * ----------------------------------------
 * Summary: Initializes a priority queue to default values (null, zero)
 * Parameters: None
 * Returns: The initialized priority queue
 */
struct PQueue initializeQueue(void)
{
	struct PQueue init;
	init.size = 0;
	init.min = maxVal;
	init.front = NULL;
	init.rear = NULL;
	return init;
}

/*
 * Function: getInput
 * -----------------------------------------
 * Summary: Retrieves user input from stdin
 * Parameters: None
 * Returns: A pointer to the string
 * Notes: Does not include the terminating '\n' or EOF
 */
char *getInput(void)
{
	int i;
	char c;
	char *input;
	input = malloc(sizeof(char));
	for(i = 0; (c = getchar()) != '\n' && c != EOF; i++)
	{
		input[i] = c;
		input = realloc(input, sizeof(char) * (i + 2));
	}
	input[i] = '\0';
	return input;
}

/*
 * Function: add
 * --------------------------------------------
 * Summary: Adds a given node to the given priority queue
 * Parameters: A pointer to the node to be added to the queue
 * and a pointer to the queue itself
 * Returns: Nothing
 *
 * This function is constant time
 */
void add(struct node *newNode, struct PQueue *q)
{
	struct node *tmp;
	tmp = malloc(sizeof(struct node));
	q->size += 1;
	//Copy data to new struct
	tmp->data = newNode->data;
	tmp->priority = newNode->priority;
	
	if(q->front == NULL) //If the queue is empty
		q->front = tmp;
	//New node becomes rear
	else
		q->rear->next = tmp;
	tmp->next = NULL;
	q->rear = tmp;
}

/*
 * Function: size
 * --------------------------------------------
 * Summary: Returns the number of nodes in the priority queue
 * Parameters: The priority queue
 * Returns: The number of nodes in the priority queue - int
 */
int size(struct PQueue q)
{
	return q.size;
}

/*
 * Function: deleteMin
 * ---------------------------------------------
 * Summary: Deletes the node with the lowest priority from the queue
 * Parameters: A pointer to the priority queue
 * Returns: The removed node, or NULL if the queue is empty
 *
 * This implementation is linear in the number of nodes, as we
 * have to iterate through the entire queue twice, once to find the node to be removed
 * and once to update the minimum value
 * Note: Nodes deleted using this function will need to be free'd outside
 * of this function to avoid memory leaks.
 */
struct node *deleteMin(struct PQueue *q)
{
	struct node *removedNode;
	struct node *previousNode = NULL;
	struct node *i;
	int16_t newMinimum = maxVal;
	if((removedNode = q->front) != NULL)
	{
		//Find the node with the min priority
		while(removedNode->priority != q->min)
		{
			previousNode = removedNode;
			removedNode = removedNode->next;
		}
		
		//Remove the node from the queue
		if(previousNode != NULL) //If the removed node is not the front
			previousNode->next = removedNode->next;
		else
			q->front = q->front->next;
		
		//Update rear if necessary
		q->rear = (q->rear == removedNode) ? previousNode : q->rear;
		if(q->rear != NULL)
			q->rear->next = NULL;
		
		//Find the new minimum and update it in the struct
		for(i = q->front; i != NULL; i = i->next)
			newMinimum = (i->priority < newMinimum) ? i->priority : newMinimum;
		q->size -= 1;
		q->min = newMinimum;
		return removedNode;
	}
	return NULL;
}

/*
 * Function: freeQueue
 * Summary: Recursively frees a Queue starting at the given node
 * Parameters: The node to start at
 * Returns: Nothing
*/
void freeQueue(struct node *currentNode)
{
	struct node *next;
	if(currentNode != NULL)
	{
		next = currentNode->next;
		free(currentNode);
		freeQueue(next);
	}
}
