/* scheduler_fifo.c: PQSH FIFO Scheduler */

#include "../include/pqsh/macros.h"
#include "../include/pqsh/scheduler.h"
#include "../include/pqsh/process.h"
#include "../include/pqsh/timestamp.h"

#include <assert.h>

/**
 * Schedule next process using fifo policy:
 *
 *  As long as we have less running processes than our number of CPUS and
 *  there are waiting processes, then we should move a process from the
 *  waiting queue to the running queue.
 *
 * @param   s	    Scheduler structure
 */
void scheduler_fifo(Scheduler *s) {
    /* TODO: Implement FIFO Policy */
    while((s->running).size < s->cores && (s->waiting).size != 0) { 				// while s.running.size() < NCPUS and not s.waiting.empty():
		Process *p = queue_pop(&(s->waiting)); 										// process = s.waiting.pop()
		process_start(p); 															// StartProcess(process)
		queue_push(&(s->running),p);												// s.running.push(process)
		p->start_time = timestamp();
	}
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
