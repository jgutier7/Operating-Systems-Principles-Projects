/* freelist.c: Free List Implementation
 *
 * The FreeList is an unordered doubly-linked circular list containing all the
 * available memory allocations (memory that has been previous allocated and
 * can be re-used).
 **/

#include "malloc/counters.h"
#include "malloc/freelist.h"

/* Global Variables */

Block FreeList = {-1, -1, &FreeList, &FreeList};

/* Functions */

/**
 * Search for an existing block in free list with at least the specified size
 * using the first fit algorithm.
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block * free_list_search_ff(size_t size) {
    // TODO: Implement first fit algorithm
    for (Block *curr = FreeList.next; curr != &FreeList; curr = curr->next) {
        if (curr->capacity >= size) {
            return curr;
        }
    }
    return NULL;
}

/**
 * Search for an existing block in free list with at least the specified size
 * using the best fit algorithm.
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block * free_list_search_bf(size_t size) {
    // TODO: Implement best fit algorithm
    bool flag=false;
    Block *best=FreeList.next;
    size_t bests=SIZE_MAX;
    for (Block *curr = FreeList.next; curr != &FreeList; curr = curr->next) {
        if (curr->capacity >= size && bests > curr->capacity) {
            flag=true;
            best=curr;
            bests=curr->capacity;
        }
    }
    if(!flag) return NULL;
    return best;
}

/**
 * Search for an existing block in free list with at least the specified size
 * using the worst fit algorithm.
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block * free_list_search_wf(size_t size) {
    // TODO: Implement worst fit algorithm
    Block *best=FreeList.next;
    for (Block *curr = FreeList.next; curr != &FreeList; curr = curr->next) {
        if (best->capacity < curr->capacity) {
            best=curr;
        }
    }
    if(best==&FreeList||best->capacity<size) return NULL;
    return best;
}

/**
 * Search for an existing block in free list with at least the specified size.
 *
 * Note, this is a wrapper function that calls one of the three algorithms
 * above based on the compile-time setting.
 *
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block * free_list_search(size_t size) {
    Block * block = NULL;
#if	defined FIT && FIT == 0
    block = free_list_search_ff(size);
#elif	defined FIT && FIT == 1
    block = free_list_search_wf(size);
#elif	defined FIT && FIT == 2
    block = free_list_search_bf(size);
#endif

    if (block) {
        Counters[REUSES]++;
    }
    return block;
}

/**
 * Insert specified block into free list.
 *
 * Scan the free list for the first block whose address is after the given
 * input block and then insert the block into the free list in a manner that
 * ensures that the blocks in the free list are stored in ascending address
 * values (ie. the first block's address is less than the second block's
 * address, etc.).
 *
 * After inserting the block into the free list, attempt to merge block with
 * adjacent blocks.
 *
 * @param   block   Pointer to block to insert into free list.
 **/
void	free_list_insert(Block *block) {
    // TODO: Implement free list insertion
    bool ins=false;
    for (Block *curr = FreeList.next; curr != &FreeList; curr = curr->next) {
        if ((intptr_t) curr > (intptr_t) block) {
            Block *tail   = curr->prev;
            Block *front  = curr;
            tail->next    = block;
            front->prev   = block;
            block->next   = front;
            block->prev   = tail;
            ins=true;
            break;
        }
    }
    //if address is greater than the last node
    if(!ins){ 
        Block *tail   = FreeList.prev;
        tail->next    = block;
        FreeList.prev = block;
        block->next   = &FreeList;
        block->prev   = tail;
    }
    block_merge(block,block->next);
    block_merge(block->prev,block);
}

/**
 * Return length of free list.
 * @return  Length of the free list.
 **/
size_t  free_list_length() {
    // TODO: Implement free list length
    size_t ans=0;
    for (Block *curr = FreeList.next; curr != &FreeList; curr = curr->next) {
        ans++;
    }
    return ans;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
