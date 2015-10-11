/*
 *    Copyright 2015, Jeffrey Chan.
 */

#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_



#include <stdlib.h>
#include <string.h>
#include "linkedList.h"


struct binTreeNode;
typedef struct binTreeNode binTreeNode_t;

/*
 * Binary tree node data type.
 */
struct binTreeNode {
	int key;
	/* pointer to the value that the node holds */
	linkedList_t *value;
	/* Left and right children subtrees */
	binTreeNode_t *left, *right;
};





/*
 * Create a tree.
 */
binTreeNode_t* createTreeNode(int key, linkedList_t *value);


/*
 * Destroy a tree.
 */
void destroyTree(binTreeNode_t *tree);


/*
 * Destroy a node
 */
void destroyTreeNode(binTreeNode_t *node);


/*
 * Insert a value.
 * NOTE: Duplicates are allowed.
 */
int insertTreeNode(binTreeNode_t *pTree, binTreeNode_t *pNewNode);


/*
 * Delete the first node found with value.
 */
void deleteTreeNode(binTreeNode_t **ppTree, binTreeNode_t *pDelNode, binTreeNode_t *pDelParent, int bleftChild);



/*
 * Find node with key and return it
 */
binTreeNode_t* searchValue(binTreeNode_t *pTree, int key);

/*
 * Find node with key, and also return by reference that node's parent and whether it is a left or right child of the parent node.
 */
binTreeNode_t* searchDeleteNode(binTreeNode_t *pTree, int key, binTreeNode_t **pParent, int *pLeftChild);


/*
 * Display the tree using preorder traversal.
 */
void printTree(binTreeNode_t *ppTree);






/* TODO: Please implement if using the above tree structure and code. */
/*
 * Free memory for the node's value.
 */
void freeValueMemory(linkedList_t *value);




#endif

