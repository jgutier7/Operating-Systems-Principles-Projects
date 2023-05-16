/* pqsh.c: Process Queue Shell */

#include "../include/pqsh/macros.h"
#include "../include/pqsh/options.h"
#include "../include/pqsh/scheduler.h"
#include "../include/pqsh/signal.h"

#include <errno.h>
#include <string.h>
#include <sys/time.h>

/* Global Variables */

Scheduler PQShellScheduler = {                                                  // default scheduling FIFO 
    .policy    = FIFO_POLICY,
    .cores     = 1,
    .timeout   = 250000,
};

/* Help Message */

void help() {
    printf("Commands:\n");
    printf("  add    command    Add command to waiting queue.\n");
    printf("  status [queue]    Display status of specified queue (default is all).\n");
    printf("  help              Display help message.\n");
    printf("  exit|quit         Exit shell.\n");
}

/* Main Execution */

int main(int argc, char *argv[]) {
    Scheduler *s = &PQShellScheduler;

    /* TODO: Parse command line options */
    if(!parse_command_line_options(argc,argv,s)) {
        return EXIT_FAILURE; 
    }

    /* TODO: Register signal handlers */
    signal_register(SIGALRM,0,sigalrm_handler);

    /* TODO: Start timer interrupt */
    struct itimerval interval = {                                                           // struct variable for timer interrupt
        .it_interval = { .tv_sec = 0, .tv_usec = s->timeout },                              // set interval equal to timeout set by user
        .it_value    = { .tv_sec = 0, .tv_usec = s->timeout },
    };
    if (setitimer(ITIMER_REAL, &interval, NULL) < 0) {
        return EXIT_FAILURE;                                                                // returns error if error
    }


    /* TODO: Process shell comands */
    while (!feof(stdin)) {                                                                  // reads user input until quit | exit 
        char command[BUFSIZ]  = "";
        char argument[BUFSIZ] = "";

        printf("\nPQSH> ");                                                                

        while (!fgets(command, BUFSIZ, stdin) && !feof(stdin));                             // gets each user command and saves it to comman 

        chomp(command);                                                                     // removes trailing newline
        
        /* TODO: Handle add and status commands */
        if (streq(command, "help")) {                                                       // if command is help, show help 
            help();
        } else if (strstr(command, "add") != NULL) {                                        // if user wats to add a process  
            // argument = strchr(command,(int)" ");                                         // returns pointer to string after the word add
            if(strlen(command) > strlen("add")) {
                for(int i = strlen("add"); i < strlen(command) - strlen("add"); i++){       // places the rest of the string into the argument string
                    argument[i - strlen("add")+1] = command[i];
                }
                scheduler_add(s,stdout,argument);                                           // add system argument to scheduler
                fprintf(stdout,"Added process -%s- to waiting queue",command); 
            } else help();                                                                  //  tried to add nothing 
        } else if(strstr(command, "status") != NULL) {                                      // if status is in the command string
            int queue = 0;                                                                      // value for which queue to pass into the status call 
            if(strstr(command, "running") != NULL) queue = RUNNING;
            else if(strstr(command,"waiting") != NULL) queue = WAITING;
            else if (strstr(command,"finished") != NULL)queue = FINISHED;
            scheduler_status(s,stdout,queue);                                                // send to scheduler status 
        } else if (streq(command, "exit") || streq(command, "quit")) {                      // if exit | quit, break from while loop 
            break;
        } else if (strlen(command)) {                                                       // if command is unknown 
            printf("Unknown command: %s\n", command);
        }

        scheduler_next(s);
        scheduler_wait(s); 
    }

    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
