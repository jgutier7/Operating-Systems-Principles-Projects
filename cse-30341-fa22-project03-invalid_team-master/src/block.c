/* block.c: Block Structure */
/* completed by: Jacob Gutierrez, Steve Zhao */

#include "malloc/block.h"
#include "malloc/counters.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Allocate a new block on the heap using sbrk:
 *
 *  1. Determined aligned amount of memory to allocate.
 *  2. Allocate memory on the heap.
 *  3. Set allocage block properties.
 *
 * @param   size    Number of bytes to allocate.
 * @return  Pointer to data portion of newly allocate block.
 **/
Block *	block_allocate(size_t size) {
    // Allocate block
    intptr_t allocated = sizeof(Block) + ALIGN(size);
    Block *  block     = sbrk(allocated);
    if (block == SBRK_FAILURE) {
        return NULL;
    }

    // Record block information
    block->capacity = ALIGN(size);
    block->size     = size;
    block->prev     = block;
    block->next     = block;

    // Update counters
    Counters[HEAP_SIZE] += allocated;
    Counters[BLOCKS]++;
    Counters[GROWS]++;
    return block;
}

/**
 * Attempt to release memory used by block to heap:
 *
 *  1. If the block is at the end of the heap.
 *  2. The block capacity meets the trim threshold.
 *
 * @param   block   Pointer to block to release.
 * @return  Whether or not the release completed successfully.
 **/
bool	block_release(Block *block) {
    // TODO: Implement block release
    size_t allocated = block->capacity + sizeof(Block);
    void * end=sbrk(0);
    if (end == SBRK_FAILURE) {
        return false;
    }
    intptr_t end_dest   = (intptr_t) block + block->capacity + sizeof(Block); 
    if((intptr_t)end != end_dest) return false; //do not release if it is no at the end
    // check if capacity meets the trim threshold 
    if (block->capacity >= TRIM_THRESHOLD){ 
        // Detach Block 
        block_detach(block);
        sbrk(-allocated); //shrink the heap?
        // Update Counters 
        Counters[BLOCKS]--;
        Counters[SHRINKS]++;
        Counters[HEAP_SIZE] -= allocated;
        return true;
    }
    return false;
}

/**
 * Detach specified block from its neighbors.
 *
 * @param   block   Pointer to block to detach.
 * @return  Pointer to detached block.
 **/
Block * block_detach(Block *block) {
    // TODO: Detach block from neighbors by updating previous and next block
    if (block) {
        Block *prev = block->prev;
        Block *next = block->next;
        prev->next  = block->next;
        next->prev  = block->prev;
        // point the block to itself
        block->next = block;
        block->prev = block;
    }
    return block;
}

/**
 * Attempt to merge source block into destination.
 *
 *  1. Compute end of destination and start of source.
 *
 *  2. If they both match, then merge source into destination by giving the
 *  destination all of the memory allocated to source.
 *
 *  3. Update references from and to destination block appropriately.
 *
 * @param   dst     Destination block we are merging into.
 * @param   src     Source block we are merging from.
 * @return  Whether or not the merge completed successfully.
 **/
bool	block_merge(Block *dst, Block *src) {
    // TODO: Implement block merge
    // compute the end of destination and start of source 
    intptr_t end_dest   = (intptr_t) dst + dst->capacity + sizeof(Block); 
    intptr_t start_src  = (intptr_t) src;
    // if they both match, merge source into destination 
    if (end_dest == start_src) {
        

        dst->capacity = dst->capacity + src->capacity + sizeof(Block);
        block_detach(src);
        // Update Counters 
        Counters[MERGES]++;
        Counters[BLOCKS]--;
        return true;
    }
    return false;
}

/**
 * Attempt to split block with the specified size:
 *
 *  1. Check if block capacity is sufficient for requested aligned size and
 *  header Block.
 *
 *  2. Split specified block into two blocks.
 *
 * @param   block   Pointer to block to split into two separate blocks.
 * @param   size    Desired size of the first block after split.
 * @return  Pointer to original block (regardless if it was split or not).
 **/
Block * block_split(Block *block, size_t size) {
    // TODO: Implement block split
    // Check if capacity is sufficicent 
    
    if (block->capacity > (ALIGN(size) + sizeof(Block))) {
        // split into two blocks
        size_t new_size     = block->capacity - ALIGN(size)-sizeof(Block);
        intptr_t newpt= (intptr_t) block + (intptr_t)(ALIGN(size) + sizeof(Block));
        Block *new          = (Block *) newpt;
        new->capacity       = new_size;
        new->size           = 0; //not sure
        new->prev           = block;
        new->next           = block->next;

        // update block references
        new->next->prev     = new;
        block->next         = new;
        block->capacity     = ALIGN(size);
        block->size         = size; 
        
        // Update Counters 
        Counters[SPLITS]++;
        Counters[BLOCKS]++;
    } else {
        /* Not sure why but the test script shows that even if there is no split, the size passed into the function still becomes the new block size */
        block->size = size;
    }
    return block;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */