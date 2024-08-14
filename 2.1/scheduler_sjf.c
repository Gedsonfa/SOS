#include <stdio.h>
#include "queue.h"
#include "proc.h"
#include "stats.h"
#include "utils.h"

// Utilizing the global variables defined in 'main'
extern struct queue * ready;    // Queue for ready processes
extern struct queue * ready2;   // Second queue for ready processes
extern struct queue * blocked;  // Queue for blocked processes
extern struct queue * finished; // Queue for finished processes

// Global variable indicating the maximum time a process can execute in total
extern int MAX_TIME;

// Scheduler function implementing the SJF algorithm
struct proc * scheduler(struct proc * current) {
    struct proc * selected = NULL;

    /*
     * Handling the process that is currently executing
     */

    // At this point, the scheduler has the 'current' variable pointing 
    // to the process that just finished executing. It is necessary
    // to verify the state of this process and handle it appropriately:
    //     a) Verify its state
    //     b) Add it to the appropriate queue based on its state
    //     c) Perform accounting based on its new queue

    // Handle the case where the system is starting, and there's no one running
    if (current != NULL) {
        // Checking the state of the executing process
        switch (current->state) {
            // If it's 'ready', it was preempted
            case READY:
                // Add to the end of the ready queue
                enqueue(ready, current);

                // Update statistics for the process entering the ready queue
                count_ready_in(current);

                break;

            // If the process is 'blocked', it performed I/O
            case BLOCKED:
                // Add to the end of the blocked queue
                enqueue(blocked, current);

                // Update statistics for the process entering the blocked queue
                count_blocked_in(current);

                break;

            // If the process has finished
            case FINISHED:
                // Add to the finished queue
                enqueue(finished, current);

                // Update statistics for the process entering the finished queue
                count_finished_in(current);

                break;

            default:
                printf("@@ ERROR: Invalid state for process %d\n", current->pid);
        }
    }

    /*
     * Strategy for selecting a new process to execute
     */

    // If the ready queue is empty, there is nothing to do
    if (isempty(ready)) {
        // printf("== sched SJF: READY queue is empty\n");
        return NULL;
    }

    // Implementing SJF strategy:
    struct proc * temp = ready->head;
    int min_remaining_time = MAX_TIME + 1;  // Start with a value larger than any possible remaining time

    while (temp != NULL) {
        // Select the process with the minimum remaining time
        if (temp->remaining_time < min_remaining_time) {
            min_remaining_time = temp->remaining_time;
            selected = temp;
        }

        temp = temp->next;
    }

    // Remove the selected process from the ready queue by PID
    dequeue_bypid(ready, selected->pid);

    // Update statistics for the process leaving the ready queue
    count_ready_out(selected);

    // Change the state of the selected process to running
    selected->state = RUNNING;

    // Return the selected process
    return selected;
}
