# Project 01: Process Queue Shell

This is [Project 01] of [CSE.30341.FA22].

## Students

- JACOB GUTIERREZ (JGUTIER7@ND.EDU)


VIDEO: 
https://youtu.be/6O5xQ-YB474  

## Brainstorming

The following are questions that should help you in thinking about how to
approach implementing [Project 01].

### Process

1. Given a command string such as `sleep 10`, how would you start a process
   that executes this command?  In particular, how would you handle the name of
   the program (ie. `sleep`) versus the arguments to the program (ie. `10`)?


2. What signals do you need to send to a process to **pause** it (ie. suspend
   its execution) or to **resume** it (ie. continue its execution)?

	SIGSTOP and SIGCONT

### Queue

1. What must happen when **pushing** a process to a `Queue`?
	Queue must be updated and the new process must be at the end of it. 
	Updating means that we set the new process as the tail of the list. 

2. What must happen when **popping** a process from a `Queue`?
	When we pop, we set the next process asa the head of the list. 

3. What must happen when **removing** a process from a `Queue`?
	When removing, we have to adjust the list accordingly. 

### Shell

1. What **system call** will allow us to trigger a timer event at periodic
   intervals?  What signal does it send when a timer is triggered?

2. What functions would allow you to parse strings with various arguments?

### Scheduler

1. What needs to be created when calling `scheduler_add` and where should this
   object go?

2. How would you determine if you should display a particular queue in the
   `scheduler_status` function?	


3. How would you wait for a process without blocking? What information do you
   need to update when a process terminates?

### Signal

1. What should happen in the `Scheduler` when a timer event fires?
	The scheduler should be choosing which process is going to be running next.

### Timestamp

1. How would you use the result of `gettimeofday` to return a `double`
   representing the current time in **seconds**?

## Errata

> Describe any known errors, bugs, or deviations from the requirements.


[Project 01]:       https://www3.nd.edu/~pbui/teaching/cse.30341.fa22/project01.html
[CSE.30341.FA22]:   https://www3.nd.edu/~pbui/teaching/cse.30341.fa22/
