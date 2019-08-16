/*
 ============================================================================
 Name        : A3Q7.c
 Author      : Rylee Thompson
 Version     :
 Copyright   :
 Description : Solution to Assignment 3 question 7: Implement the remove(u) 
 method, that removes the node u from a MeldableHeap. This method should run 
 in O(log n) expected time.
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define maxNumNodes 10
#define maxVal 100
#define COUNT 10

struct node{
	struct node* left;
	struct node* right;
	struct node* parent;
	int data;
};

struct heap{
	struct node *root;
	int size;
};


void addElement(int data, struct heap *meldHeap, struct node *currentNode);
void print2DUtil(struct node *root, int space);
struct node *merge(struct node *h1, struct node *h2, struct heap *meldHeap);
struct node *findNode(int data, struct node *currentNode);
void removeRandomNodes(struct heap *meldHealp, int dataArray[], int numElements);
void removeNode(struct node *remove, struct heap *meldHeap);
int *randomInitialization(struct heap *meldHeap, int numNodes);

int main(void)
{
	struct heap meldHeap;
	meldHeap.root = NULL;
	meldHeap.size = 0;

	srand(time(NULL));
	int numNodes = (rand() % maxNumNodes) + 1;
	int *dataArray = randomInitialization(&meldHeap, numNodes);

	printf("The randomly initialized meldable heap is:\n");
	print2DUtil(meldHeap.root, 0); //Print the heap to stdout in 2D

	removeRandomNodes(&meldHeap, dataArray, numNodes);

	free(dataArray);

	return 0;
}

/*
 * Function: randomInitialization
 * ------------------------------------------------------------------
 * Summary: Adds the given number of nodes to the given meldable heap,
 * where each node is in range [1, maxVal].
 * Parameters: A pointer to the heap to be added to, and the number of
 * nodes that are to be added.
 * Returns: A pointer to an array of ints containing all the elements 
 * that were added (must be free'd after use).
*/ 
int *randomInitialization(struct heap *meldHeap, int numNodes)
{
	int *dataArray = malloc(sizeof(int) * numNodes);
	for(int i = 0; i < numNodes; i++)
	{
		dataArray[i] = (rand() % maxVal) + 1;
		addElement(dataArray[i], meldHeap, meldHeap->root);
	}
	return dataArray;
}

/*
 * Function: addElement
 * -----------------------------------------------------------------------
 * Summary: Adds a node with the given data to the given meldable heap.
 * Parameters: The data to be added, a pointer to the meldable heap, and 
 * a pointer to the root of the tree (or subtree) to add the node to.
 * Returns: Nothing.
*/
void addElement(int data, struct heap *meldHeap, struct node *currentNode)
{
	struct node *newNode = malloc(sizeof(struct node));
	newNode->data = data;
	newNode->left = newNode->right = newNode->parent = NULL;
	meldHeap->size += 1;

	currentNode = merge(currentNode, newNode, meldHeap);
}
/*
 * Function: merge
 * ------------------------------------------------------------------------------
 * Summary: Recursively merges the two nodes to satisfy meldable heap properties.
 * Parameters: A pointer to the node being added, the node it is being added to,
 * and the tree containing both nodes.
 * Returns: A pointer to the updated node
*/
struct node *merge(struct node *h1, struct node *h2, struct heap *meldHeap)
{
	if(h1 == NULL) //Base case 1
	{
		if(meldHeap->root == NULL)
		{
			meldHeap->root = h2;
			h2->left = h2->right = h2->parent = NULL;
		}
		return h2;
	}
	if(h2 == NULL) //Base case 2
		return h1;

	//Make sure h1.data <= h2.data and update root
	struct node *temp = h1;
	h1 = h1->data <= h2->data ? h1 : h2;
	h2 = temp->data <= h2->data ? h2 : temp;
	meldHeap->root = h2 == meldHeap->root ? h1 : meldHeap->root;

	struct node *mergeNode[2];
	mergeNode[0] = h1->left, mergeNode[1] = h1->right;

	int randNum = rand() % 2; //Either 0 or 1 (like a coin flip) - for indexing mergeNode
	mergeNode[randNum] = merge(mergeNode[randNum], h2, meldHeap); //Recursive call
	mergeNode[randNum]->parent = h1;

	//Update left and right pointers if they were used in the recursive call above
	h1->left = randNum == 0 ? mergeNode[randNum] : h1->left;
	h1->right = randNum == 1 ? mergeNode[randNum] : h1->right;

	return h1;
}

/*
 * Function: findNode
 * ----------------------------------------------------------------
 * Summary: Finds a node with the given data in the meldable heap,
 * starts searching at the given node.
 * Parameters: The data to be found, and a pointer to the node to
 * start at.
 * Returns: A pointer to the node if found, otherwise NULL.
*/
struct node *findNode(int data, struct node *currentNode)
{
	if(currentNode == NULL || currentNode->data == data)
		return currentNode;

	struct node *node1 = findNode(data, currentNode->right);
	if(node1 != NULL && node1->data == data)
		return node1;
	else
		return findNode(data, currentNode->left);
}

/*
 * Function: removeRandomNodes
 * ------------------------------------------------------------------------------
 * Summary: Removes nodes from a meldable heap in a random order. Must pass an
 * array containing the data to be removed and the number of nodes to be removed.
 * Parameters: A pointer to the heap, an array of data to be removed, and the number
 * of nodes to be removed.
 * Returns: Nothing
*/
void removeRandomNodes(struct heap *meldHeap, int dataArray[], int numNodes)
{
	struct node *remove;
	int ix;
	while(meldHeap->size != 0)
	{
		ix = rand() % numNodes;
		/* 
		 * Rereading this before I submit and realized I probably should have
		 * made dataArray an array of pointers to nodes and not ints.... oh well
		*/
		remove = findNode(dataArray[ix], meldHeap->root);
		if(remove != NULL)
		{
			printf("\nremoving: %d\n", remove->data);
			removeNode(remove, meldHeap);
			printf("new heap: \n");
			print2DUtil(meldHeap->root, 0);
		}
	}

}
/*
 * Funtion: removeNode
 * -----------------------------------------------------------------------------------
 * Summary: Removes the given node from the given heap.
 * Parameters: A pointer to the node to be remove, and a pointer to the heap.
 * Returns: Nothing
*/
void removeNode(struct node *remove, struct heap *meldHeap)
{
	if(remove == NULL)
		return;

	struct node *newNode = merge(remove->left, remove->right, meldHeap);

	meldHeap->root = meldHeap->root == remove ? newNode : meldHeap->root;

	//Update parent pointer of the result from merge()
	if(newNode != NULL)
		newNode->parent = remove->parent;

	//Update parent pointers of the removed node
	if(remove->parent != NULL)
	{
		remove->parent->left = remove->parent->left == remove ? newNode : remove->parent->left;
		remove->parent->right = remove->parent->right == remove ? newNode : remove->parent->right;
	}

	meldHeap->size -= 1;
	free(remove);
}


/*
 * Function: print2DUtil
 * -------------------------------------------
 * Summary: Recursively prints the tree (given a pointer to the root) to stdout.
 * Parameters: A pointer to the root of the tree and spacing to be used (should be 0)
 * Returns: Nothing
 * Note: This function was taken from https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
 * for debugging and visualization purposes.
*/
void print2DUtil(struct node *root, int space)
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", root->data);

    // Process left child
    print2DUtil(root->left, space);
}
