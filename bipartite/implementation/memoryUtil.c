/*
 * memoryUtil.c
 *
 *  Created on: 01/07/2015
 *      Author: jkct440
 */


#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "memoryUtil.h"

/*
 * Amount of memory currently dynamically allocated.
 */
static
size_t sg_allocatedMemory;


size_t pollMemoryAllocated()
{
	return sg_allocatedMemory;
} /* end of dynMemoryAllocated() */




void *safeMalloc(size_t size)
{
	void *pMem = NULL;
	if ((pMem = malloc(size)) == NULL) {
		fprintf(stderr, "Error: safe_malloc() cannot allocate "
			"memory of size %lu.\n", size);
		exit(EXIT_FAILURE);
	}

	sg_allocatedMemory += size;

	return pMem;
} /* end of safeMalloc() */


void *safeRealloc(void *pMem, size_t newSize, size_t extraMem)
{
	if ((pMem = realloc(pMem, newSize)) == NULL) {
		fprintf(stderr, "Error: safe_realloc() cannot allocate "
			"new memory of size %lu.\n", newSize);
		exit(EXIT_FAILURE);
	}

	sg_allocatedMemory += extraMem;

	return pMem;
} /* end of safeRealloc() */


void safeFree(void *pMem, size_t memSize)
{
	free(pMem);
	sg_allocatedMemory = (sg_allocatedMemory <= memSize) ? 0 : (sg_allocatedMemory - memSize);
} /* end of safeFree() */



