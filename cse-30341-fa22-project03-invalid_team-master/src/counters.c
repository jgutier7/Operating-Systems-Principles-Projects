/* counters.c: Counters */

#include "../include/malloc/block.h"
#include "../include/malloc/counters.h"
#include "../include/malloc/freelist.h"

#include <assert.h>

/* Global Variables */

extern Block FreeList;
size_t Counters[NCOUNTERS] = {0};
int    DumpFD              = -1;

/* Functions */

/**
 * Initialize counters by doing the following:
 *
 *  1. Register the dump_counters function to run when the program terminates.
 *  2. Duplicate standard output file descriptor to the DumpFD global variable.
 *
 * Note, these actions should only be performed once regardless of how many
 * times the function is called.
 **/
void init_counters() {
    static bool initialized = false;

    if (!initialized) {
        assert(atexit(dump_counters) == 0);
        initialized = true;
        DumpFD      = dup(STDOUT_FILENO);
        assert(DumpFD >= 0);
    }
}

/**
 * Compute internal fragmentation in heap using the formula:
 *
 *  FRAGMENTATION = Sum(internal fragments) / HeapSize * 100.0
 *
 * @return  Percentage of internal fragmentation in heap.
 **/
double  internal_fragmentation() {
    // TODO: Implement internal fragmentation computation
    // Internal Fragmentation = All Unused / Heap Size
    double frag_perc = 0; 
    double sum       = 0;

    // sum up the internal fragments with loop 
    for(Block *curr = FreeList.next; curr != &FreeList; curr = curr->next){
        sum += (curr->capacity - curr->size);
    }

    // calculate fragmentation 
    if( Counters[HEAP_SIZE] != 0 ){
        // FRAGMENTATION = Sum(internal fragments) / HeapSize * 100.0
        frag_perc = (sum / Counters[HEAP_SIZE]) * 100.0;
    }


    return frag_perc;
}

/**
 * Compute external fragmentation in heap using the formula:
 *
 *  FRAGMENTATION = (1 - (LARGEST_FREE_BLOCK / ALL_FREE_MEMORY)) * 100.0
 *
 * https://www.edn.com/design/systems-design/4333346/Handling-memory-fragmentation
 *
 * @return  Percentage of external fragmentation in heap.
 **/
double  external_fragmentation() {
    // TODO: Implement external fragmentation computation
    double largest   = 0;             // largest block of free space 
    double sum       = 0;             // sum of free memory
    double free_size = 0; 
    double frag_perc = 0;

    // sum up 
    for(Block *curr = FreeList.next; curr != &FreeList; curr = curr->next) {
        free_size = curr->capacity;            // amount of space free within a block 
        sum += free_size;                                   // add to sum
        if(free_size > largest) largest = free_size;        // check if largest free space 
    }

    // calculate fragmentation 
    if( sum != 0 ){
        // FRAGMENTATION = (1 - (LARGEST_FREE_BLOCK / ALL_FREE_MEMORY)) * 100.0
        frag_perc = (1 - (largest / sum)) * 100.0;
    }
    return frag_perc;
}

/**
 * Display all counters to the DumpFD global file descriptor saved in
 * init_counters.
 *
 * Note, the function should close the DumpFD global file descriptor at the end
 * of the function.
 **/
void dump_counters() {
    char buffer[BUFSIZ];
    assert(DumpFD >= 0);

    fdprintf(DumpFD, buffer, "blocks:      %lu\n"   , Counters[BLOCKS]);
    fdprintf(DumpFD, buffer, "free blocks: %lu\n"   , free_list_length());
    fdprintf(DumpFD, buffer, "mallocs:     %lu\n"   , Counters[MALLOCS]);
    fdprintf(DumpFD, buffer, "frees:       %lu\n"   , Counters[FREES]);
    fdprintf(DumpFD, buffer, "callocs:     %lu\n"   , Counters[CALLOCS]);
    fdprintf(DumpFD, buffer, "reallocs:    %lu\n"   , Counters[REALLOCS]);
    fdprintf(DumpFD, buffer, "reuses:      %lu\n"   , Counters[REUSES]);
    fdprintf(DumpFD, buffer, "grows:       %lu\n"   , Counters[GROWS]);
    fdprintf(DumpFD, buffer, "shrinks:     %lu\n"   , Counters[SHRINKS]);
    fdprintf(DumpFD, buffer, "splits:      %lu\n"   , Counters[SPLITS]);
    fdprintf(DumpFD, buffer, "merges:      %lu\n"   , Counters[MERGES]);
    fdprintf(DumpFD, buffer, "requested:   %lu\n"   , Counters[REQUESTED]);
    fdprintf(DumpFD, buffer, "heap size:   %lu\n"   , Counters[HEAP_SIZE]);
    fdprintf(DumpFD, buffer, "internal:    %4.2lf\n", internal_fragmentation());
    fdprintf(DumpFD, buffer, "external:    %4.2lf\n", external_fragmentation());

    close(DumpFD);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
