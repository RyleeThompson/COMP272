/*
 ============================================================================
 Name        : A2Q2.c
 Author      : Rylee Thompson
 Version     :
 Copyright   :
 Description : Solution to Assignment 2 question 2: Design a recursive 
 linear-time algorithm that tests whether a binary tree satisfies the search
 tree order property at every node.
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
};

struct btree{
    struct node *root;
};

void initializeBST(struct btree *tree, int numNodes);
void fillBST(struct node *currentNode, struct btree tree, int numNodes, int data);
void print2DUtil(struct node *root, int space);
void addNode(struct node *currentNode, int data);
void initializeTree(struct btree *tree, int numNodes);
void fillTree(struct node *currentNode, int data, int currentLevel, int numLevels);
int checkBST(struct node *currentNode, struct btree tree);
void freeTree(struct node *currentNode);

#define COUNT 10
#define maxNumNodes 15

int main(void)
{
    struct btree bst; //The binary search tree
    struct btree nonBST; //The non binary search tree
    int numNodes;
    srand(time(NULL));
    numNodes = (rand() % maxNumNodes) + 1; //Randomly choose number of nodes to be with 1-maxNumNodes

    initializeBST(&bst, numNodes); //Fill the bst with the number of nodes
    printf("The randomized binary search tree is:\n");
    print2DUtil(bst.root, 0); //Print the bst to stdout in 2D

    initializeTree(&nonBST, numNodes); //Fill the nonBST with the number of nodes using preorder traversal
    printf("The non bst is:\n");
    print2DUtil(nonBST.root, 0); //Print the nonBST to stdout in 2D

    printf("bst check: %d\n", checkBST(bst.root, bst)); //Check if the created bst satisfies bst properties
    printf("nonbst check: %d\n", checkBST(nonBST.root, nonBST)); //Check if the nonBST is a bst

    freeTree(bst.root);
    freeTree(nonBST.root);

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
    fillBST(root, *tree, numNodes - 1, rand() % 100);
}

/*
 * Function: fillBST
 * --------------------------------
 * Summary: Recursively fills the BST (starting at the given node) with the given
 * number of nodes. The nodes data are a random number between 0-100, and are placed
 * in the tree to satisfy BST properties.
 * Parameters: A pointer to the node to start at, the tree itself, the number of nodes to add,
 * and the data the first node should contain.
 * Returns: Nothing
*/
void fillBST(struct node *currentNode, struct btree tree, int numNodes, int data)
{
    if(numNodes <= 0) //Base case
	return;

    //Process left child
    if(data < currentNode->data)
    {
        if(currentNode->left == NULL)
	{
	    addNode(currentNode, data); //Insert left child node
	    fillBST(tree.root, tree, numNodes - 1, rand() % 100); //Restart recursion with new data
	}
	else
	    fillBST(currentNode->left, tree, numNodes, data);
    }
    //Process right child
    else if(data > currentNode->data)
    {
	if(currentNode->right == NULL)
	{
	    addNode(currentNode, data); //Insert right child node
	    fillBST(tree.root, tree, numNodes - 1, rand() % 100); //Restart recursion with new data
	}
	else
            fillBST(currentNode->right, tree, numNodes, data);
    }
    //Restart recursion with new data if the data is already in the tree
    else
	fillBST(tree.root, tree, numNodes, rand() % 100);
}

/*
 * Function: addNode
 * ---------------------------------------
 * Summary: Adds a child with the given data to the given node such
 * that is satisfies bst properties.
 * Parameters: A pointer to the node, and the data that the nodes child
 * should contain.
 * Returns: Nothing
*/
void addNode(struct node *currentNode, int data)
{
    struct node *newNode;
    newNode = malloc(sizeof(struct node));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    currentNode->left = data < currentNode->data ? newNode : currentNode->left;
    currentNode->right = data > currentNode->data ? newNode : currentNode->right;
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
void initializeTree(struct btree *tree, int numNodes)
{
    struct node *root = malloc(sizeof(struct node));
    root->data = numNodes;
    tree->root = root;
    //Fill the tree with the given number of nodes
    fillTree(root, numNodes, 0, log(numNodes)/log(2));
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
    if(currentLevel >= numLevels || data <= 1) //Base case
        return;

    currentNode->left = malloc(sizeof(struct node));
    currentNode->left->data = data - 1;
    //Recursive call for left side of tree with updated data and currentLevel
    fillTree(currentNode->left, data - 1, currentLevel + 1, numLevels);

    //Don't allocate memory if there is no space left for the right node
    currentNode->right = data - pow(2, numLevels - currentLevel) >= 1 ? malloc(sizeof(struct node)) : NULL;
    if(currentNode->right != NULL)
    {
        currentNode->right->data = data - pow(2, numLevels - currentLevel);
        //Recursive call for right side of the tree
        fillTree(currentNode->right, data - pow(2, numLevels - currentLevel), currentLevel + 1, numLevels);
    }
}

/*
 * Function: checkBST
 * ---------------------------------
 * Summary: Recursively checks if a tree satisfies bst properties starting
 * at the given node.
 * Parameters: A pointer to the node to start at (root of the tree),
 * and the tree itself.
 * Returns: 1 if it satisfies bst properties and 0 otherwise.
*/
int checkBST(struct node *currentNode, struct btree tree)
{
    static int check = 1;

    //Clear static variable for when this function is called multiple times in same program
    check = currentNode == tree.root ? 1 : check;

    if(check == 0) //Base case
	return check;

    if(currentNode->left != NULL)
    {
	check = currentNode->left->data < currentNode->data ? check : 0;
	//Process left child
	checkBST(currentNode->left, tree);
    }
    if(currentNode->right != NULL)
    {
	check = currentNode->right->data > currentNode->data ? check : 0;
	//Process right child
	checkBST(currentNode->right, tree);
    }
    return check;
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
