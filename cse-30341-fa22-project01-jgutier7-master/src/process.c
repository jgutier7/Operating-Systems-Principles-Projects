/* process.c: PQSH Process */

#include "../include/pqsh/macros.h"
#include "../include/pqsh/process.h"
#include "../include/pqsh/timestamp.h"

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/**
 * Create new process structure given command.
 * @param   command     String with command to execute.
 * @return  Pointer to new process structure
 **/
Process *process_create(const char *command) {
    /* TODO: Implement */
    Process *new_process = calloc(1, sizeof(Process));         // allocate pointer 
    if(!new_process) return NULL;                              // return if fails
    if(!strcpy(new_process->command,command)) return NULL;     // copy command into new_process->commamnd
    new_process->arrival_time = timestamp();                   // set time for when it arrives in waiting queue 
    return new_process;                                        // returns new_process
}

/**
 * Start process by forking and executing the command.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not starting the process was successful
 **/
bool process_start(Process *p) {
    /* TODO: Implement */
    p->pid = fork();                                                                            // fork()
    char *argv[MAX_ARGUMENTS] = {NULL};                                                         // char *argv[MAX_ARGUMENTS] = {0}
    int i = 0;                                                                                  // counter for argument 
    switch (p->pid) {
        case -1:                                                                                // unsuccessful fork
            fprintf(stderr, "Unable to fork: %s\n", strerror(errno));                           // prints to stderr
            return false;                                                                       // returns false
        case  0:;                                                                               // child
            if(p->command) {
                for (char *token = strtok(p->command, " "); token; token = strtok(NULL, " ")){  // parses command, spltting using \space as a delimiter 
                    argv[i] = token;                                                            // argv[i] = token; 
                    i++;                                                                        // increment 
                }
                if (!execvp(argv[0],argv)) {                                                    // executes command, arguments
                    fprintf(stderr, "Unable to execvp: %s\n", strerror(errno));
                    return false;                                                               // returns with false if execvp fails
                }
            } else return false;
            break;
        default:                                                                                // Parent (Success)
            p->start_time = timestamp();                                                        // update timestamp for when process starts                                   
            break;                                                                              
    } 

    return true;
}

/**
 * Pause process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_pause(Process *p) {                                                                 // use SIGSTOP 
    /* TODO: Implement */
    if(!kill(p->pid, SIGSTOP)) return true;                                                      // if signal sent succeeds, return true
    return false;
}

/**
 * Resume process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_resume(Process *p) {                                                                // use SIGCONT signal 
    /* TODO: Implement */
    if(!kill(p->pid, SIGCONT)) return true;
    return false; 
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
