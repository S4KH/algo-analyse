/*
 * memoryUtil.h
 *
 *  Created on: 01/07/2015
 *      Author: jkct440
 */

#ifndef MEMORYUTIL_H_
#define MEMORYUTIL_H_

#include <stddef.h>



/*
 * Poll the dynamic memory requested so far.
 */
size_t pollMemoryAllocated();


/*
 * Update the memory allocation count.
 */
void dynMemoryFreed(size_t freeMemory);


/*
	creates a new block of memory with error checking
	returned value should be freed by the calling function
*/
void *safeMalloc(size_t size);

/*
	resize a pre-existing block of memory with error checking
	returned value should be freed by the calling function
*/
void *safeRealloc(void *old_mem, size_t new_size, size_t extraMem);


/*
 * Frees the dynamically allocated memory pMem and also decrease memory usage by memSize.
 */
void safeFree(void *pMem, size_t memSize);


#endif /* MEMORYUTIL_H_ */
