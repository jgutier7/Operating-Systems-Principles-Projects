/* posix.c: POSIX API Implementation */

#include "malloc/counters.h"
#include "malloc/freelist.h"

#include <assert.h>
#include <string.h>

/**
 * Allocate specified amount memory.
 * @param   size    Amount of bytes to allocate.
 * @return  Pointer to the requested amount of memory.
 **/
void *malloc(size_t size) {
    // Initialize counters
    init_counters();

    // Handle empty size
    if (!size) {
        return NULL;
    }

    // TODO: Search free list for any available block with matching size
    Block *block;
    block=free_list_search(size);
    if(!block){
         block=block_allocate(size);  
    }else{
        block=block_split(block, size);
        block=block_detach(block);
    }

    // Could not find free block or allocate a block, so just return NULL
    if (!block) {
        return NULL;
    }
    block->size=size;


    // Update counters
    Counters[MALLOCS]++;
    Counters[REQUESTED] += size;

    // Return data address associated with block
    intptr_t d=(intptr_t) block + sizeof(Block);
    return (void *) d;
}

/**
 * Release previously allocated memory.
 * @param   ptr     Pointer to previously allocated memory.
 **/
void free(void *ptr) {
    if (!ptr) {
        return;
    }

    // Update counters
    Counters[FREES]++;

    // TODO: Try to release block, otherwise insert it into the free list
    // from malloc1.c
    Block *block = BLOCK_FROM_POINTER(ptr);
    if (!block_release(block)) free_list_insert(block);

}

/**
 * Allocate memory with specified number of elements and with each element set
 * to 0.
 * @param   nmemb   Number of elements.
 * @param   size    Size of each element.
 * @return  Pointer to requested amount of memory.
 **/
void *calloc(size_t nmemb, size_t size) {
    // TODO: Implement calloc
    if(nmemb == 0 || size == 0) return NULL;    // If nmemb or size is 0, then calloc() returns either NULL, or a unique pointer
    size *= nmemb;                              // allocates memory for an array of nmemb elements of size bytes each
    Counters[CALLOCS]++;                        // update counter 
    void *ptr = malloc(size);                   // malloc memory
    if(ptr) memset(ptr, 0, size);               // if malloc worked, fill the memory is set to zero
    return ptr;                                 // return pointer to allocated memory 
}

/**
 * Reallocate memory with specified size.
 * @param   ptr     Pointer to previously allocated memory.
 * @param   size    Amount of bytes to allocate.
 * @return  Pointer to requested amount of memory.
 **/
void *realloc(void *ptr, size_t size) {
    // TODO: Implement realloc
    Counters[REALLOCS]++;                   // update REALLOC counter
    
    if(!ptr) return malloc(size);           // If ptr is NULL, then the call is equivalent to malloc(size)
    if(size == 0) {                         // if size is equal to zero, and ptr is not NULL, then the call is equivalent to free(ptr)
        free(ptr);
        return NULL;
    }
    
    Block *block = BLOCK_FROM_POINTER(ptr); // make new block fromm pointer 

    void *requested = malloc(size);          // malloc requested amount of memory 
    if(!requested) return NULL;              // check if failed

    memcpy(requested, ptr, block->size);     // copy memory area from original pointer to the requested memory 

    free(ptr);                               // free pointer to previosuly allocated memory 
    return requested;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
