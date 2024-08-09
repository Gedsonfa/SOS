#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/param.h>

#include "queue.h"
#include "proc.h"

extern int MAX_TIME;
extern double INTERRUPT_PROB;

int make_time_max()
{
    return rand() % (MAX_TIME + 1);
}

int make_time_limit(int limit)
{
    return rand() % (limit + 1);
}

int make_process_time(struct proc *p)
{
    // No mínimo 1 u.t. é retornada
    return MAX(make_time_limit(p->remaining_time), 1);
}

double make_time()
{
    return (100 * (double) random() / (double) RAND_MAX);
}

void set_time_now(struct timeval * t)
{
    // definindo o tempo atual
    gettimeofday(t, NULL);
}

double set_time_diff_now(struct timeval t)
{
    // definindo o tempo atual
    struct timeval now;
    gettimeofday(&now, NULL);

    // computando a diferença de tempo
    // return ((double)(now.tv_sec + now.tv_usec*1.e-6) - (t.tv_sec + t.tv_usec*1.e-6));
    return ((double)(now.tv_sec/1.e-6 + now.tv_usec) - (t.tv_sec/1.e-6 + t.tv_usec));
}


double convert_time(struct timeval t)
{
    return (double)(t.tv_sec + t.tv_usec*1.e-6);
}

int interruption()
{
    // double r = ((double) random() / (double) RAND_MAX);
    // printf("INTERRUPT rand %f prob: %f\n", r, INTERRUPT_PROB);
    // return r <= INTERRUPT_PROB;
    return (((double) random() / (double) RAND_MAX) <= INTERRUPT_PROB);
}

