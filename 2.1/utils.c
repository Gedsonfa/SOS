#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "time.h"

// contagem global dos eventos
extern int event_num;

// buffer global para formatar a linha de debug de eventos
extern char gbuffer[100];

double make_rand(int limit)
{
    // 0 a (limit - 1)
    return rand() % limit;
}

// retorna a contagem de eventos e o tempo em que ele ocorreu
char * event()
{
    // print com time
    struct timeval t;
    gettimeofday(&t, NULL);

    snprintf(gbuffer, sizeof(gbuffer), "[%d - %f]", event_num++, convert_time(t));

    // print sem time
    // snprintf(gbuffer, sizeof(gbuffer), "[%d]", event_num++);

    return gbuffer;
}

