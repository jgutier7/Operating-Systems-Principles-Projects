/* timestamp.c: PQSH Timestamp */

#include "../include/pqsh/timestamp.h"
#include <time.h>
#include <sys/time.h>

/**
 * Return current timestamp as a double.
 *
 * Utilizes gettimeofday for precision (accounting for seconds and
 * microseconds) and falls back to time (only account for seconds) if that
 * fails.
 *
 * @return  Double representing the current time.
 **/
double timestamp() {
    double time_stamp;                                                              // return value 
    struct timeval timed;                                                           // time variable to hold the struct returned by gettimeofday
 
    if(gettimeofday(&timed,NULL) != 0){                                             // gets time of day && check if fails
        time_t seconds = time(NULL);                                            
        return seconds;                                                             // returns seconds instead
    } 

    time_stamp = (double)timed.tv_sec+((double)timed.tv_usec / 1000000.0);          //converts time of day into seconds 
    return time_stamp;                                                              //returns the time of day in seconds 
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
