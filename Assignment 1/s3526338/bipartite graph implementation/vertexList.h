/*
 * vertexList.h
 * 
 */
#include "linkedList.h"
#ifndef vertexLIST_H_
#define vertexLIST_H_


/*
 * vertex list node type.
 */

struct vertexListNode_t;

struct vertexListNode_t
{
	int vertId;
	linkedList_t *edges;
	struct vertexListNode_t *pNext;
};

typedef struct vertexListNode_t vlNode_t;

/*
 * vertex list type.  Only has head pointer.
 */
typedef struct
{
	/* pointer to head of list. */
	vlNode_t *pHead;
	/* count of number of vertIds in list */
	int size;
} vertexList_t;


/* ************************************************************************************** */
/* Function declarations */


/*
 * Creates a node with value 'vertId' and adds the newly created node to 'pList', which points to the head of the list.
 * pList - point to head of list.
 * vertId - the vertId you want to insert into the list.
 */
extern
void addVertex(vertexList_t *pList, int vertId);


/*
 * Deletes a node containing vertId from list 'pList'.
 * pList - point to head of list.
 * vertId - the vertId you want to remove from the list.
 * status - the status of the deletion operation.
 *
 * returns - pointer to the (new) head of the list.
 */
extern
int deleteVertex(vertexList_t *pList, int vertId);


/*
 * Destroy the vertex list, assuming all memory is dynamically allocated.
 * pList - point to head of vertex list.
 */
extern
void destroyVList(vertexList_t *pList);


/*
 * Searches if vertId is in the list 'pList'.
 * pList - point to head of list.
 * vertId - the vertId you want to search for in the list.
 */
extern
vlNode_t* findVertex(vertexList_t *pList, int vertId);




#endif /* vertexLIST_H_ */
