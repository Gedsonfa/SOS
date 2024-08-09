#include <stdio.h>
#include <limits.h>  // Para usar INT_MAX

#include "queue.h" 
#include "proc.h"  
#include "stats.h" 
#include "utils.h" 

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    
extern struct queue * ready2;   
extern struct queue * blocked;  
extern struct queue * finished; 



struct proc * scheduler(struct proc * current) {
    struct proc * selected = NULL;

    if (current != NULL) {
        switch (current->state) {
            case READY:
                enqueue(ready, current);
                count_ready_in(current);
                break;

            case BLOCKED:
                enqueue(blocked, current);
                count_blocked_in(current);
                break;

            case FINISHED:
                enqueue(finished, current);
                count_finished_in(current);
                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    if (isempty(ready)) {
        return NULL;
    }

    struct proc * temp;
    int min_remaining_time = INT_MAX;

    

        if (temp->remaining_time < min_remaining_time) {
            if (selected != NULL) {
                enqueue(ready, selected);
            }
            selected = temp;
            min_remaining_time = temp->remaining_time;
        } else {
            enqueue(ready, temp);
        }
    }

    
