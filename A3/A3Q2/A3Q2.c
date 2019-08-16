/*
 ============================================================================
 Name        : A3Q2.c
 Author      : Rylee Thompson
 Version     :
 Copyright   :
 Description : Solution to Assignment 3 question 2:  Illustrate that via AVL 
 single rotation, any binary search tree T1 can be transformed into another 
 search tree T2 (with the same items) (5 marks).
 Give an algorithm to perform this transformation using O(N log N) rotation
 on average (10 marks).
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

struct node{
    int data;
    int balance;
    int size;
    struct node* right;
    struct node* left;
    struct node* parent;
};

struct btree{
    struct node *root;
};

void initializeBST(struct btree *tree, int numNodes);
void insertNode(int data, struct btree *tree);
int addAVLNode(struct node *parent, struct node *child, struct btree *tree);
void updateNodePointers(struct node *parent, struct node *child, struct btree *tree);
void print2DUtil(struct node *root, int space);
void addNode(struct node *currentNode, int data);
void freeTree(struct node *currentNode);
void rotateRight(struct node *root, struct btree *tree);
void rotateLeft(struct node *root, struct btree *tree);
void leftRightRotation(struct node *criticalNode, struct btree *tree);
void rightLeftRotation(struct node *criticalNode, struct btree *tree);
struct node* findCriticalNode(struct node *currentNode, struct btree tree);
int *findRotationType(struct node *criticalNode, struct node *child);
void rebuildAVLTree(struct node *criticalNode, struct node *child, struct btree *tree);
int max(int h1, int h2);
void updateHeightAndBalance(struct node *N);
int size(struct node *N);
int balance(struct node *N);
#define COUNT 10
#define maxNumNodes 7

int main(void)
{
    struct btree bst; //The binary search tree
    int numNodes;
    srand(time(NULL));
    numNodes = rand() % maxNumNodes + 1;

    initializeBST(&bst, numNodes); //Fill the bst with the number of nodes
    printf("The randomized binary search tree is:\n");
    print2DUtil(bst.root, 0); //Print the bst to stdout in 2D

    printf("After rotating right:\n");
    rotateRight(bst.root, &bst);
    print2DUtil(bst.root, 0);

    printf("After rotating left (back to original): \n");
    rotateLeft(bst.root, &bst);
    print2DUtil(bst.root, 0);

    freeTree(bst.root);

    return 0;
}

/*
 * Function: initializeBST
 * -------------------------------
 * Summary: Initializes the given tree to have the given number of nodes
 * and to satisfy BST properties. The root is node is a random number
 * between 40-60, while all nodes are between 0-100.
 * Parameters: A pointer to the tree to be initialized and the number of
 * nodes it should contain.
 * Returns: Nothing
 * Note: Modifies the given tree in place
*/

void initializeBST(struct btree *tree, int numNodes)
{
    struct node *root = malloc(sizeof(struct node));
    root->data = (rand() % 20) + 40; //Random number between 40-60
    tree->root = root;
    //Fill the tree with given number of nodes
    for(int i = 0; i < numNodes - 1; i++)
	insertNode(rand() % 100, tree);

}
/*
 * Function: insertNode
 * ------------------------------------------------------
 * Summary: Inserts a node with the given data into the given tree
 * and rebuilds if necessary to satisfy AVL tree properties.
 * Parameters: The data to be added and a pointer to the tree to add it to.
 * Returns: Nothing
*/
void insertNode(int data, struct btree *tree)
{
	//Initializations
    struct node *child = malloc(sizeof(struct node));
    child->data = data;
    child->left = child->right = NULL;
    child->balance = 0;
    child->size = 1;

    int rebuild = 0;
    rebuild = addAVLNode(tree->root, child, tree);
    if(rebuild)
    {
	struct node *criticalNode = NULL;
	criticalNode = findCriticalNode(child, *tree);
	rebuildAVLTree(criticalNode, child, tree);
    }
}
/*
 * Function: addAVLNode
 * -------------------------------------------------------------------------
 * Summary: Adds the given node to the given tree, and updates the balance 
 * of nodes encountered in recursion to determine if rebuilding is required.
 * Parameters: A pointer to the root of the tree the node is to be added to,
 * a pointer to the node to be added, and a pointer to the tree.
 * Returns: Whether the tree needs to be rebuilt or not (1 or 0) (int)
*/
int addAVLNode(struct node *parent, struct node *child, struct btree *tree)
{
    static int rebuild = 0;
    rebuild = parent == tree->root ? 0 : rebuild;
	static int duplicate = 0; //Keeps track of whether a node with the given data has already been added to the tree
    duplicate = parent == tree->root ? 0 : duplicate;
    duplicate = child->data == parent->data ? 1 : duplicate;
    int tempBalance = parent->balance;

	//First node to be added to the tree
    if(parent == NULL)
    {
	tree->root = child;
	child->parent = NULL;
	return rebuild;
    }
	//Left subtree
    else if(child->data < parent->data)
    {
	parent->balance += 1;

	if(parent->left != NULL)
	    addAVLNode(parent->left, child, tree);
	else
	    updateNodePointers(parent, child, tree); //Insert the node as left child of parent
    }
	
	//Right subtree
    else if(child->data > parent->data)
    {
	parent->balance -= 1;

	if(parent->right != NULL)
	    addAVLNode(parent->right, child, tree);
	else
	    updateNodePointers(parent, child, tree); //Insert the node as right child of parent
    }

    parent->balance = duplicate == 1 ? tempBalance : parent->balance; //Update parent balance if duplicate
    parent->size = 1 + size(parent->left) + size(parent->right);
    rebuild = abs(parent->balance) > 1 ? 1 : rebuild;
    return rebuild;
}

/*
 * Function: updateNodePointers
 * ---------------------------------------------------------------------------------
 * Summary: Updates the given pointers of the parent and child to complete the insertion of
 * the child node.
 * Parameters: A pointer to the parent, child, and tree it is being inserted in.
 * Returns: Nothing
*/
void updateNodePointers(struct node *parent, struct node *child, struct btree *tree)
{
    parent->left = child->data < parent->data ? child : parent->left;
    parent->right = child->data > parent->data ? child : parent->right;
    child->parent = parent;
    child->left = child->right = NULL;
}
/*
 * Function: rotateRight
 * -------------------------------------------------------------
 * Summary: Rotates the given tree to the right around the given node.
 * Parameters: A pointer to the node to be rotated around and the tree.
 * Returns: Nothing
*/
void rotateRight(struct node* root, struct btree *tree)
{
    if(root->left == NULL)
	return;

    struct node *goingDown = root;
    struct node *goingUp = root->left;
    struct node *movingOver = goingUp->right;

    //Move the root down and its left child up
    if(goingDown->parent != NULL)
    {
	goingDown->parent->left = goingDown->parent->left == goingDown ? goingUp : goingDown->parent->left;
	goingDown->parent->right = goingDown->parent->right == goingDown ? goingUp : goingDown->parent->right;
    }
    goingUp->parent = goingDown->parent;
    goingDown->parent = goingUp;

    //Move the right subtree of the new root to be the left subtree of the old root
    goingDown->left = movingOver;
    if(movingOver != NULL)
        movingOver->parent = goingDown;
    goingUp->right = goingDown;

    updateHeightAndBalance(goingDown);
	updateHeightAndBalance(goingUp);

	//Update root of tree
    tree->root = tree->root == root ? goingUp : tree->root;
}
/*
 * Function: rotateLeft
 * ---------------------------------------------------------------
 * Summary: Rotates the given tree to the left around the given node.
 * Parameters: A pointer to the node to be rotated around and the tree.
 * Returns: Nothing
*/
void rotateLeft(struct node *root, struct btree *tree)
{
    if(root->right == NULL)
	return;

    struct node *goingDown = root;
    struct node *goingUp = root->right;
    struct node *movingOver = goingUp->left;

    //Move the root down and its right child up
    if(goingDown->parent != NULL)
    {
	goingDown->parent->left = goingDown->parent->left == goingDown ? goingUp : goingDown->parent->left;
	goingDown->parent->right = goingDown->parent->right == goingDown ? goingUp : goingDown->parent->right;
    }
    goingUp->parent = goingDown->parent;
    goingDown->parent = goingUp;

    //Move the left subtree of the new node to be the right subtree of the old node
    goingDown->right = movingOver;
    if(movingOver != NULL)
        movingOver->parent = goingDown;
    goingUp->left = goingDown;

    updateHeightAndBalance(goingDown);
	updateHeightAndBalance(goingUp);

    tree->root = tree->root == root ? goingUp : tree->root;
}

/*
 * Function: rightLeftRotation
 * -----------------------------------------------------------------
 * Summary: Performs a right-left rotation to preserve AVL properties 
 * given the critical node and the tree.
 * Parameters: A pointer to the critical node and the tree.
 * Returns: Nothing
*/
void rightLeftRotation(struct node *criticalNode, struct btree *tree)
{
	rotateRight(criticalNode->right, tree);
	rotateLeft(criticalNode, tree);
}
/* 
 * Function: leftRightRotation
 * -----------------------------------------------------------------
 * Summary: Performs a left-right rotation to preserve AVL properties
 * given the critical node and the tree.
 * Parameters: A pointer to the critical node and the tree.
 * Returns: Nothing.
*/
void leftRightRotation(struct node *criticalNode, struct btree *tree)
{
	rotateLeft(criticalNode->left, tree);
	rotateRight(criticalNode, tree);
}

/*
 * Function: rebuildAVLTree
 * --------------------------------------------------------------------------------
 * Summary: Rebuilds the given AVL tree given a pointer to the critical node
 * and the recently added node. Finds what rotation type is required and
 * executes it.
 * Parameters: A pointer to the critical node, recently added node, and the tree.
 * Returns: Nothing
*/
void rebuildAVLTree(struct node *criticalNode, struct node *child, struct btree *tree)
{
	void (*rotation[4])(struct node *criticalNode, struct btree *tree);
	rotation[0] = rotateRight, rotation[1] = leftRightRotation;
	rotation[2] = rotateLeft, rotation[3] = rightLeftRotation;
	int *rotate = findRotationType(criticalNode, child);
	if(rotate == NULL)
		return;
	rotation[*rotate](criticalNode, tree);
	free(rotate);
}

/*
 * Function: findCriticalNode
 * -----------------------------------------------------------------------
 * Summary: Finds the critical node given a pointer to the recently added
 * node that caused debalancing.
 * Parameters: A pointer to the recently added node.
 * Returns: A pointer to the critical node.
*/
struct node* findCriticalNode(struct node *currentNode, struct btree tree)
{
    if(abs(currentNode->balance) > 1 || currentNode == tree.root)
	return currentNode;

    return findCriticalNode(currentNode->parent, tree);
}

/*
 * Function: findRotationType
 * ------------------------------------------------------------------
 * Summary: Finds what rotation is required given a pointer to the
 * critical node and the recently added node that caused debalancing.
 * Parameters: A pointer to the critical node and recently added node.
 * Returns: An pointer to an int used for indexing the array of function 
 * pointers in rebuildAVLTree.
 * 0: Right rotation.
 * 1: Left-right rotation.
 * 2: Left rotation.
 * 3: Right-left rotation.
 * NULL: critical node given is not actually critical.
*/
int *findRotationType(struct node *criticalNode, struct node *child)
{
	int *key = malloc(sizeof(int)); //For indexing the array of function pointers in insertNode
	if(criticalNode->balance > 1)
	{
		*key = child->data <= criticalNode->left->data ? 0 : 1;
		return key;
	}
	else if(criticalNode->balance < -1)
	{
		*key = child->data >= criticalNode->right->data ? 2 : 3;
		return key;
	}
	return NULL;
}

/*
 * Function: updateHeightAndBalance
 * --------------------------------------------------------------------------
 * Summary: Updates the height and balance of the given node pointer.
 * Parameters: A pointer to the node to be updated
 * Returns: Nothing
*/
void updateHeightAndBalance(struct node *N)
{
    N->size = 1 + size(N->left) + size(N->right);
    N->balance = size(N->left) - size(N->right);
}
/*
 * Function: size
 * ------------------------------------------
 * Summary: Returns the size of the given node.
 * Parameters: A pointer to the node.
 * Returns: The size of the node, or 0 if pointer
 * is NULL.
*/ 
int size(struct node *N)
{
    if(N == NULL)
	return 0;
    return N->size;
}

/*
 * Function: balance
 * ------------------------------------------
 * Summary: Returns the balance of the given node.
 * Parameters: A pointer to the node.
 * Returns: The balance of the node, or 0 if pointer
 * is NULL.
*/ 
int balance(struct node *N)
{
    if(N == NULL)
	return 0;
    return N->balance;
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
