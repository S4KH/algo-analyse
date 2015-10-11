/*
 * bipartiteGraph.h
 *
 *  Created on: 01/07/2015
 *      Author: jkct440
 */

#ifndef BIPARTITEGRAPH_H_
#define BIPARTITEGRAPH_H_


/* Forward declaration.  Actual definition is up to each individual data structure implementation. */
typedef struct implBipartGraph_t bpGraph_t;







/* ******************************************************************************************* */
/* Functions declarations for the bipartite graph abstract data type, that you will implement the definitions of in the source files. */


/*
 * Creates a bipartite graph with 'vertNumPart1' number of vertices in partite 1 and 'vertNumPart2' number of vertices in partite 2.
 *
 * vertNumPart1 - Number of vertices in partite 1.
 * vertNumPart1 - Number of vertices in partite 2.
 *
 * returns - pointer to newly created graph (or NULL if operation failed).
 */
extern
bpGraph_t* bipartGraphCreate(int vertNumPart1, int vertNumPart2);


/*
 * Destroys bipartite graph 'pGraph'.
 *
 * pGraph - pointer to bipartite graph.
 */
extern
void bipartGraphDestroy(bpGraph_t *pGraph);


/*
 * Inserts a new vertex with id label 'vertId' into bipartite graph 'pGraph'.
 *
 * pGraph - pointer to bipartite graph.
 * vertId - id of vertex to insert.
 * partite - which partite to insert the vertex.
 *
 * returns - status code whether the vertex was successfully inserted or not.
 */
extern
int bipartGraphInsertVertex(bpGraph_t *pGraph, int vertId, int partite);


/*
 * Inserts an edge (srcVertId, tarVertId), with 'srcVertId' from partite 'srcPartite' into bipartite graph 'pGraph'.
 *
 * pGraph - pointer to bipartite graph.
 * srcVertId - id of source vertex.
 * tarVertId - id of targe vertex.
 * srcPartite - which partite does the source vertex belongs it (this determines the direction of the edge).
 *
 * returns - status code whether the edge was successfully inserted or not.
 */
extern
int bipartGraphInsertEdge(bpGraph_t *pGraph, int srcVertId, int tarVertId, int srcPartite);


/*
 * Removes a vertex with id label 'vertId' from bipartite graph 'pGraph'.
 *
 * pGraph - pointer to bipartite graph.
 * vertId - id of vertex to insert.
 * partite - which partite to insert the vertex.
 *
 * returns - status code whether the vertex was successfully removed or not.
 */
extern
int bipartGraphDeleteVertex(bpGraph_t *pGraph, int vertId, int partite);


/*
 * Removes an edge (srcVertId, tarVertId), with 'srcVertId' from partite 'srcPartite' from bipartite graph 'pGraph'.
 *
 * pGraph - pointer to bipartite graph.
 * srcVertId - id of source vertex.
 * tarVertId - id of targe vertex.
 * srcPartite - which partite does the source vertex belongs it (this determines the direction of the edge).
 *
 * returns - status code whether the edge was successfully removed or not.
 */
extern
int bipartGraphDeleteEdge(bpGraph_t *pGraph, int srcVertId, int tarVertId, int srcPartite);


/*
 * Searches for a vertex with id label 'vertId' in bipartite graph 'pGraph'.
 *
 * pGraph - pointer to bipartite graph.
 * vertId - id of vertex to search for.
 * partite - which partite to insert the vertex.
 *
 * returns - status code whether the vertex was successfully found or not.
 */
extern
int bipartGraphFindVertex(bpGraph_t *pGraph, int vertId, int partite);


/*
 * Searches for an edge (srcVertId, tarVertId), with 'srcVertId' from partite 'srcPartite' from bipartite graph 'pGraph'.
 *
 * pGraph - pointer to bipartite graph.
 * srcVertId - id of source vertex.
 * tarVertId - id of targe vertex.
 * srcPartite - which partite does the source vertex belongs it (this determines the direction of the edge).
 *
 * returns - status code whether the edge was successfully found or not.
 */
extern
int bipartGraphFindEdge(bpGraph_t *pGraph, int srcVertId, int tarVertId, int srcPartite);


/*
 * Prints to stdout the contents of the bipartite graph 'pGraph'.
 *
 * pGraph - pointer to bipartite graph.
 *
 * returns - status code whether vertex was successfully printed or not.
 */
extern
void bipartGraphPrint(bpGraph_t *pGraph);



#endif /* BIPARTITEGRAPH_H_ */
