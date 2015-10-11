/*
 * bpGraphAdjList_LL.c
 *
 *	Linked list-linked list implementation.
 *
 *  Created on: 01/07/2015
 *      Author: Jeffrey Chan
 */

#include <stdio.h>
#include <stddef.h>
#include "bipartiteGraph.h"
#include "vertexList.h"
#include "linkedList.h"
#include "commonDefs.h"
#include "memoryUtil.h"

struct implBipartGraph_t
{
	/* List to store partite 1 vertices */
	vertexList_t *vertices1;
	/* List to store partite 2 vertices */
	vertexList_t *vertices2;
};

/* Static functions */
static void findAndDelete(bpGraph_t *pGraph, int vertId, int partite);


/* ************************************************************************* */
/* Function implementations */


bpGraph_t* bipartGraphCreate(int part1VertNum, int part2VertNum)
{
	int i;

	/* initializing graph */
	bpGraph_t *pGraph = (bpGraph_t*) safeMalloc(sizeof(bpGraph_t));

	pGraph->vertices1 = (vertexList_t*)safeMalloc(sizeof(vertexList_t));
	pGraph->vertices2 = (vertexList_t*)safeMalloc(sizeof(vertexList_t));

	/* Adding vertices to the partite1 */
	for(i=0;i<part1VertNum;i++){
		bipartGraphInsertVertex(pGraph, i, 1);
	}

	/* Adding vertices to the partite2 */
	for(i=0;i<part2VertNum;i++){
		bipartGraphInsertVertex(pGraph, i, 2);
	}

	return pGraph;
} /* end of bipartGraphDestroy() */


void bipartGraphDestroy(bpGraph_t* pGraph)
{
	/* free the vertex list */
	destroyVList(pGraph->vertices1);
	destroyVList(pGraph->vertices2);
	/* free the graph itself */
	safeFree(pGraph, sizeof(bpGraph_t));
	/* make sure we can't accidentally use graph */
	pGraph = NULL;
} /* end of bipartGraphDestroy() */


int bipartGraphInsertVertex(bpGraph_t* pGraph, int vertId, int partite)
{
	if(partite ==1){
		/* Checking for vertex duplications */
		if((findVertex(pGraph->vertices1, vertId))==NULL){
			addVertex(pGraph->vertices1, vertId);
			return NEW_VERTEX;
		}
		else{
			return EXISTING_VERTEX;
		}
	}
	else if(partite == 2){
		/* Checking for vertex duplication */
		if((findVertex(pGraph->vertices2, vertId))==NULL){
			addVertex(pGraph->vertices2, vertId);
			return NEW_VERTEX;
		}
		else{
			return EXISTING_VERTEX;
		}

	}

	return ERROR_VALUE;
} /* end of bipartGraphInsertVertex() */


int bipartGraphInsertEdge(bpGraph_t* pGraph, int srcVertId, int tarVertId, int srcPartite)
{
	vlNode_t *srcVertex, *tarVertex;
	if (srcPartite == 1) {

		/* Finding the source and target vertices */
		srcVertex = findVertex(pGraph->vertices1, srcVertId);
		tarVertex = findVertex(pGraph->vertices2, tarVertId);

		/* Check if vertices exist */
		if(srcVertex!=NULL && tarVertex!=NULL){
			if(srcVertex->edges==NULL){
				srcVertex->edges = (linkedList_t*)safeMalloc(sizeof(linkedList_t));
				srcVertex->edges->pHead=NULL;
			}

			/* Need to check for duplicates */
			if (!findElement(srcVertex->edges, tarVertId)) {
				addNode(srcVertex->edges, tarVertId);
				return NEW_EDGE;
			}
			/* else must be existing edge */
			return EXISTING_EDGE;
		}
	}
	else if (srcPartite == 2) {

		/* Finding the source and target vertices */
		srcVertex = findVertex(pGraph->vertices2, srcVertId);
		tarVertex = findVertex(pGraph->vertices1, tarVertId);

		/* Check if vertices exist */
		if(srcVertex!=NULL && tarVertex!=NULL){
			if(srcVertex->edges == NULL){
				srcVertex->edges = (linkedList_t*)safeMalloc(sizeof(linkedList_t));
			}

			/* Need to check for duplicates */
			if (!findElement(srcVertex->edges, tarVertId)) {
				addNode(srcVertex->edges, tarVertId);
				return NEW_EDGE;
			}
			/* else must be existing edge */
			return EXISTING_EDGE;
		}
	}

	return ERROR_VALUE;
} /* end of bipartGraphInsertEdge() */


int bipartGraphDeleteVertex(bpGraph_t* pGraph, int vertId, int partite)
{
	int errorStatus;

	if (partite == 1) {

		/* delete linked list node */
		errorStatus = deleteVertex(pGraph->vertices1, vertId);

		if (errorStatus > 0) {

            findAndDelete(pGraph, vertId, partite);

			return FOUND;
		}

		/* vertex not in graph */
		return NOT_FOUND;
	}
	else if (partite == 2) {

		/* delete linked list node */
		errorStatus = deleteVertex(pGraph->vertices2, vertId);

		if (errorStatus > 0) {

            findAndDelete(pGraph, vertId, partite);

			return FOUND;
		}
		/* vertex not in graph */
		return NOT_FOUND;
	}

	return ERROR_VALUE;
} /* end of bipartGraphDeleteVertex() */


int bipartGraphDeleteEdge(bpGraph_t* pGraph,  int srcVertId, int tarVertId, int srcPartite)
{
	vlNode_t *srcVertex;
	int errorStatus;

	if (srcPartite == 1) {

		/* Find the vertex that is connected*/
		srcVertex = findVertex(pGraph->vertices1, srcVertId);

		if(srcVertex!=NULL){
			/* delete linked list node */
			errorStatus = deleteNode(srcVertex->edges, tarVertId);

			if (errorStatus > 0) {
				return FOUND;
			}
		}
		/* edge/vertex not in graph */
		return NOT_FOUND;

	}
	else if (srcPartite == 2) {

		/* Find the vertex that is connected*/
		srcVertex = findVertex(pGraph->vertices2, srcVertId);

		if(srcVertex!=NULL){
			/* delete linked list node */
			errorStatus = deleteNode(srcVertex->edges, tarVertId);

			if (errorStatus>0) {
				return FOUND;
			}
		}
		/* edge/vertex not in graph */
		return NOT_FOUND;
	}

	return ERROR_VALUE;
} /** end of bipartGraphDeleteEdge() */


int bipartGraphFindVertex(bpGraph_t *pGraph, int vertId, int partite)
{
	if (partite == 1) {
		if(findVertex(pGraph->vertices1, vertId)==NULL){
			return NOT_FOUND;
		}
		return FOUND;
	}
	if (partite == 2) {
		if(findVertex(pGraph->vertices2, vertId)==NULL){
			return NOT_FOUND;
		}
		return FOUND;
	}

	/* unknown partite */
	return ERROR_VALUE;
} /* end of bipartGraphFindVertex() */


int bipartGraphFindEdge(bpGraph_t* pGraph, int srcVertId, int tarVertId, int srcPartite)
{
	vlNode_t *srcVertex;
	if (srcPartite == 1) {

		/* Finding the right vertext from list */
		srcVertex = findVertex(pGraph->vertices1, srcVertId);

		if(srcVertex!=NULL){
			/* search for edge */
			return findElement(srcVertex->edges, tarVertId);
		}
		return NOT_FOUND;
	}
	else if (srcPartite == 2) {

		/* Finding the right vertext from list */
		srcVertex = findVertex(pGraph->vertices2, srcVertId);

		if(srcVertex!=NULL){
			/* search for edge */
			return findElement(srcVertex->edges, tarVertId);
		}
		return NOT_FOUND;
	}

	return ERROR_VALUE;
} /* end of bipartGraphFindEdge() */


void bipartGraphPrint(bpGraph_t *pGraph)
{
	vlNode_t *pCurrVertex = NULL;
	llNode_t *pCurrNode = NULL;

	/* print vertices */
	printf("Vertices:\n");
	printf("Part 1:\n");
	pCurrVertex = pGraph->vertices1->pHead;
	while (pCurrVertex != NULL) {
		printf("%d ", pCurrVertex->vertId);
		pCurrVertex = pCurrVertex->pNext;
	}

	printf("\nPart 2:\n");
	pCurrVertex = pGraph->vertices2->pHead;
	while (pCurrVertex != NULL) {
		printf("%d ", pCurrVertex->vertId);
		pCurrVertex = pCurrVertex->pNext;
	}
	printf("\n");

	/* print edges */
	printf("Edges:\n");
	/* partite 1 to partite 2 edges. */
	printf("Part 1 to 2:\n");
	pCurrVertex = pGraph->vertices1->pHead;

	/* Iterating over embedded linked list (edge) inside linked list(vertex) */
	while (pCurrVertex != NULL) {
		if(pCurrVertex->edges!=NULL){
			pCurrNode = pCurrVertex->edges->pHead;
			while(pCurrNode!=NULL){
				printf("%d %d\n", pCurrVertex->vertId, pCurrNode->element);
				pCurrNode = pCurrNode->pNext;
			}
		}
		pCurrVertex = pCurrVertex->pNext;
	}

	printf("Part 2 to 1:\n");
	pCurrVertex = pGraph->vertices2->pHead;

	/* Iterating over embedded linked list(edge) inside linked list(vertex) */
	while (pCurrVertex != NULL) {
		if(pCurrVertex->edges!=NULL){
			pCurrNode = pCurrVertex->edges->pHead;
			while(pCurrNode!=NULL){
				printf("%d %d\n", pCurrVertex->vertId, pCurrNode->element);
				pCurrNode = pCurrNode->pNext;
			}
		}
		pCurrVertex = pCurrVertex->pNext;
	}

} /* end of bipartGraphPrint() */

void findAndDelete(bpGraph_t *pGraph, int vertId, int partite)
{
	vlNode_t *pCurrNode = NULL;

	/* search for all elements in in-neighbourhood. */
	if (partite == 1) {
        pCurrNode = pGraph->vertices1->pHead;
	}
	else if (partite == 2) {
        pCurrNode = pGraph->vertices2->pHead;
	}
	while (pCurrNode != NULL) {

		/* Delete from the edge list */
		deleteNode(pCurrNode->edges, vertId);
		pCurrNode = pCurrNode->pNext;
	}
} /* end of findAndDelete() */
