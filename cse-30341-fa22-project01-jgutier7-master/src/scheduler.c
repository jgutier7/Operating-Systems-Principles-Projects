/* scheduler.c: PQSH Scheduler */

#include "../include/pqsh/macros.h"
#include "../include/pqsh/scheduler.h"
#include "../include/pqsh/timestamp.h"
#include "../include/pqsh/process.h"

#include <errno.h>
#include <sys/wait.h>

/**
 * Add new command to waiting queue.
 * @param   s	    Pointer to Scheduler structure.
 * @param   fs      File stream to write to.
 * @param   command Command string for new Process.
 **/
void scheduler_add(Scheduler *s, FILE *fs, const char *command) {
    /* TODO: Implement */ 
    Process *p = process_create(command);
    queue_push(&(s->waiting), p);
    p->arrival_time = timestamp();
    
}

/**
 * Display status of queues in Scheduler.
 * @param   s	    Pointer to Scheduler structure.
 * @param   fs      File stream to write to.
 * @param   queue   Bitmask specifying which queues to display.
 **/
void scheduler_status(Scheduler *s, FILE *fs, int queue) {
    /* Display status of specified queue (default is all) */
    fprintf(fs, "Running = %4lu, Waiting = %4lu, Finished = %4lu, Turnaround = %05.2lf, Response = %05.2lf\n",
        (s->running).size, (s->waiting).size, (s->finished).size,s->total_turnaround_time, s->total_response_time);

    /* TODO: Complement implementation. */
    if(queue == 0 || queue == RUNNING) {

        fprintf(fs, "Running Queue:\n");
        queue_dump(&s->running,fs);
    } 
    if (queue == 0 || queue == WAITING) {
        fprintf(fs, "Waiting Queue:\n");
        queue_dump(&s->waiting,fs);
    } 
    if (queue == 0 || queue == FINISHED) {
        fprintf(fs, "Finished Queue:\n");
        queue_dump(&s->finished,fs);
    }
    

}

/**
 * Schedule next process using appropriate policy.
 * @param   s	    Pointer to Scheduler structure.
 **/
void scheduler_next(Scheduler *s) {
    /* TODO: Dispatch to appropriate scheduler function. */
    if(s->policy == FIFO_POLICY) {                                          // if scheduler is fifo policy, send to fifo scheduler 
        scheduler_fifo(s);
    } else {                                                                // else: the user wants RDRN scheduling 
        scheduler_rdrn(s);
    }
    
}

/**
 * Wait for any children and remove from queues and update metrics.
 * @param   s	    Pointer to Scheduler structure.
 **/
void scheduler_wait(Scheduler *s) {
    pid_t pid;
    /* TODO: Wait for any children without blocking:
     *
     *  - Remove process from queues.
     *  - Update Process metrics.
     *  - Update Scheduler metrics.
     **/
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        /* remove process from queues */
        Process *found = queue_remove(&s->running, pid);

        /* update process metrics */
        found->end_time = timestamp();                              // end time
        
        /* update scheduler metrics */
        queue_push(&s->finished,found);                             // push process to the finished queue
        s->total_turnaround_time = found->end_time - found->arrival_time;                    
 

    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
