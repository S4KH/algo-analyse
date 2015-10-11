/*
 * bipartGraphAdjList.c
 *
 * Array-linked list implementation of the adjacency list representation.
 *
 *  Created on: 01/07/2015
 *      Author: Jeffrey Chan
 */


#include <stdio.h>
#include <stdlib.h>
#include "commonDefs.h"
#include "bipartiteGraph.h"
#include "linkedList.h"
#include "memoryUtil.h"


/*
 * Implementation of the bipartite graph type for this data structure.
 */
struct implBipartGraph_t
{
	/** Vertex number of partite 1. */
	int vertNum1;
	/** Vertex number of partite 2. */
	int vertNum2;

	/* Dynamically allocated array of linked lists of neighbours for partite 1. */
	linkedList_t **vpVertsP1;
	/* Dynamically allocated array of linked lists of neighbours for partite 2. */
	linkedList_t **vpVertsP2;

	/* Dynamically allocated array of chars to indicate if vertex exists. */
	char *vVertExistP1;
	char *vVertExistP2;
};


/* *********************************************************************** */
/* Static functions */

static
void findAndDelete(bpGraph_t *pGraph, int vertId, int partite);


/* ************************************************************************* */
/* Function implementations */


bpGraph_t* bipartGraphCreate(int vertNumPart1, int vertNumPart2)
{
	int i;

	bpGraph_t *pGraph = (bpGraph_t*) safeMalloc(sizeof(bpGraph_t));
	pGraph->vertNum1 = vertNumPart1;
	pGraph->vertNum2 = vertNumPart2;

	/* initialise arrays */
	pGraph->vpVertsP1 = (linkedList_t**) safeMalloc(vertNumPart1 * sizeof(linkedList_t*));
	pGraph->vVertExistP1 = (char *) safeMalloc(vertNumPart1 * sizeof(char));
	/* initialise all elements to null for neighbours and true for existance */
	for (i = 0; i < pGraph->vertNum1; ++i) {
		pGraph->vpVertsP1[i] = createList();
		pGraph->vVertExistP1[i] = 1;
	}

	pGraph->vpVertsP2 = (linkedList_t**) safeMalloc(vertNumPart2 * sizeof(linkedList_t*));
	pGraph->vVertExistP2 = (char *) safeMalloc(vertNumPart2 * sizeof(char));
	/* initialise all elements to null */
	for (i = 0; i < pGraph->vertNum2; ++i) {
		pGraph->vpVertsP2[i] = createList();
		pGraph->vVertExistP2[i] = 1;
	}

	return pGraph;
} /* end of bipartGraphCreate() */


void bipartGraphDestroy(bpGraph_t* pGraph)
{
	int i;

	/* free the existence arrays */
	safeFree(pGraph->vVertExistP1, sizeof(char) * pGraph->vertNum1);
	safeFree(pGraph->vVertExistP2, sizeof(char) * pGraph->vertNum2);

	/* free the linked lists */
	for (i = 0; i < pGraph->vertNum1; ++i) {
		if (pGraph->vpVertsP1[i] != NULL) {
			destroyList(pGraph->vpVertsP1[i]);
		}
	}

	for (i = 0; i < pGraph->vertNum2; ++i) {
		if (pGraph->vpVertsP2[i] != NULL) {
			destroyList(pGraph->vpVertsP2[i]);
		}
	}

	/* free the arrays */
	safeFree(pGraph->vpVertsP1, sizeof(linkedList_t*) * pGraph->vertNum1);
	safeFree(pGraph->vpVertsP2, sizeof(linkedList_t*) * pGraph->vertNum2);
	/* free the graph itself */
	safeFree(pGraph, sizeof(bpGraph_t));
	/* make sure we can't accidentally use graph */
	pGraph = NULL;
} /* end of bipartGraphDestroy() */



int bipartGraphInsertVertex(bpGraph_t* pGraph, int vertId, int partite)
{
	int i;

	/* insert into partite 1 */
	if (partite == 1) {
		/* check if 'vertid' is an existing vertex */
		if (vertId < pGraph->vertNum1) {
			if (pGraph->vVertExistP1[vertId]) {
				return EXISTING_VERTEX;
			}
			else {
				pGraph->vVertExistP1[vertId] = 1;
				return NEW_VERTEX
			}
		}
		else {
			/* must be a new vertex, so we need to allocate more space */
			linkedList_t **pTemp = (linkedList_t**) safeRealloc(pGraph->vpVertsP1, (vertId+1) * sizeof(linkedList_t*), (vertId+1-pGraph->vertNum1) * sizeof(linkedList_t*));
			pGraph->vpVertsP1 = pTemp;
			char *pTemp2 = (char*) safeRealloc(pGraph->vVertExistP1, (vertId+1) * sizeof(char), (vertId+1-pGraph->vertNum1) * sizeof(char));
			pGraph->vVertExistP1 = pTemp2;
			/* initialise all new elements to NULL */
			for (i = pGraph->vertNum1; i < vertId+1; ++i) {
				pGraph->vpVertsP1[i] = NULL;
				pGraph->vVertExistP1[i] = 0;
			}
			pGraph->vpVertsP1[vertId] = createList();
			pGraph->vVertExistP1[vertId] = 1;

			pGraph->vertNum1 = vertId+1;
			return NEW_VERTEX;
		}
	}
	/* insert into partite 1 */
	else if (partite == 2) {
		/* check if 'vertid' is an existing vertex */
		if (vertId < pGraph->vertNum2) {
			if (pGraph->vVertExistP2[vertId]) {
				return EXISTING_VERTEX;
			}
			else {
				pGraph->vVertExistP2[vertId] = 1;
				return NEW_VERTEX
			}
		}
		else {
			/* must be a new vertex, so we need to allocate more space */
			linkedList_t **pTemp = (linkedList_t**) safeRealloc(pGraph->vpVertsP2, (vertId+1) * sizeof(linkedList_t*), (vertId+1-pGraph->vertNum2) * sizeof(linkedList_t*));
			pGraph->vpVertsP2 = pTemp;
			char *pTemp2 = (char*) safeRealloc(pGraph->vVertExistP2, (vertId+1) * sizeof(char), (vertId+1-pGraph->vertNum2) * sizeof(char));
			pGraph->vVertExistP2 = pTemp2;
			/* initialise all new elements to NULL */
			for (i = pGraph->vertNum2; i < vertId+1; ++i) {
				pGraph->vpVertsP2[i] = NULL;
				pGraph->vVertExistP2[i] = 0;
			}
			pGraph->vpVertsP2[vertId] = createList();
			pGraph->vVertExistP2[vertId] = 1;

			pGraph->vertNum2 = vertId+1;
			return NEW_VERTEX;
		}
	}

	/* Error, unknown partite! */
	return ERROR_VALUE;
} /* end of bipartGraphInsertVertex() */



int bipartGraphInsertEdge(bpGraph_t* pGraph, int srcVertId, int tarVertId, int srcPartite)
{
	if (srcPartite == 1) {
		/* The requested addition should be within the vertices id limits. */
		if (srcVertId >= pGraph->vertNum1 || tarVertId >= pGraph->vertNum2 || !pGraph->vVertExistP1[srcVertId] || !pGraph->vVertExistP2[tarVertId]) {
			return ERROR_VALUE;
		}

		/* Need to check for duplicates */
		if (!findElement(pGraph->vpVertsP1[srcVertId], tarVertId)) {
			addNode(pGraph->vpVertsP1[srcVertId], tarVertId);
			return NEW_EDGE;
		}
		/* else must be existing edge */
		return EXISTING_EDGE;
	}
	else if (srcPartite == 2) {
		/* The requested addition should be within the vertices id limits. */
		if (srcVertId >= pGraph->vertNum2 || tarVertId >= pGraph->vertNum1 || !pGraph->vVertExistP2[srcVertId] || !pGraph->vVertExistP1[tarVertId]) {
			return ERROR_VALUE;
		}

		/* Need to check for duplicates */
		if (!findElement(pGraph->vpVertsP2[srcVertId], tarVertId)) {
			addNode(pGraph->vpVertsP2[srcVertId], tarVertId);
			return NEW_EDGE;
		}
		/* else must be existing edge */
		return EXISTING_EDGE;
	}

	return ERROR_VALUE;
} /* end of bipartGraphInsertEdge() */




int bipartGraphDeleteVertex(bpGraph_t* pGraph, int vertId, int partite)
{
	if (partite == 1) {
		/* check if vertex exists */
		if (pGraph->vVertExistP1[vertId]) {
			/* delete neighbours */
			destroyList(pGraph->vpVertsP1[vertId]);
			pGraph->vpVertsP1[vertId] = NULL;
			pGraph->vVertExistP1[vertId] = 0;

			/* search the in neighbours and delete them */
			findAndDelete(pGraph, vertId, 2);

			return FOUND;
		}

		/* vertex not in graph */
		return NOT_FOUND;
	}
	else if (partite == 2) {
		/* check if vertex exists */
		if (pGraph->vVertExistP2[vertId]) {
			/* delete neighbours */
			destroyList(pGraph->vpVertsP2[vertId]);
			pGraph->vpVertsP2[vertId] = NULL;
			pGraph->vVertExistP2[vertId] = 0;

			/* search the in neighbours and delete them */
			findAndDelete(pGraph, vertId, 1);

			return FOUND;
		}

		return NOT_FOUND;
	}

	/** Wrong partite. */
	return ERROR_VALUE;
} /* end of bipartGraphDeleteVertex() */


int bipartGraphDeleteEdge(bpGraph_t* pGraph, int srcVertId, int tarVertId, int srcPartite)
{
	int errorStatus;

	if (srcPartite == 1) {
		/* The requested addition should be within the vertices id limits. */
		if (srcVertId >= pGraph->vertNum1 || tarVertId >= pGraph->vertNum2 || !pGraph->vVertExistP1[srcVertId] || !pGraph->vVertExistP2[tarVertId]) {
			return ERROR_VALUE;
		}

		/* delete linked list node */
		errorStatus = deleteNode(pGraph->vpVertsP1[srcVertId], tarVertId);

		if (errorStatus > 0) {
			return NOT_FOUND;
		}

		/* vertex not in graph */
		return FOUND;
	}
	else if (srcPartite == 2) {
		/* The requested addition should be within the vertices id limits. */
		if (srcVertId >= pGraph->vertNum2 || tarVertId >= pGraph->vertNum1 || !pGraph->vVertExistP2[srcVertId] || !pGraph->vVertExistP1[tarVertId]) {
			return ERROR_VALUE;
		}

		/* delete linked list node */
		errorStatus = deleteNode(pGraph->vpVertsP2[srcVertId], tarVertId);

		if (!errorStatus) {
			return NOT_FOUND;
		}

		return FOUND;
	}

	return ERROR_VALUE;
} /* end of bipartGraphDeleteEdge() */


int bipartGraphFindVertex(bpGraph_t *pGraph, int vertId, int partite)
{
	if (partite == 1) {
		if (vertId >= pGraph->vertNum1) {
			return NOT_FOUND;
		}

		return FOUND;
	}
	if (partite == 2) {
		if (vertId >= pGraph->vertNum2) {
			return NOT_FOUND;
		}

		return FOUND;
	}

	/* unknown partite */
	return ERROR_VALUE;
} /* end of bipartGraphFindVertex() */


int bipartGraphFindEdge(bpGraph_t* pGraph, int srcVertId, int tarVertId, int srcPartite)
{
	/* check if vertices exists */
	if (srcPartite == 1) {
		if (srcVertId >= pGraph->vertNum1 || tarVertId >= pGraph->vertNum2 || !pGraph->vVertExistP1[srcVertId] || !pGraph->vVertExistP2[tarVertId]) {
			return NOT_FOUND;
		}

		/* search for edge */
		return findElement(pGraph->vpVertsP1[srcVertId], tarVertId);
	}
	else if (srcPartite == 2) {
		if (srcVertId >= pGraph->vertNum2 || tarVertId >= pGraph->vertNum1 || !pGraph->vVertExistP2[srcVertId] || !pGraph->vVertExistP1[tarVertId]) {
			return NOT_FOUND;
		}

		/* search for edge */
		return findElement(pGraph->vpVertsP2[srcVertId], tarVertId);
	}

	return ERROR_VALUE;
} /* end of bipartGraphFindEdge() */



void bipartGraphPrint(bpGraph_t *pGraph)
{
	int i;
	llNode_t *pCurrNode = NULL;

	/* print vertices */
	printf("Vertices:\n");
	printf("Part 1:\n");
	for (i = 0; i < pGraph->vertNum1; ++i) {
		if (pGraph->vVertExistP1[i]) {
			printf("%d ", i);
		}
	}
	printf("\n");

	printf("Part 2:\n");
	for (i = 0; i < pGraph->vertNum2; ++i) {
		if (pGraph->vVertExistP2[i]) {
			printf("%d ", i);
		}
	}
	printf("\n");

	/* print edges */
	printf("Edges:\n");
	/* partite 1 to partite 2 edges. */
	printf("Part 1 to 2:\n");
	for (i = 0; i < pGraph->vertNum1; ++i) {
		if (pGraph->vpVertsP1[i] != NULL) {
			/* TODO: You are welcome to encapsulate this by creating a new printing function. */
			pCurrNode = pGraph->vpVertsP1[i]->pHead;

			while (pCurrNode != NULL) {
				printf("%d %d\n", i, pCurrNode->element);
				pCurrNode = pCurrNode->pNext;
			}
		}
	}

	/* partite 2 to partite 1 edges. */
	printf("Part 2 to 1:\n");
	for (i = 0; i < pGraph->vertNum2; ++i) {
		if (pGraph->vpVertsP2[i] != NULL) {
			/* TODO: You are welcome to encapsulate this by creating a new printing function. */
			pCurrNode = pGraph->vpVertsP2[i]->pHead;

			while (pCurrNode != NULL) {
				printf("%d %d\n", i, pCurrNode->element);
				pCurrNode = pCurrNode->pNext;
			}
		}
	}
} /* end of bipartGraphPrint() */



/* ********************************************************** */

void findAndDelete(bpGraph_t *pGraph, int vertId, int partite)
{
	int i;

	/* search for all elements in in-neighbourhood. */
	if (partite == 1) {
		for (i = 0; i < pGraph->vertNum1; ++i) {
			if (pGraph->vVertExistP1[i]) {
				deleteNode(pGraph->vpVertsP1[i], vertId);
			}
		}
	}
	else if (partite == 2) {
		for (i = 0; i < pGraph->vertNum2; ++i) {
			if (pGraph->vVertExistP2[i]) {
				deleteNode(pGraph->vpVertsP2[i], vertId);
			}
		}
	}
} /* end of findAndDelete() */




