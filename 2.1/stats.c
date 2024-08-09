#include <stdio.h>

#include "queue.h"
#include "proc.h"
#include "time.h"

extern int NPROC;

// Contabilização do processo ao entrar (IN) na fila de aptos
void count_ready_in(struct proc *p)
{
    // Contabilizando o número de entradas na fila de aptos
    p->num_ready++;

    // Tempo que o processo entra na fila de aptos
    set_time_now(&p->time_ready);
}

// Contabilização do processo ao sair (OUT) da fila de aptos
void count_ready_out(struct proc *p)
{
    // Contabilizando o tempo que ele ficou na fila de aptos
    p->waiting_time += set_time_diff_now(p->time_ready);
}

// Contabilização do processo ao entrar (IN) na fila de bloqueados
void count_blocked_in(struct proc *p)
{
    // Contabilizando o número de entradas na fila de aptos
    p->num_blocked++;

    // Tempo que o processo entra na fila de bloqueados
    set_time_now(&p->time_blocked);
}

// Contabilização do processo ao sair (OUT) da fila de bloqueados
void count_blocked_out(struct proc *p)
{
    // Contabilizando o tempo que ele ficou na fila de bloqueados
    p->blocked_time += set_time_diff_now(p->time_blocked);
}

// Contabilização do processo ao entrar (IN) na fila de finalizados
void count_finished_in(struct proc *p)
{
    // Contabilizando o tempo de execucao total do processo
    p->turnaround_time = set_time_diff_now(p->time_start);
}

// Contabiliza as métricas para analisar os algoritimos de escalonamento
void accounting(struct queue *q)
{
    struct proc *aux;

    // Tempo Médio de Espera
    double TME = 0;
    
    // Tempo Médio Bloqueados
    double TMB = 0;
    
    // Tempo Médio de Turnaround
    double TMT = 0;

    // Total de vezes que os processos entraram na fila de aptos
    double num_ready = 0;
    
    // Total de vezes que os processos entraram na fila de bloqueados
    double num_blocked = 0;

    printf("\n_________ ESTATISTICAS ________\n\n");
    
    // looping nos processos da fila
    for(aux = q->head; aux != NULL; aux = aux->next)
    {
        // printproc(aux);
        // printf("\t--------------------\n");

        TME += aux->waiting_time;
        num_ready += aux->num_ready;
        
        TMB += aux->blocked_time;
        num_blocked += aux->num_blocked;

        TMT += aux->turnaround_time;
    }

    printf("TME: %f\n", (double)TME/(double)num_ready);
    
    printf("TMB: %f\n", (double)TMB/(double)num_blocked);
    
    printf("TMT: %f\n", (double)TMT/(double)NPROC);
    
    printf("_______________________________\n");

}

