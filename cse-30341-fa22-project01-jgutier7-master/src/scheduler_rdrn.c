/* scheduler_rdrn.c: PQSH Round Robin Scheduler */

#include "../include/pqsh/macros.h"
#include "../include/pqsh/scheduler.h"
#include "../include/pqsh/process.h"
#include "../include/pqsh/timestamp.h"

#include <assert.h>

/**
 * Schedule next process using round robin policy:
 *
 *  1. If there no waiting processes, then do nothing.
 *
 *  2. Move one process from front of running queue and place in back of
 *  waiting queue.
 *
 *  3. Move one process from front of waiting queue and place in back of
 *  running queue.
 *
 * @param   s	    Scheduler structure
 **/
void scheduler_rdrn(Scheduler *s) {
    /* TODO: Implement Round Robin Policy */
	if((s->running).size == s->cores) {															// if s.running.size() == NCPUS:
		Process *p = queue_pop(&(s->waiting)); 													// 	process = s.running.pop()
		process_pause(p);																		// 	PauseProcess(process) ... Preemptive by pausing process
		queue_push(&(s->waiting),p);															// 	s.waiting.push(process)
	}
	
	/* Move processes from waiting queue to running queue */
	while((s->running).size < s->cores && (s->waiting).size != 0) { 							// while s.running.size() < NCPUS and not s.waiting.empty():
		Process *p = queue_pop(&(s->waiting));													// 	process = s.waiting.pop()
		if(p->pid == 0) {																		// 	if process.pid == 0:
			process_start(p);																	// 		Start new process
		} else {																				// 	else:
			process_resume(p);																	// 		Resume old process
		}
		queue_push(&(s->running),p);															// 	s.running.push(process)
		p->start_time = timestamp();
	}
	
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
