/*
 * linkedList.c
 *
 *  Created on: 01/07/2015
 *      Author: jkct440
 */

#include <stdio.h>
#include <stdlib.h>
#include "commonDefs.h"
#include "linkedList.h"
#include "memoryUtil.h"


linkedList_t* createList()
{
	linkedList_t *pNewList = (linkedList_t*) safeMalloc(sizeof(linkedList_t));
	pNewList->size = 0;
	pNewList->pHead = NULL;

	return pNewList;
} /* end of createList() */



void destroyList(linkedList_t *pList)
{
	llNode_t *pCurrNode = pList->pHead;

	while (pCurrNode != NULL) {
		llNode_t *pTempNode = pCurrNode->pNext;
		safeFree(pCurrNode, sizeof(llNode_t));
		pCurrNode = NULL;
		pCurrNode = pTempNode;
	}

	pList->pHead = NULL;
	pList->size = 0;
	safeFree(pList, sizeof(linkedList_t));
} /* end of destroyList() */


void addNode(linkedList_t *pList, int element)
{
	llNode_t *pNewNode = (llNode_t*) safeMalloc(sizeof(llNode_t));
	pNewNode->element = element;
	/* if null, then first node.  If not null, then pNewNode becomes new head. */
	pNewNode->pNext = pList->pHead;
	pList->pHead = pNewNode;
	pList->size += 1;
} /** end of addNode() */



int deleteNode(linkedList_t *pList, int element)
{
	llNode_t *pCurrNode = pList->pHead, *pPrevNode = NULL;

	if (pCurrNode != NULL) {
		/** If head is the deleted node. */
		if (pCurrNode->element == element) {
			pList->pHead = pCurrNode->pNext;
			safeFree(pCurrNode, sizeof(llNode_t));
			pList->size -= 1;
			return SUCCESS;
		}

		pPrevNode = pCurrNode;
		pCurrNode = pCurrNode->pNext;
	}

	while (pCurrNode != NULL) {
		if (pCurrNode->element == element) {
			pPrevNode->pNext = pCurrNode->pNext;
			safeFree(pCurrNode, sizeof(llNode_t));
			return SUCCESS;
		}
		pPrevNode = pCurrNode;
		pCurrNode = pCurrNode->pNext;
	}

	/* Can't find element. */
	return FAILED;
} /* end of deleteNode() */



int findElement(linkedList_t *pList, int element)
{
	llNode_t *pCurrNode = pList->pHead;

	while (pCurrNode != NULL) {
		if (pCurrNode->element == element) {
			return FOUND;
		}
		pCurrNode = pCurrNode->pNext;
	}

	/** Can't find element. */
	return NOT_FOUND;
} /** end of findNode() */



void printList(linkedList_t *pList)
{
	llNode_t *pCurrNode = pList->pHead;

	while (pCurrNode != NULL) {
		printf("%d ", pCurrNode->element);
		pCurrNode = pCurrNode->pNext;
	}
	printf("\n");

} /* end of printList() */

