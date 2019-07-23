/*
 ============================================================================
 Name        : A2Q5.c
 Author      : Rylee Thompson
 Version     :
 Copyright   :
 Description : Solution to Assignment 2 question 5: Create a subclass of 
 BinaryTree whose nodes have fields for storing preorder, post-order, and 
 in-order numbers. Write methods preOrderNumber(), inOrderNumber(), and 
 postOrderNumbers() that assign these numbers correctly. These methods should
 each run in O(n) time.
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

struct node{
    int data;
    struct node* right;
    struct node* left;
    int preorderNum;
    int inorderNum;
    int postorderNum;
};

struct btree{
    struct node *root;
};

void initalizeTree(struct btree *tree, int numNodes);
void fillTree(struct node *currentNode, int data, int currentLevel, int numLevels);
void print2DUtil(struct node *root, int space, char type[]);
void preorderNumber(struct node *currentNode, struct btree tree);
void inorderNumber(struct node *currentNode, struct btree tree);
void postorderNumber(struct node *currentNode, struct btree tree);
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
    print2DUtil(tree.root, 0, "data"); //Print the tree to stdout in 2D

    preorderNumber(tree.root, tree);
    printf("The binary tree indexing in preorder traversal:\n");
    print2DUtil(tree.root, 0, "pre");

    inorderNumber(tree.root, tree);
    printf("The binary tree indexing in inorder traversal:\n");
    print2DUtil(tree.root, 0, "in");

    postorderNumber(tree.root, tree);
    printf("The binary tree indexing in postorder traversal:\n");
    print2DUtil(tree.root, 0, "post");

    freeTree(tree.root);

    return 0;
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
 * Function: preorderNumber
 * ------------------------------------------
 * Summary: Given the root of a tree and the tree itself, this function assigns 
 * the correct indexing to each node when pre-order traversal is used.
 * Parameters: A pointer to the root of the tree, the tree itself, and the data
 * to be found.
 * Returns: Nothing
*/
void preorderNumber(struct node *currentNode, struct btree tree)
{
    static int counter = 0;

    //Clear the static counter for when this function is called multiple times in the same program
    counter = currentNode == tree.root ? 0 : counter;

    if(currentNode != NULL)
    {
	currentNode->preorderNum = counter; //Visit node
	counter++;
	preorderNumber(currentNode->left, tree); //Traverse left sub-tree
	preorderNumber(currentNode->right, tree); //Traverse right sub-tree
    }
    return;
}

/*
 * Function: inorderNumber
 * ------------------------------------------
 * Summary: Given the root of a tree and the tree itself, this function assigns 
 * the correct indexing to each node when in-order traversal is used.
 * Parameters: A pointer to the root of the tree, the tree itself, and the data
 * to be found.
 * Returns: Nothing
*/
void inorderNumber(struct node *currentNode, struct btree tree)
{
    static int counter = 0;

    //Clear the static counter for when this function is called multiple times in the same program
    counter = currentNode == tree.root ? 0 : counter;

    if(currentNode != NULL)
    {
	inorderNumber(currentNode->left, tree); //Traverse left sub-tree
	currentNode->inorderNum = counter; //Visit node
	counter++;
	inorderNumber(currentNode->right, tree); //Traverse right sub-tree
    }
    return;
}


/*
 * Function: postorderNumber
 * ------------------------------------------
 * Summary: Given the root of a tree and the tree itself, this function assigns 
 * the correct indexing to each node when post-order traversal is used.
 * Parameters: A pointer to the root of the tree, the tree itself, and the data
 * to be found.
 * Returns: Nothing
*/
void postorderNumber(struct node *currentNode, struct btree tree)
{
    static int counter = 0;

    //Clear the static counter for when this function is called multiple times in the same program
    counter = currentNode == tree.root ? 0 : counter;

    if(currentNode != NULL)
    {
	postorderNumber(currentNode->left, tree); //Traverse left sub-tree
	postorderNumber(currentNode->right, tree); //Traverse right sub-tree
	currentNode->postorderNum = counter; //Visit node
	counter++;
    }
    return;
}

/*
 * Function: freeTree
 * ------------------------------------
 * Summary: Frees a dynamically alocated tree starting at the given node
 * Parameters: The node to start free at
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
void print2DUtil(struct node *root, int space, char type[])
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(root->right, space, type);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    if(!strcmp(type, "data"))
    	printf("%d\n", root->data);
    else if(!strcmp(type, "pre"))
	printf("%d\n", root->preorderNum);
    else if(!strcmp(type, "in"))
	printf("%d\n", root->inorderNum);
    else if(!strcmp(type, "post"))
	printf("%d\n", root->postorderNum);

    // Process left child
    print2DUtil(root->left, space, type);
}