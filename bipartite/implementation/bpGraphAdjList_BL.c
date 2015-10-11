/*
 * bpGraphAdjList_BL.c
 *
 * Double Linked list implementation of an adjacency list representation.
 *
 *  Created on: 01/07/2015
 *      Author: Jeffrey Chan

 *  represent the partite vertices using binary tree
 */


#include <stdio.h>
#include <stdlib.h>
#include "commonDefs.h"
#include "bipartiteGraph.h"
#include "linkedList.h"
#include "memoryUtil.h"
#include "binaryTree.h"


#include <stddef.h>


struct implBipartGraph_t
{
    /* List to store partite 1 vertices */
	binTreeNode_t **vertices1;
	/* List to store partite 2 vertices */
	binTreeNode_t **vertices2;
};

/* Static functions */

static void preorder(binTreeNode_t *node);

static
void findAndDelete(binTreeNode_t *temp, int vertId);

/* ************************************************************************* */
/* Function implementations */


bpGraph_t* bipartGraphCreate(int part1VertNum, int part2VertNum)
{
    int i;

	/* initializing graph */
	bpGraph_t *pGraph = (bpGraph_t*) safeMalloc(sizeof(bpGraph_t));

	/* initializing tree for each vertices */
    pGraph->vertices1 = (binTreeNode_t**)safeMalloc(sizeof(binTreeNode_t*));
    pGraph->vertices2 = (binTreeNode_t**)safeMalloc(sizeof(binTreeNode_t*));

	/* Adding vertex to the partite1 */
	for(i=0;i<part1VertNum;i++){
		bipartGraphInsertVertex(pGraph, i, 1);
	}

	/* Adding vertex to the partite2 */
	for(i=0;i<part2VertNum;i++){
		bipartGraphInsertVertex(pGraph, i, 2);
	}

	return pGraph;
} /* end of bipartGraphDestroy() */


void bipartGraphDestroy(bpGraph_t* pGraph)
{
    /* Destroy each partite tree */
    destroyTree(*pGraph->vertices1);
    destroyTree(*pGraph->vertices2);

    /* Free the graph struct */
    safeFree(pGraph, sizeof(pGraph));
} /* end of bipartGraphDestroy() */


int bipartGraphInsertVertex(bpGraph_t* pGraph, int vertId, int partite)
{
    int status;
    binTreeNode_t *vertex;

	if(partite==1){
        vertex=searchValue(*pGraph->vertices1, vertId);

        /* Check if vertex already exists */
        if(vertex==NULL){

            if(*pGraph->vertices1==NULL){
                *pGraph->vertices1 = createTreeNode(vertId, NULL);

                return NEW_VERTEX;
            }
            else{
                /* initialise new node */
                binTreeNode_t *newNode = createTreeNode(vertId, NULL);

                status = insertTreeNode(*pGraph->vertices1,newNode);

                return status;
            }
        }

        return EXISTING_VERTEX;
	}else if(partite==2){
        vertex=searchValue(*pGraph->vertices2, vertId);

        /* Check if vertex already exists */
        if(vertex==NULL){

            if(*pGraph->vertices2==NULL){
                *pGraph->vertices2 = createTreeNode(vertId, NULL);

                return NEW_VERTEX;
            }
            else{
                /* initialise new node */
                binTreeNode_t *newNode = createTreeNode(vertId, NULL);

                status = insertTreeNode(*pGraph->vertices2,newNode);

                return status;
            }
        }

        return EXISTING_VERTEX;
	}

	return ERROR_VALUE;
} /* end of bipartGraphInsertVertex() */


int bipartGraphInsertEdge(bpGraph_t* pGraph, int srcVertId, int tarVertId, int srcPartite)
{
    binTreeNode_t *src, *tar;

    if(srcPartite == 1){

        /* Find source and target vertices */
        src = searchValue(*pGraph->vertices1, srcVertId);
        tar = searchValue(*pGraph->vertices2, tarVertId);

        /* Check if vertices exist */
        if(src!=NULL && tar !=NULL){

            /* Check if it is the first edge*/
            if(src->value==NULL){

                /*Initialise list and assign*/
                src->value = (linkedList_t*)safeMalloc(sizeof(linkedList_t));
                src->value->pHead=NULL;
                addNode(src->value, tarVertId);

                return NEW_EDGE;
            }else{

                /*Check if edge already exists*/
                if(!findElement(src->value, tarVertId)){
                    addNode(src->value, tarVertId);
                    return NEW_EDGE;
                }
                return EXISTING_EDGE;
            }
        }
    }else if(srcPartite==2){

        /* Find source and target vertices */
        src = searchValue(*pGraph->vertices2, srcVertId);
        tar = searchValue(*pGraph->vertices1, tarVertId);

        /* Check if vertices exist */
        if(src!=NULL && tar !=NULL){

            /* Check if it is the first edge*/
            if(src->value==NULL){

                /*Initialise list and assign*/
                src->value = (linkedList_t*)safeMalloc(sizeof(linkedList_t));
                src->value->pHead=NULL;
                addNode(src->value, tarVertId);

                return NEW_EDGE;
            }else{

                /*Check if edge already exists*/
                if(!findElement(src->value, tarVertId)){
                    addNode(src->value, tarVertId);
                    return NEW_EDGE;
                }
                return EXISTING_EDGE;
            }
        }
    }

    /* partite not found */
    return ERROR_VALUE;
} /* end of bipartGraphInsertEdge() */


int bipartGraphDeleteVertex(bpGraph_t* pGraph, int vertId, int partite)
{
    binTreeNode_t *pDelNode;
    binTreeNode_t **parentNode = (binTreeNode_t**)safeMalloc(sizeof(binTreeNode_t*));
    int *pLeftChild = (int *)safeMalloc(sizeof(int));

	if(partite==1){
        pDelNode = searchDeleteNode(*pGraph->vertices1, vertId, parentNode, pLeftChild);

        /* Delete the vertex from tree */
        deleteTreeNode(pGraph->vertices1,pDelNode,*parentNode,*pLeftChild);

        findAndDelete(*pGraph->vertices2, vertId);

        return FOUND;
	}
	else if(partite==2){
        pDelNode = searchDeleteNode(*pGraph->vertices2, vertId, parentNode, pLeftChild);

        /* Delete the vertex from tree */
        deleteTreeNode(pGraph->vertices2,pDelNode,*parentNode,*pLeftChild);

        findAndDelete(*pGraph->vertices1, vertId);

        return FOUND;
	}

	return ERROR_VALUE;
} /* end of bipartGraphDeleteVertex() */


int bipartGraphDeleteEdge(bpGraph_t* pGraph,  int srcVertId, int tarVertId, int srcPartite)
{
    binTreeNode_t *pNode;
    int status;

	if(srcPartite == 1){
        pNode = searchValue(*pGraph->vertices1, srcVertId);

        /* Check if vertex exists and has edges */
        if(pNode!=NULL && pNode->value!=NULL){
            status = deleteNode(pNode->value, tarVertId);

            return status;
        }

        return NOT_FOUND;
	}
	else if(srcPartite==2){
        pNode = searchValue(*pGraph->vertices2, srcVertId);

        /* Check if vertex exists and has edges */
        if(pNode!=NULL && pNode->value!=NULL){
            status = deleteNode(pNode->value, tarVertId);

            return status;
        }

        return NOT_FOUND;
	}

	/* Partite does not exist */
	return ERROR_VALUE;
} /* end of bipartGraphDeleteEdge() */


int bipartGraphFindVertex(bpGraph_t *pGraph, int vertId, int partite)
{
    binTreeNode_t *pNode;

    if(partite==1){
        /* Searching from the tree */
        pNode = searchValue(*pGraph->vertices1, vertId);

        if(pNode!=NULL){
            return FOUND;
        }

        return NOT_FOUND;
    }
    else if(partite==2){
        /* Searching from the tree */
        pNode = searchValue(*pGraph->vertices2, vertId);

        if(pNode!=NULL){
            return FOUND;
        }

        return NOT_FOUND;
    }

    /* Partite does not exist */
	return ERROR_VALUE;
} /* end of bipartGraphFindVertex() */


int bipartGraphFindEdge(bpGraph_t* pGraph, int srcVertId, int tarVertId, int srcPartite)
{
    binTreeNode_t *pNode;

    if(srcPartite==1){
        /* Searching from the tree */
        pNode = searchValue(*pGraph->vertices1, srcVertId);

        /* Check if vertex exists and has edges */
        if(pNode!=NULL && pNode->value!=NULL){
            return findElement(pNode->value, tarVertId);
        }

        return NOT_FOUND;
    }
    else if(srcPartite==2){
        /* Searching from the tree */
        pNode = searchValue(*pGraph->vertices2, srcVertId);

        /* Check if vertex exists and has edges */
        if(pNode!=NULL && pNode->value!=NULL){
            return findElement(pNode->value, tarVertId);
        }

        return NOT_FOUND;
    }


	return ERROR_VALUE;
} /* end of bipartGraphFindEdge() */


void bipartGraphPrint(bpGraph_t *pGraph)
{
    printf("Vertices:\n");

    printf("Partite 1:\n");
    printTree(*pGraph->vertices1);
    printf("Partite 2:\n");
    printTree(*pGraph->vertices2);

    printf("Edges:\n");
    printf("Partite 1 to 2:\n");
    preorder(*pGraph->vertices1);
    printf("Partite 2 to 1:\n");
    preorder(*pGraph->vertices2);

} /* end of bipartGraphPrint() */


/* ********************************************************** */

void findAndDelete(binTreeNode_t *temp, int vertId)
{
    if (temp != NULL) {

        if(temp->value!=NULL){
            /* Iterating through the list that contains edges */
            deleteNode(temp->value, vertId);
        }

        findAndDelete(temp->left, vertId);
        findAndDelete(temp->right, vertId);
    }
} /* end of findAndDelete() */

void preorder(binTreeNode_t *temp) {
    llNode_t *pCurrNode;

    if (temp != NULL) {

        if(temp->value!=NULL){
            /* Iterating through the list that contains edges */
            pCurrNode = temp->value->pHead;
            while (pCurrNode != NULL) {
                printf("%d %d\n", temp->key, pCurrNode->element);
                pCurrNode = pCurrNode->pNext;
            }
        }

        preorder(temp->left);
        preorder(temp->right);
    }
}

