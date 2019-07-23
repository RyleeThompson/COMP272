/*
 ============================================================================
 Name        : A2Q1.c
 Author      : Rylee Thompson
 Version     :
 Copyright   :
 Description : Solution to Assignment 2 question 2: Design an algorithm for 
 the following operations for a binary tree BT, and show the worst-case 
 running times for each implementation:
 
 preorderNext(x): return the node visited after node x in a pre-order traversal of BT.

 postorderNext(x): return the node visited after node x in a post-order traversal of BT.

 inorderNext(x): return the node visited after node x in an in-order traversal of BT.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct node{
    int data;
    struct node* right;
    struct node* left;
};

struct btree{
    struct node *root;
};

void printResult(struct btree tree, int r);
void initalizeTree(struct btree *tree, int numNodes);
void fillTree(struct node *currentNode, int data, int currentLevel, int numLevels);
void print2DUtil(struct node *root, int space);
struct node* preorderNext(struct node *currentNode, struct btree tree, int x);
struct node* inorderNext(struct node *currentNode, struct btree tree, int x);
struct node* postorderNext(struct node *currentNode, struct btree tree, int x);
void freeTree(struct node *currentNode);

#define COUNT 10
#define maxNumNodes 15

int main(void)
{
    struct btree tree; //The binary tree struct
    int numNodes;
    srand(time(NULL));
    numNodes = (rand() % maxNumNodes) + 1; //Randomly choose number of nodes to be with 1-maxNumNodes
    initalizeTree(&tree, numNodes); //Fill the tree with the number of nodes using preorder traversal

    printf("The binary tree is:\n");
    print2DUtil(tree.root, 0); //Print the tree to stdout in 2D

    int i;
    int r;
    //Randomly choose a few numbers between 1-numNodes to test
    for(i = 0; i < 4; i++)
    {
	r = rand() % numNodes + 1;
	//Find preorderNext, inorderNext, and postorderNext for r and print it
	printResult(tree, r);
    }

    printf("\nThe worst case of these functions would be when we are asked to find the next"
           " node of the final node.\n");

    printf("In preorder traversal it would be when we are asked to find the next of the node"
           " with data 1\n");

    printf("In inorder traversal it is when we find the next of the node that is furthest to"
           " the right\n");

    printf("In postorder traversal it is when we find the next of the root of the tree (or %d)\n", tree.root->data);

    freeTree(tree.root);

    return 0;
}
/*
 * Function: printResult
 * ----------------------------------------------
 * Summary: Prints the node following the node containing the given data
 * when inorder, preorder, and postorder traversal is used.
 * Parameters: The tree to be searched, and the data of the node whose next node
 * is to be printed.
 * Returns: Nothing
*/
void printResult(struct btree tree, int r)
{
    char **traversalTypes;
    traversalTypes = malloc(sizeof(char*) * 3);

    traversalTypes[0] = "preorder";
    traversalTypes[1] = "inorder";
    traversalTypes[2] = "postorder";

    struct node *(*traversalFunction[3])(struct node *currentNode, struct btree tree, int data);
    traversalFunction[0] = preorderNext;
    traversalFunction[1] = inorderNext;
    traversalFunction[2] = postorderNext;

    putchar('\n');
    struct node *current;
    for(int i = 0; i < 3; i++)
    {
	current = traversalFunction[i](tree.root, tree, r);
	if(current != NULL)
	    printf("The node after %d in %s traversal is %d\n", r, traversalTypes[i], current->data);
	else
	    printf("%d is the last node in %s traversal\n", r, traversalTypes[i]);
    }

    free(traversalTypes);

}

/*
 * Function: initializeTree
 * -------------------------------
 * Summary: Initializes the given tree to have the given number of nodes
 * using preorder traversal
 * Parameters: A pointer to the tree to be initialized and the number of
 * nodes it should contain.
 * Returns: Nothing
 * Note: Modifies the given tree in place
*/
void initalizeTree(struct btree *tree, int numNodes)
{
    struct node *root = malloc(sizeof(struct node));
    root->data = numNodes;
    tree->root = root;
    //Fill the tree with given number of nodes
    fillTree(root, numNodes, 0, log(numNodes)/log(2)); //logs give the number of levels in the tree
}

/*
 * Function: fillTree
 * --------------------------------
 * Summary: Recursively fills the tree (starting at the given node) with the given
 * number of nodes. The first nodes data is the number of nodes, and each subsequent
 * node is decreased by one (preorder traversal).
 * Parameters: A pointer to the node to start at, the number of nodes to add, the
 * current level (should be 0), and the total number of levels to be in the tree.
 * Returns: Nothing
*/
void fillTree(struct node *currentNode, int data, int currentLevel, int numLevels)
{
    if(currentLevel < numLevels && data > 1)
    {
        currentNode->left = malloc(sizeof(struct node));
        currentNode->left->data = data - 1;

		//Recursive call for left side of tree with updated data and currentLevel
        fillTree(currentNode->left, data - 1, currentLevel + 1, numLevels);

	//Don't allocate memory if there is no space left for the right node
	currentNode->right = data - pow(2, numLevels - currentLevel) >= 1 ? malloc(sizeof(struct node)) : NULL;
    /*pow(2, numLevels - currentLevel) gives the number of nodes in the left side of the tree
	 *plus one, so data - pow(2, numLevels - currentLevel) tells us if there is space for
	 *the right node (and if so, what the value should be)
	*/
	if(currentNode->right != NULL)
        {
            currentNode->right->data = data - pow(2, numLevels - currentLevel);

			//Recursive call for right side of the tree
            fillTree(currentNode->right, data - pow(2, numLevels - currentLevel), currentLevel + 1, numLevels);
        }
    }
}

/*
 * Function: preorderNext
 * ------------------------------------------
 * Summary: Given the root of a tree, the tree itself, and a value to be found
 * in the tree, this function returns the next node in the tree when preorder
 * traversal is used.
 * Parameters: A pointer to the root of the tree, the tree itself, and the data
 * to be found.
 * Returns: A pointer to the next node (or NULL if the given data is not found
 * or is the last node in the tree)
*/
struct node* preorderNext(struct node *currentNode, struct btree tree, int x)
{
    static int found = 0;
    static struct node* next = NULL;

    //Clear the static variables for when this function is called multiple times in the same program
    found = currentNode == tree.root ? 0 : found;
    next = currentNode == tree.root ? NULL : next;

    //Next is equal to the current node when the data is found in the previous function call
    next = found == 1 && next == NULL && currentNode != NULL ? currentNode : next;
    if(currentNode != NULL && next == NULL)
    {
	found = currentNode->data == x ? 1 : found; //Visit node
	preorderNext(currentNode->left, tree, x); //Traverse left sub-tree
	preorderNext(currentNode->right, tree, x); //Traverse right sub-tree
    }
    return next;
}



/*
 * Function: inorderNext
 * ------------------------------------------
 * Summary: Given the root of a tree, the tree itself, and a value to be found
 * in the tree, this function returns the next node in the tree when inorder
 * traversal is used.
 * Parameters: A pointer to the root of the tree, the tree itself, and the data
 * to be found.
 * Returns: A pointer to the next node (or NULL if the given data is not found or
 * is the last node in the tree)
*/
struct node* inorderNext(struct node *currentNode, struct btree tree, int x)
{
    static int found = 0;
    static struct node* next = NULL;

    //Clear the static variables for when this function is called multiple times in the same program
    found = currentNode == tree.root ? 0 : found;
    next = currentNode == tree.root ? NULL : next;

    if(currentNode != NULL && next == NULL)
    {
	inorderNext(currentNode->left, tree, x); //Traverse left sub-tree
	found = currentNode->data == x ? 1 : found; //Visit node
	next = found == 1 && next == NULL && currentNode->data != x ? currentNode : next;
	inorderNext(currentNode->right, tree, x); //Traverse right sub-tree
    }
    return next;
}


/*
 * Function: postorderNext
 * ------------------------------------------
 * Summary: Given the root of a tree, the tree itself, and a value to be found
 * in the tree, this function returns the next node in the tree when postorder
 * traversal is used.
 * Parameters: A pointer to the root of the tree, the tree itself, and the data
 * to be found.
 * Returns: A pointer to the next node (or NULL when the given data is not found
 * or it is the last node in the tree)
*/

struct node* postorderNext(struct node *currentNode, struct btree tree, int x)
{
    static int found = 0;
    static struct node* next = NULL;

    //Clear the static variables for when the function is called multiple times in the same program
    found = currentNode == tree.root ? 0 : found;
    next = currentNode == tree.root ? NULL : next;

    if(currentNode != NULL && next == NULL)
    {
	postorderNext(currentNode->left, tree, x); //Traverse left sub-tree
	postorderNext(currentNode->right, tree, x); //Traverse right sub-tree
	found = currentNode->data == x ? 1 : found; //Visit node
	next = found == 1 && next == NULL && currentNode->data != x ? currentNode : next;
    }
    return next;
}

/*
 * Function: freeTree
 * ------------------------------------
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