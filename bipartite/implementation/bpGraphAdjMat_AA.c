/*
 * bipartGraphAdjMat.c
 *
 *  Created on: 01/07/2015
 *      Author: jkct440
    2* 2d arrays each representing one partite and its edges

 */

#include <stdio.h>
#include <stddef.h>
#include "bipartiteGraph.h"
#include "memoryUtil.h"
#include "commonDefs.h"


struct implBipartGraph_t
{
	/** Vertex number of partite 1. */
	int vertNum1;
	/** Vertex number of partite 2. */
	int vertNum2;

	/* Array to store graph edges and vertices */
	int **vertArray1;
	int **vertArray2;

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


bpGraph_t* bipartGraphCreate(int part1VertNum, int part2VertNum)
{
	int i;

    /* Initializing/allocating memories with given values */
	bpGraph_t *pGraph = (bpGraph_t*) safeMalloc(sizeof(bpGraph_t));
	pGraph->vertNum1 = part1VertNum;
	pGraph->vertNum2 = part2VertNum;

	pGraph->vVertExistP1 = (char *) safeMalloc(part1VertNum * sizeof(char));
	pGraph->vVertExistP2 = (char *) safeMalloc(part2VertNum * sizeof(char));

    /* Partite 1 */
    pGraph->vertArray1 =(int**) safeMalloc(part1VertNum*sizeof(int**));
	for (i = 0; i < part1VertNum; i++) {
        pGraph->vVertExistP1[i] = 1;
        pGraph->vertArray1[i] =(int*) safeMalloc(part2VertNum * sizeof(int*));
	}

    /* Partite 2 */
	pGraph->vertArray2 =(int**) safeMalloc(part2VertNum*sizeof(int**));
	for (i = 0; i < part2VertNum; i++) {
        pGraph->vVertExistP2[i] = 1;
        pGraph->vertArray2[i] =(int*) safeMalloc(part1VertNum * sizeof(int*));
	}
	return pGraph;
} /* end of bipartGraphDestroy() */


void bipartGraphDestroy(bpGraph_t* pGraph)
{
    int i;
	/* Free each vertex's neighbours array in partite 1 */
	for(i = 0; i < pGraph->vertNum1 ; i++){
		safeFree(pGraph->vertArray1[i], pGraph->vertNum2);
	}

	/* Free each vertex's neighbours array in partite 2 */
	for(i = 0; i < pGraph->vertNum2 ; i++){
		safeFree(pGraph->vertArray2[i], pGraph->vertNum1);
	}

	/* Free the array of vertices */
	safeFree(pGraph->vertArray1, pGraph->vertNum1);
	safeFree(pGraph->vertArray2, pGraph->vertNum2);

	/* Free the graph itself */
	safeFree(pGraph, sizeof(bpGraph_t));
} /* end of bipartGraphDestroy() */


int bipartGraphInsertVertex(bpGraph_t* pGraph, int vertId, int partite)
{
	int j;

	/* insert into partite 1 */
	if (partite == 1) {
		/* check if 'vertid' is an existing vertex */
		if (pGraph->vertNum1>vertId) {
			if (pGraph->vVertExistP1[vertId]) {
				return EXISTING_VERTEX;
			}
			else {
				pGraph->vVertExistP1[vertId] = 1;
				return NEW_VERTEX;
			}
		}
		else {
			/* adding new row with number of columns equal to partite 2 total vertex */
			int **vDPTemp = (int**) safeRealloc(pGraph->vertArray1, (vertId+1) * sizeof(int*), (vertId+1-pGraph->vertNum1) * sizeof(int*));
			pGraph->vertArray1 = vDPTemp;
			int *vTemp = (int *)safeMalloc(pGraph->vertNum2 * sizeof(int*));
			pGraph->vertArray1[vertId] = vTemp;
			char *cTemp = (char*) safeRealloc(pGraph->vVertExistP1, (vertId+1) * sizeof(char), (vertId+1-pGraph->vertNum1) * sizeof(char));
			pGraph->vVertExistP1 = cTemp;

            /* initialise all new elements to 0 */
			for (j=0; j < pGraph->vertNum2; j++) {
				pGraph->vertArray1[vertId][j] = 0;
				vTemp = (int *)safeRealloc(pGraph->vertArray2[j], pGraph->vertNum1+1 * sizeof(int*), (vertId+1-pGraph->vertNum1) * sizeof(int*));
				pGraph->vertArray2[j] = vTemp;
				pGraph->vertArray2[j][vertId] = 0;
			}

			pGraph->vVertExistP1[vertId] = 1;

			pGraph->vertNum1 = vertId+1;

			for (j=0; j < pGraph->vertNum2; j++) {
				pGraph->vertArray1[vertId][j] = 0;
			}

			/*printMatrix(pGraph);*/
			return NEW_VERTEX;
		}
	}
	/* insert into partite 1 */
	else if (partite == 2) {
		/* check if 'vertid' is an existing vertex */
		if (pGraph->vertNum2>vertId) {
			if (pGraph->vVertExistP2[vertId]) {
				return EXISTING_VERTEX;
			}
			else {
				pGraph->vVertExistP2[vertId] = 1;
				return NEW_VERTEX;
			}
		}
		else {
			/* adding new row with number of columns equal to partite 1 total vertex */
			int **vDPTemp = safeRealloc(pGraph->vertArray2, (vertId+1) * sizeof(int*), (vertId+1-pGraph->vertNum2) * sizeof(int*));
			pGraph->vertArray2 = vDPTemp;
			int *vTemp = (int *)safeMalloc(pGraph->vertNum1 * sizeof(int*));
			pGraph->vertArray2[vertId] =vTemp;
            char *cTemp = (char*) safeRealloc(pGraph->vVertExistP2, (vertId+1) * sizeof(char), (vertId+1-pGraph->vertNum2) * sizeof(char));
			pGraph->vVertExistP2 = cTemp;

            /* initialise all new elements to 0 */
			for (j=0; j < pGraph->vertNum1; j++) {
				pGraph->vertArray2[vertId][j] = 0;
				vTemp = (int *)safeRealloc(pGraph->vertArray1[j], pGraph->vertNum2+1 * sizeof(int*), (vertId+1-pGraph->vertNum2) * sizeof(int*));
				pGraph->vertArray1[j] = vTemp;
				pGraph->vertArray1[j][vertId] = 0;
			}

			pGraph->vVertExistP2[vertId] = 1;

			for (j=0; j < pGraph->vertNum1; j++) {
				pGraph->vertArray2[vertId][j] = 0;
			}

			pGraph->vertNum2 = vertId+1;
			/*printMatrix(pGraph);*/
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
        if (pGraph->vertArray1[srcVertId][tarVertId]==0) {
            pGraph->vertArray1[srcVertId][tarVertId]=1;
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
        if (pGraph->vertArray2[srcVertId][tarVertId]==0) {
            pGraph->vertArray2[srcVertId][tarVertId]=1;
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
			safeFree(pGraph->vertArray1[vertId],sizeof(pGraph->vertNum2));
			pGraph->vertArray1[vertId] = NULL;
			pGraph->vVertExistP1[vertId] = 0;
			findAndDelete(pGraph, vertId, partite);

			return FOUND;
		}

		/* vertex not in graph */
		return NOT_FOUND;
	}
	else if (partite == 2) {
		/* check if vertex exists */
		if (pGraph->vVertExistP2[vertId]) {
			/* delete neighbours */
			safeFree(pGraph->vertArray2[vertId],sizeof(pGraph->vertNum1));
			pGraph->vertArray2[vertId] = NULL;
			pGraph->vVertExistP2[vertId] = 0;
			findAndDelete(pGraph, vertId, partite);

			return FOUND;
		}

		return NOT_FOUND;
	}

	/* Wrong partite. */
	return ERROR_VALUE;
} /* end of bipartGraphDeleteVertex() */


int bipartGraphDeleteEdge(bpGraph_t* pGraph,  int srcVertId, int tarVertId, int srcPartite)
{
	if (srcPartite == 1) {
		/* The requested addition should be within the vertices id limits. */
		if (srcVertId >= pGraph->vertNum1 || tarVertId >= pGraph->vertNum2 || !pGraph->vVertExistP1[srcVertId] || !pGraph->vVertExistP2[tarVertId]) {
			return ERROR_VALUE;
		}

        /* Check if there is edge */
        if(pGraph->vertArray1[srcVertId][tarVertId]){
            /* set zero to corresponding matrix cell */
            pGraph->vertArray1[srcVertId][tarVertId] = 0;
            return FOUND;
		}
        return NOT_FOUND;

	}
	else if (srcPartite == 2) {
		/* The requested addition should be within the vertices id limits. */
		if (srcVertId >= pGraph->vertNum2 || tarVertId >= pGraph->vertNum1 || !pGraph->vVertExistP2[srcVertId] || !pGraph->vVertExistP1[tarVertId]) {
			return ERROR_VALUE;
		}

        /* Check if there is edge */
        if(pGraph->vertArray2[srcVertId][tarVertId]){
            /* set zero to corresponding matrix cell */
            pGraph->vertArray2[srcVertId][tarVertId] = 0;
            return FOUND;
		}
        return NOT_FOUND;


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
		if (srcVertId >= pGraph->vertNum1 || tarVertId >= pGraph->vertNum2 ) {
			return NOT_FOUND;
		}

        /* check if edge exists */
		return pGraph->vertArray1[srcVertId][tarVertId]==1 ? 1 : 0;
	}
	else if (srcPartite == 2) {
		if (srcVertId >= pGraph->vertNum2 || tarVertId >= pGraph->vertNum1) {
			return NOT_FOUND;
		}

		/* check if edge exists */
		return pGraph->vertArray2[srcVertId][tarVertId]==1 ? 1 : 0;
	}

	return ERROR_VALUE;
} /* end of bipartGraphFindEdge() */


void bipartGraphPrint(bpGraph_t *pGraph)
{
    int i, j;
    printf("Vertices:\n");
    printf("Partite 1:\n");
    for(i=0;i<pGraph->vertNum1;i++){
        if(pGraph->vVertExistP1[i]){
            printf("%d ",i);
        }
    }

    printf("\nPartite 2:\n");
    for(i=0;i<pGraph->vertNum2;i++){
        if(pGraph->vVertExistP2[i]){
            printf("%d ",i);
        }
    }

    printf("\nEdges:\n");
    printf("Partite 1 to 2:\n");
    for(i=0;i<pGraph->vertNum1;i++){
        for(j=0;j<pGraph->vertNum2;j++){
            if(pGraph->vertArray1[i]!=NULL && pGraph->vertArray1[i][j] && pGraph->vVertExistP2[j]){
                printf("%d %d\n", i, j);
            }
        }
    }

    printf("Partite 2 to 1:\n");
    for(i=0;i<pGraph->vertNum2;i++){
        for(j=0;j<pGraph->vertNum1;j++){
            if(pGraph->vertArray2[i]!=NULL && pGraph->vertArray2[i][j] && pGraph->vVertExistP1[j]){
                printf("%d %d\n", i, j);
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
				pGraph->vertArray1[i][vertId]=0;
			}
		}
	}
	else if (partite == 2) {
		for (i = 0; i < pGraph->vertNum2; ++i) {
			if (pGraph->vVertExistP2[i]) {
				pGraph->vertArray2[i][vertId]=0;
			}
		}
	}
} /* end of findAndDelete() */
