#include <stdio.h>

#include "queue.h"
#include "proc.h"
#include "stats.h"

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos

// para tratar o que fazer com o processo ao ocorrer a 
// interrupçao que define sua saída da fila de bloqueados
void proc_interrupt(struct proc * p)
{
    // insere o processo no final da fila de aptos
    enqueue(ready, p);

    // alterando o status para apto
    p->state = READY;

    // Realizando as estatisticas para o processo que 
    // entra na fila de aptos
    count_ready_in(p);
}

