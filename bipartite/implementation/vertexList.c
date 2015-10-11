/*
 * vertexList.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "commonDefs.h"
#include "vertexList.h"
#include "memoryUtil.h"


void addVertex(vertexList_t *pList, int vertId)
{
	vlNode_t *pNewNode = (vlNode_t*) safeMalloc(sizeof(vlNode_t));
	pNewNode->vertId = vertId;
	/* if null, then first node.  If not null, then pNewNode becomes new head. */
	pNewNode->pNext = pList->pHead;
	pList->pHead = pNewNode;
	pList->size += 1;
} /** end of addNode() */



int deleteVertex(vertexList_t *pList, int vertId)
{
	vlNode_t *pCurrNode = pList->pHead, *pPrevNode = NULL;

	if (pCurrNode != NULL) {
		/** If head is the deleted node. */
		if (pCurrNode->vertId == vertId) {
			pList->pHead = pCurrNode->pNext;
			
			/* Delete the edges connected to the vertex*/
			if(pCurrNode->edges !=NULL){ 
				destroyList(pCurrNode->edges);
				safeFree(pCurrNode->edges, sizeof(linkedList_t));
			}
			
			safeFree(pCurrNode, sizeof(vlNode_t));
			pList->size -= 1;
			return SUCCESS;
		}

		pPrevNode = pCurrNode;
		pCurrNode = pCurrNode->pNext;
	}

	while (pCurrNode != NULL) {
		if (pCurrNode->vertId == vertId) {
			pPrevNode->pNext = pCurrNode->pNext;
			
			/* Delete the edges connected to the vertex*/
			if(pCurrNode->edges !=NULL){ 
				destroyList(pCurrNode->edges);
				pCurrNode->edges=NULL;
			}
			
			safeFree(pCurrNode, sizeof(vlNode_t));
			pList->size -= 1;
			return SUCCESS;
		}
		pPrevNode = pCurrNode;
		pCurrNode = pCurrNode->pNext;
	}

	/* Can't find vertId. */
	return FAILED;
} /* end of deleteNode() */

void destroyVList(vertexList_t *pList)
{
	vlNode_t *pCurrNode = pList->pHead;

	while (pCurrNode != NULL) {
		vlNode_t *pTempNode = pCurrNode->pNext;
		
		/* Delete the edges connected to the vertex*/
		if(pCurrNode->edges !=NULL){ 
			destroyList(pCurrNode->edges);
			pCurrNode->edges=NULL;
		}
		
		safeFree(pCurrNode, sizeof(vlNode_t));
		pCurrNode = NULL;
		pCurrNode = pTempNode;
	}

	pList->pHead = NULL;
	pList->size = 0;
	safeFree(pList, sizeof(vertexList_t));
} /* end of destroyList() */

vlNode_t* findVertex(vertexList_t *pList, int vertId)
{
	vlNode_t *pCurrNode = pList->pHead;

	while (pCurrNode != NULL) {
		if (pCurrNode->vertId == vertId) {
			return pCurrNode;
		}
		pCurrNode = pCurrNode->pNext;
	}

	/** Can't find vertId. */
	return NULL;
} /** end of findNode() */


