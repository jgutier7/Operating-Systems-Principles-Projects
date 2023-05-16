/* queue.c: PQSH Queue */

#include "../include/pqsh/macros.h"
#include "../include/pqsh/queue.h"

#include <assert.h>

/**
 * Push process to back of queue.
 * @param q     Pointer to Queue structure.
 **/
void        queue_push(Queue *q, Process *p) {
    /* TODO: Implement */
    if(q->size==0) q->head = p;                                                     // if first item, make it head
    else q->tail = p;                                                               // else, places process as the tail of the queue
    (q->size)++;                                                                    // updates the size of the queue  
}

/**
 * Pop process from front of queue.
 * @param q     Pointer to Queue structure.
 * @return  Process from front of queue.
 **/
Process *   queue_pop(Queue *q) {
    /* TODO: Implement */
    if(q->size == 0) return NULL;
    Process *head = q->head;                                                        // places current head in a placeholder to return       
    (q->head)++;                                                                    // set head to the next item in the queue
    (q->size)--;                                                                    // updates the size of the queue   
    return head;                                                                    // returns popped item

}

/**
 * Remove and return process with specified pid.
 * @param q     Pointer to Queue structure.
 * @param pid   Pid of process to return.
 * @return  Process from Queue with specified pid.
 **/
Process *   queue_remove(Queue *q, pid_t pid) {
    /* TODO: Implement */
    if(!pid) return NULL;                                                            // returns if pid is NULL 
    for(Process *p = q->head; p; p++){                                               // loop through each process in queue
        if(p->pid == pid){                                                           // if pid is found
            if(p == q->head){                                                        // if process is head 
                (q->head)++;                                                         // sets new head
            } else if (p == q->tail){                                                // if it's the tail
                (q->tail)--;                                                         // set tail pointer to the next to last item in queue 
            }

            (q->size)--;                                                             // updates size of queue
            return p;                                                                // returns found process 
        }
    } 
    return NULL;                                                                     // returns NULL if PID is not found in queue
}

/**
 * Dump the contents of the Queue to the specified stream.
 * @param q     Queue structure.
 * @param fs    Output file stream.
 **/
void        queue_dump(Queue *q, FILE *fs) {
    if(q->size == 0) return;                                        // returns if queue is empty
    fprintf(fs, "%6s %-30s %-13s %-13s %-13s\n", 
                "PID", "COMMAND", "ARRIVAL", "START", "END");
    /* TODO: Display information for each item in Queue. */
    int i = 0;
    for(Process *p = q->head; i < q->size; i++){
        fprintf(fs, "%6d %-30s %-13.2f %-13.2f %-13.2f\n", (int)p->pid, p->command, p->arrival_time, p->start_time, p->end_time);
        p++;
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
