/*
 * ========================================================================
 * Author: Rylee Thompson
 * Summary: Solution to COMP272 Assignment 1 question 1b: Implement a stack
 * using two FIFO queues.
 * ========================================================================
*/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct node{
	int data;
	struct node *next;
};

struct queue{
	int size;
	struct node *front;
	struct node *rear;
};

char *getInput(void);
struct queue initializeQueue(void);
struct node *dequeue(struct queue *q);
void enqueue(struct queue *q, struct node *QNode);
int isempty(struct queue q);
struct node *pop(struct queue *q1, struct queue *q2);
void push(struct queue *q1, struct queue *q2, struct node *newNode);

int main(void)
{
	struct queue Queue1; //The first queue
	struct queue Queue2; //The second queue
	struct queue temp;
	char *response; //Stores user input
	int exit = 0; //Program exit flag
	char *endptr = NULL; //For strtol
	struct node *newNode; //New node to be pushed
	struct node *poppedNode; //Node that was popped
	long int newVal = 0; //Integer converted by strtol
	int convError = 0; //Used to detect errors in strtol

	//Initalize Queues
	Queue1 = initializeQueue();
	Queue2 = initializeQueue();
	//Initialize node
	newNode = malloc(sizeof(struct node));
	do{
		do{
			printf("Enter a number to push, P to pop, or E to exit:");
			//Retrieve user input
			response = getInput();
			//Try and convert it to integer
			newVal = strtol(response, &endptr, 10);
			//Detect if there was an error
			convError = (endptr == response) ? 1 : 0;
			convError = (*endptr != '\0') ? 1 : convError;
		}while(strcmp(response, "P") && strcmp(response, "E") && convError == 1); //Until the input is acceptable
		//Pop
		if(!strcmp(response, "P"))
		{
			//Pop a node
			poppedNode = pop(&Queue1, &Queue2);
			if(poppedNode != NULL) //If the stack wasn't empty
			{
				printf("popped: %d\n", poppedNode->data);
				free(poppedNode);
				//Switch Queue1 and Queue2
				temp = Queue2;
				Queue2 = Queue1;
				Queue1 = temp;
			}
		}
		//Push
		else if(strcmp(response, "E"))
		{
			printf("pushing: %ld\n", newVal);
			newNode->data = (int)newVal; //Set the data of the node to the integer input by user
			//Push the node
			push(&Queue1, &Queue2, newNode);
		}
		exit = (strcmp(response, "E")) ? 0 : 1;
		free(response);
	}while(exit == 0); //Until "E" is input by user

	//Free data left on the stack
	while(!isempty(Queue1))
	{
		poppedNode = dequeue(&Queue1);
		free(poppedNode);
	}
	free(newNode);
	return 0;
}

/*
 * Function: getInput
 * ------------------------
 * Summary: Retrieves user input from stdin
 * Parameters: None
 * Returns: Pointer to an array containing the user input
 * not including the '\n' or EOF
*/
char *getInput(void)
{
	int i;
	char c;
	char *input; //Pointer to be returned
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
 * Function: initializeQueue
 * ------------------------------
 * Summary: Initializes a queue struct to default values.
 * Parameters: None
 * Returns: An initialized queue struct
*/
struct queue initializeQueue(void)
{
	struct queue init;
	init.size = 0;
	init.front = NULL;
	init.rear = NULL;
	return init;
}

/*
 * Function: isempty
 * -----------------------
 * Summary: Checks if a queue is empty
 * Parameters: Queue to be checked
 * Returns: 1 if empty, 0 otherwise
*/
int isempty(struct queue q)
{
	return q.size == 0;
}

/*
 * Function: enqueue
 * ----------------------------
 * Summary: Adds a node to the given queue
 * Parameters: A pointer to the node to be queued and a pointer to the queue itself
 * Returns: Nothing
 */
void enqueue(struct queue *q, struct node *QNode)
{
	struct node *tmp;
	tmp = malloc(sizeof(struct node));
	q->size += 1;
	tmp->data = QNode->data;
	if(q->front == NULL) //If the queue was empty
		q->front = tmp;
	else //New node becomes rear
		q->rear->next = tmp;
	tmp->next = NULL;
	q->rear = tmp;
}

/*
 * Function: dequeue
 * -------------------------------
 * Summary: Dequeues a node from the given queue
 * Parameters: A pointer to the queue
 * Returns: A pointer to the dequeued node, or NULL if the queue was empty
*/
struct node *dequeue(struct queue *q)
{
	struct node *tmp;
	if(!isempty(*q))
	{
		//Front becomes front->next
		tmp = q->front;
		q->front = q->front->next;
		q->size -= 1;
		return tmp;
	}
	else
		return NULL;
}


/*
 * Function: push
 * ----------------------------
 * Summary: Uses two queues to implement the push functionality of a stack
 * Parameters: Two pointers to two queues, and a pointer to the node to be pushed
 * Returns: Nothing
 * The running time of this function is contant time.
*/
void push(struct queue *q1, struct queue *q2, struct node *newNode)
{
	enqueue(q1, newNode);
}

/*
 * Function: pop
 * ---------------------------
 * Summary: Uses two queues to imitate the pop function of a stack
 * Parameters: Two pointers to two queues
 * Returns: A pointer to the popped node, or NULL if the stack is empty
 * Notes: The push implementation simply adds all new nodes to q1. Pop simply
 * dequeues from q1 into q2 until it reaches the last node, and then returns this node.
 * The queues are then switched in main so that the function continues to work after the
 * first pop.
 *
 * The running time of this function is linear in the number of elements in the stack as
 * you have to dequeue all elements before the return value is found.
*/
struct node *pop(struct queue *q1, struct queue *q2)
{
	struct node *dupe;
	if(!isempty(*q1))
	{
		/*Dequeue all elements from q1 and queue them in q2 until the last
		element is reached */
		while(q1->front != q1->rear)
		{
			dupe = dequeue(q1);
			enqueue(q2, dupe);
		}
		//Return the last element in the queue
		return dequeue(q1);
	}
	else
	{
		printf("Stack empty\n");
		return NULL;
	}
}
