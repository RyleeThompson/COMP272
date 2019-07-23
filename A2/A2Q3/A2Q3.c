/*
 ============================================================================
 Name        : A2Q2.c
 Author      : Rylee Thompson
 Version     :
 Copyright   :
 Description : Solution to Assignment 2 question 3: Illustrate what happens 
 when the sequence 1, 5, 2, 4, 3 is added to an empty ScapegoatTree, and show
 where the credits described in the proof of Lemma 8.3 go, and how they are 
 used during this sequence of additions
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

struct node{
    int data;
    struct node* parent;
    struct node* right;
    struct node* left;
};

struct btree{
    struct node *root;
    int numNodes;
};

void print2DUtil(struct node *root, int space);
void addDataArrayToTree(int dataArray[], int numElements, struct btree *tree);
void insertNode(int data, struct btree *tree);
void addNode(struct node *parent, struct node *child, struct btree *tree);
void updateNodePointers(struct node *parent, struct node *child, struct btree *tree);
int size(struct node *currentNode);
int depth(struct node *currentNode, struct btree tree);
float log32(int n);
struct node* findSGoat(struct node *currentNode, struct btree tree);
int storeInArray(struct node* currentNode, struct node **nodeArray, int i);
int compareFunc(const void *a, const void *b);
void rebuildTree(struct node *child, struct btree *tree);
void addArrayToTree(struct node **nodeArray, struct node *newSubtreeRoot, struct node *originalSubtreeRoot, int lower, int higher, struct btree *tree);
void freeTree(struct node *currentNode);

#define COUNT 10

int main(void)
{
    struct btree bst; //The binary search tree
    int nodeData[] = {1, 5, 2, 4, 3}; //The data to be added to the scapegoat tree
    bst.root = NULL;
    bst.numNodes = 0;

    //Add data from array to bst
    addDataArrayToTree(nodeData, (sizeof(nodeData)/sizeof(int)), &bst);

    printf("The binary search tree is:\n");
    print2DUtil(bst.root, 0); //Print the bst to stdout in 2D

    freeTree(bst.root);

    //Same thing but with a larger tree
    int nodeData2[] = {8, 9 , 10, 7, 6, 2, 1, 0, 5, 3, 4}; //The data to be added to the bst
    struct btree bst2;

    bst2.root = NULL;
    bst.numNodes = 0;

    addDataArrayToTree(nodeData2, (sizeof(nodeData2)/sizeof(int)), &bst2);

    printf("The binary search tree is:\n");
    print2DUtil(bst2.root, 0);

    freeTree(bst2.root);

    return 0;
}

/*
 * Function: addDataArrayToTree
 * ------------------------------------------
 * Summary: Adds the given array of ints to the given tree
 * Parameters: The array of data to be added, the number of elements in the
 * array, and a pointer to the tree it is to be added to.
 * Returns: Nothing
*/
void addDataArrayToTree(int dataArray[], int numElements, struct btree *tree)
{
    for(int i = 0; i < numElements; i++)
	insertNode(dataArray[i], tree);
}


/*
 * Function: insertNode
 * --------------------------------------------
 * Summary: Inserts a node with the given data into the given scapegoat tree and rebuilds
 * the tree if necessary.
 * Parameters: A pointer to the tree and the data to be added.
 * Returns: Nothing
 * Note: Modifies the tree in place.
*/
void insertNode(int data, struct btree *tree)
{
    struct node *child = malloc(sizeof(struct node));
    child->data = data;
    child->left = child->right = NULL;

    addNode(tree->root, child, tree);

    //Check if the added node violates rules of scapegoat trees
    if(depth(child, *tree) > log32(tree->numNodes))
    {
	printf("The tree before rebuilding (rule violated when adding node with data %d):\n", child->data);
	print2DUtil(tree->root, 0);
        rebuildTree(child, tree);
    }
}

/*
 * Function: addNode
 * ------------------------------------------
 * Summary: Adds the given node to the given bst tree.
 * Parameters: A pointer to the root of the tree, a pointer to the node to be added,
 * and a pointer to the tree itself.
 * Returns: Nothing
*/
void addNode(struct node *parent, struct node *child, struct btree *tree)
{
    if(parent == NULL) //This branch executes when adding the root of the tree
    {
	tree->root = child;
	child->parent = NULL;
	tree->numNodes += 1;
    }


    //Process left subtree
    else if(child->data < parent->data)
    {
	if(parent->left != NULL)
	    addNode(parent->left, child, tree);
	else
            updateNodePointers(parent, child, tree);
    }

    //Process right subtree
    else if(child->data > parent->data)
    {
	if(parent->right != NULL)
	    addNode(parent->right, child, tree);
	else
	    updateNodePointers(parent, child, tree);
    }
}

/*
 * Function: updateNodePointers
 * -----------------------------------------------
 * Summary: Updates the node pointers such that the given child is added to the tree.
 * Parameters: A pointer to the parent, a pointer to the child, and a pointer to the tree.
 * Returns: Nothing
*/
void updateNodePointers(struct node *parent, struct node *child, struct btree *tree)
{
    parent->left = child->data < parent->data ? child : parent->left;
    parent->right = child->data > parent->data ? child : parent->right;
    child->parent = parent;
    child->left = child->right = NULL;
    tree->numNodes += 1;
}

/*
 * Function: size
 * -------------------------------------------------
 * Summary: Returns the size of the given node
 * Parameters: A pointer to the node whose size is to be determined.
 * Returns: The size of the node (int)
*/
int size(struct node *currentNode)
{
    if(currentNode == NULL)
	return 0;

    return 1 + size(currentNode->left) + size(currentNode->right);
}

/*
 * Function: depth
 * --------------------------------------------------
 * Summary: Returns the depth of the given node
 * Parameters: A pointer to the node whose depth is to be determined,
 * as well as the tree it is in.
 * Returns: The depth of the node in the tree (int)
*/
int depth(struct node *currentNode, struct btree tree)
{
    if(currentNode == tree.root)
	return 0;

    return 1 + depth(currentNode->parent, tree);
}

/*
 * Function: log32
 * ------------------------------------------
 * Summary: Computes the log with base 3/2 of the given integer
 * Parameters: The integers whose log is to be computed.
 * Returns: The log base 3/2 of the given integer (float)
*/
float log32(int n)
{
    return log(n)/log(1.5);
}

/*
 * Function: findSGoat
 * ------------------------------------------------
 * Summary: Finds the scapegoat given a pointer to the violating node and the tree itself
 * Parameters: A pointer to the violating node and the tree it is in
 * Returns: A pointer to the scapegoat.
*/
struct node* findSGoat(struct node *currentNode, struct btree tree)
{
    if(currentNode == tree.root)
	return currentNode;

    if(((float)size(currentNode)/size(currentNode->parent)) > ((float)2/3))
	return currentNode->parent;

    return findSGoat(currentNode->parent, tree);
}

/*
 * Function: storeInArray
 * ----------------------------------------------------
 * Summary: Stores the entire left and right subtrees of the given node in the given
 * array of pointers to structs.
 * Parameters: A pointer to the node to start at, an array of pointers to structs, and
 * an indexing integer (zero on first call).
 * Returns: The number of nodes stored in the array (int)
 * Notes: Enough memory must be alloc'd to the array of pointers prior to calling this
 * functon.
*/
int storeInArray(struct node *currentNode, struct node **nodeArray, int i)
{
    if(currentNode == NULL)
	return i;

    nodeArray[i++] = currentNode;
    i = storeInArray(currentNode->left, nodeArray, i);
    return storeInArray(currentNode->right, nodeArray, i);
}

/*
 * Function: rebuildTree
 * ---------------------------------------------------
 * Summary: Rebuilds the scapegoat tree given a pointer to the violating node and
 * a pointer to the tree.
 * Parameters: A pointer to the violating node and a pointer to the tree.
 * Returns: Nothing
*/
void rebuildTree(struct node* child, struct btree *tree)
{
    //Find scapegoat
    struct node *sGoat = findSGoat(child, *tree);
    int sGoatSize = size(sGoat);

    //Put the scapegoat and its left and right subtree into an array and sort it
    struct node **nodeArray = malloc(sizeof(struct node*) * sGoatSize);
    storeInArray(sGoat, nodeArray, 0);
    qsort(nodeArray, sGoatSize, sizeof(struct node*), compareFunc);

    //Update tree->root if necessary
    for(int i = 0; i < sGoatSize; i++)
	tree->root = nodeArray[i] == tree->root ? nodeArray[tree->numNodes/2] : tree->root;
    tree->root->parent = NULL;

    //Update scapegoats pointers
    if(sGoat->parent != NULL)
    {
	sGoat->parent->left = sGoat->parent->left == sGoat ? nodeArray[sGoatSize/2] : sGoat->parent->left;
	sGoat->parent->right = sGoat->parent->right == sGoat ? nodeArray[sGoatSize/2] : sGoat->parent->right;
    }
    struct node *temp;
    temp = sGoat->parent;
    sGoat->parent = nodeArray[sGoatSize/2]->parent;
    nodeArray[sGoatSize/2]->parent = temp;
    nodeArray[sGoatSize/2]->left = nodeArray[sGoatSize/2]->right = NULL;

    //Add the array of pointers to the tree
    addArrayToTree(nodeArray, nodeArray[sGoatSize/2], nodeArray[sGoatSize/2], 0, sGoatSize - 1, tree);

    //Update number of nodes (count is increased as a result of reordering the tree)
    tree->numNodes -= (sizeof(nodeArray)/sizeof(struct node*));

    free(nodeArray);
}

/*
 * Function: addArrayToTree
 * ---------------------------------------
 * Summary: Adds the given array of pointers to nodes to the given tree starting at the
 * given originalSubtreeRoot (the node which is replacing the scapegoat). Uses recursion
 * to break the array into two halves and add the middle element to the tree.
 * Parameters: An array of pointers to nodes, a pointer to the root of the subtree, the minimum
 * and maximum array index bound, and a pointer to the tree to be added to.
 * Returns: Nothing
 * Notes: The elements of the given array of nodes are set to NULL after calling
 * this function.
*/
void addArrayToTree(struct node **nodeArray, struct node *newSubtreeRoot, struct node *originalSubtreeRoot, int lower, int higher, struct btree *tree)
{

    if(lower < higher)
    {
	int middle = (lower + higher) / 2;

        //Add middle element to tree
	if(nodeArray[middle] != NULL && nodeArray[middle] != originalSubtreeRoot)
	    addNode(newSubtreeRoot, nodeArray[middle], tree);

        /* updating newSubtreeRoot like this serves to make this function run in O(size(scapegoat)) time; we are directly
         * passing the child to be added and its parent to addNode - addNode will not need to use recursion to insert the
	 * child properly */
	newSubtreeRoot = nodeArray[middle];
	nodeArray[middle] = NULL;

	//Recursive call for left half of array
	addArrayToTree(nodeArray, newSubtreeRoot, originalSubtreeRoot, lower, middle - 1, tree);
	//Recursive call for right half of array
	addArrayToTree(nodeArray, newSubtreeRoot, originalSubtreeRoot, middle + 1, higher, tree);
    }
    else if(nodeArray[lower] != NULL && nodeArray[lower] != originalSubtreeRoot)
    {
	addNode(newSubtreeRoot, nodeArray[lower], tree);
	nodeArray[lower] = NULL;
    }
}

/*
 * Function: compareFunc
 * ----------------------------------------------
 * Summary: Compare function for qsort. Sorts array of pointers in ascending order based
 * on their data.
*/
int compareFunc(const void *a, const void *b)
{
    struct node *one = *(struct node**)a;
    struct node *two = *(struct node**)b;
    return (one->data - two->data);
}

/*
 * Function: freeTree
 * ---------------------------------------------
 * Summary: Frees the dynamically allocated tree starting at the given node
 * Parameters: A pointer to the node to start at
 * Returns: Nothing
*/
void freeTree(struct node *currentNode)
{
    if(currentNode == NULL) //Base case
	return;

    freeTree(currentNode->left); //Free left subtree
    currentNode->left = NULL;

    freeTree(currentNode->right); //Free right subtree
    currentNode->right = NULL;

    free(currentNode); //Free current node
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
