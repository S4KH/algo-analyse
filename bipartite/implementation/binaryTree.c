/*
 *    Copyright 2015, Jeffrey Chan.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "binaryTree.h"
#include "commonDefs.h"
#include "memoryUtil.h"
#include "linkedList.h"


/* ******************************************************************** */
/* Helper functions. */




/*
 * Find node with smallest element in tree.
 */
static
binTreeNode_t* findSmallest(binTreeNode_t *pTree, binTreeNode_t **pParent, int *bLeftChild);


/*
 * Swaps nodes pNode1 and pNode2 in the tree, and update the corresponding parent node pointers.
 */
static
void swapNodes(binTreeNode_t **ppTree, binTreeNode_t *pNode1, binTreeNode_t **pNode1Parent, int *bLeftChildNode1, binTreeNode_t *pNode2, binTreeNode_t **pNode2Parent, int *bLeftChildNode2);



/* ******************************************************************** */

binTreeNode_t* createTreeNode(int key, linkedList_t *value)
{
	binTreeNode_t *pNode = NULL;

	pNode = (binTreeNode_t*) safeMalloc(sizeof(binTreeNode_t));

	pNode->key = key;
	pNode->value = value;
	pNode->left = NULL;
	pNode->right = NULL;

	return pNode;
} /* end of createNode() */


/*
 * Destroy a tree.
 */
void destroyTree(binTreeNode_t *pTree)
{
	binTreeNode_t *pLeftChild = pTree->left, *pRightChild = pTree->right;

	if (pLeftChild != NULL) {
		destroyTree(pLeftChild);
	}

	if (pRightChild != NULL) {
		destroyTree(pRightChild);
	}

	destroyTreeNode(pTree);
	pTree = NULL;
} /* end of destroyTree() */


/*
 * Destroy a node
 */
void destroyTreeNode(binTreeNode_t *pNode)
{
	freeValueMemory(pNode->value);
	free(pNode);
} /* end of destroyNode() */


/*
 * Insert a value.
 * NOTE: For this assignment, we don't want duplicates.
 */
int insertTreeNode(binTreeNode_t *pTree, binTreeNode_t *pNewNode)
{
	int bLeftChild;
	/* TODO: please implement */

	binTreeNode_t* pCurrNode = pTree, *pParentNode = NULL;


	while (pCurrNode != NULL) {
		if (pNewNode->key == pCurrNode->key) {

			return FAILED;
		}
		/* see if traverse down left subtree */
		if (pNewNode->key < pCurrNode->key) {
			pParentNode = pCurrNode;
			bLeftChild = 1;
			pCurrNode = pCurrNode->left;
		}
		/* try right */
		else {
			pParentNode = pCurrNode;
			bLeftChild = 0;
			pCurrNode = pCurrNode->right;
		}
	}

	/* must be new node to insert */
	if (bLeftChild) {
		pParentNode->left = pNewNode;
	}
	else {
		pParentNode->right = pNewNode;
	}



	return SUCCESS;
} /* end of insertValue() */





/*
 * Delete the first node found with key.
 */
void deleteTreeNode(binTreeNode_t **ppTree, binTreeNode_t *pDelNode, binTreeNode_t *pDelParent, int bLeftChild)
{
	binTreeNode_t *leftChild, *rightChild;
	binTreeNode_t *pSmallest = NULL, *pSmallestParent = NULL;
	int bSmallestLeftChild = 0;

	if (pDelNode != NULL) {
		/* remove node then update subtrees */
		leftChild = pDelNode->left;
		rightChild = pDelNode->right;


		/* update */
		/* no children */
		if (leftChild == NULL && rightChild == NULL) {
			/* root node to be deleted */
			if (pDelNode == *ppTree) {
				destroyTreeNode(pDelNode);
				*ppTree = NULL;
			}
			/* not root node, so can just deallocate memory */
			else {
				destroyTreeNode(pDelNode);
				if (bLeftChild) {
					pDelParent->left = NULL;
				}
				else {
					pDelParent->right = NULL;
				}
			}
		}
		/* two children */
		else if (leftChild != NULL && rightChild != NULL) {
			pSmallest = findSmallest(rightChild, &pSmallestParent, &bSmallestLeftChild);
			/* exchange pSmallest with deleted node */
			swapNodes(ppTree, pDelNode, &pDelParent, &bLeftChild, pSmallest, &pSmallestParent, &bSmallestLeftChild);
			/* delete the pDelNode now */
			deleteTreeNode(ppTree, pDelNode, pDelParent, bLeftChild);
		}
		/* one child */
		else if (leftChild != NULL) {
			assert(rightChild == NULL);
			/* root node to be deleted */
			if (pDelNode == *ppTree) {
				destroyTreeNode(pDelNode);
				pDelNode = NULL;
				*ppTree = leftChild;
			}
			/* not root node */
			else {
				destroyTreeNode(pDelNode);
				if (bLeftChild) {
					pDelParent->left = leftChild;
				}
				else {
					pDelParent->right = leftChild;
				}
			}
		}
		else if (rightChild != NULL) {
			assert(leftChild == NULL);
			/* root node to be deleted */
			if (pDelNode == *ppTree) {
				destroyTreeNode(pDelNode);
				pDelNode = NULL;
				*ppTree = rightChild;
			}
			/* not root node */
			else {
				destroyTreeNode(pDelNode);
				if (bLeftChild) {
					pDelParent->left = rightChild;
				}
				else {
					pDelParent->right = rightChild;
				}
			}
		}
	}

} /* end of deleteNode() */


/*
 * Search for node with key.
 */
binTreeNode_t* searchValue(binTreeNode_t *pTree, int key)
{
    binTreeNode_t *pCurrNode = pTree;

	while (pCurrNode != NULL) {
		if (key == pCurrNode->key) {
			return pCurrNode;
		}
		/* see if traverse down left subtree */
		if (key < pCurrNode->key) {
			pCurrNode = pCurrNode->left;
		}
		/* try right */
		else {
			pCurrNode = pCurrNode->right;
		}
	}
	/* not found */
	return NULL;
} /* end of searchValue() */




void printTree(binTreeNode_t *pTree)
{
	if (pTree != NULL) {
		printf("Node key %d\n", pTree->key);
		printf("left:\n");
		printTree(pTree->left);
		printf("right:\n");
		printTree(pTree->right);
	}
}



/* ******************************************************************** */


binTreeNode_t* searchDeleteNode(binTreeNode_t *pTree, int key, binTreeNode_t **pParentNode, int *pLeftChild)
{
	binTreeNode_t *pCurrNode = pTree;

	while (pCurrNode != NULL) {
		if (key == pCurrNode->key) {
			return pCurrNode;
		}
		/* see if traverse down left subtree */
		if (key < pCurrNode->key) {
			*pParentNode = pCurrNode;
			*pLeftChild = 1;
			pCurrNode = pCurrNode->left;
		}
		/* try right */
		else {
			*pParentNode = pCurrNode;
			*pLeftChild = 0;
			pCurrNode = pCurrNode->right;
		}
	}

	/* not found */
	return NULL;
}



void swapNodes(binTreeNode_t **ppTree, binTreeNode_t *pNode1, binTreeNode_t **pNode1Parent, int *bLeftChildNode1, binTreeNode_t *pNode2, binTreeNode_t **pNode2Parent, int *bLeftChildNode2)
{
	binTreeNode_t *pTempLeftChild = pNode2->left, *pTempRightChild = pNode2->right;
	binTreeNode_t *pTempParent = *pNode2Parent;
	int bTempLeftChildNode = *bLeftChildNode2;

	/* we assume node 2 cannot be root */
	assert(*pNode2Parent != NULL);

	/* node 1 is root */
	if (*pNode1Parent == pNode1) {
		*ppTree = pNode2;
		pNode2->left = pNode1->left;
		pNode2->right = pNode1->right;

		pNode1->left = pTempLeftChild;
		pNode1->right = pTempRightChild;

		*pNode2Parent = *pNode1Parent;
		*pNode1Parent = pTempParent;

		*bLeftChildNode2 = *bLeftChildNode1;
		*bLeftChildNode1 = bTempLeftChildNode;
	}
	else {
		pNode2->left = pNode1->left;
		pNode2->right = pNode1->right;

		pNode1->left = pTempLeftChild;
		pNode1->right = pTempRightChild;

		if (*bLeftChildNode1) {
			(*pNode1Parent)->left = pNode2;
		}
		else {
			(*pNode1Parent)->right = pNode2;
		}

		if (*bLeftChildNode2) {
			(*pNode2Parent)->left = pNode1;
		}
		else {
			(*pNode2Parent)->right = pNode1;
		}

		/* swap the parent pointer themselves */
		*pNode2Parent = *pNode1Parent;
		*bLeftChildNode2 = *bLeftChildNode1;
		*pNode1Parent = pTempParent;
		*bLeftChildNode1 = bTempLeftChildNode;
	}
} /* end of swapNodes */


binTreeNode_t* findSmallest(binTreeNode_t *pTree, binTreeNode_t **ppSmallestParent, int *bSmallestLeftChild)
{
	binTreeNode_t *pCurrNode = pTree;
	*ppSmallestParent = NULL;

	while (pCurrNode->left != NULL) {
		*ppSmallestParent = pCurrNode;
		pCurrNode = pCurrNode->left;
	}

	/* always true */
	*bSmallestLeftChild = 1;

	return pCurrNode;
}




void freeValueMemory(linkedList_t *value)
{
    if(value!=NULL){
        destroyList(value);
    }
}
