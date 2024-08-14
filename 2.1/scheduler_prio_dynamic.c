#include <stdio.h>
#include <stdlib.h>

#include "queue.h" // contém funções úteis para filas
#include "proc.h"  // possui as funções dos processos
#include "stats.h" // possui as funções de estatísticas 
#include "utils.h" // possui funções úteis 

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados

// Variável global que indica o tempo máximo que um processo pode executar ao todo
extern int MAX_TIME;

struct proc * scheduler(struct proc * current)
{
    struct proc * selected = NULL; 

    /*
     *   Tratando o processo que está atualmente executando
     */
    if (current != NULL)
    {
        // Verificando o estado em que o processo executando está
        switch (current->state) 
        {
            case READY:
                // Processo foi preemptado, volta para a segunda fila (ready2)
                enqueue(ready2, current);
                count_ready_in(current); // Estatísticas para a fila 2
                break;

            case BLOCKED:
                // Processo fez I/O, volta para a primeira fila (ready)
                enqueue(ready, current);
                count_blocked_in(current); // Estatísticas para a fila 1
                break;

            case FINISHED:
                // Processo finalizou
                enqueue(finished, current);
                count_finished_in(current);
                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    /*
     *   Estratégia de seleção de um novo processo para executar
     */

    // Se ambas as filas de aptos estão vazias, não há o que fazer
    if (isempty(ready) && isempty(ready2))
    {
        return NULL;
    }

    // Implementação da estratégia PRIO DYNAMIC com probabilidade
    int random_value = rand() % 100;

    if ((random_value < 80 && !isempty(ready)) || isempty(ready2))
    {
        // 80% de chance de selecionar da fila 1, ou se a fila 2 está vazia
        selected = dequeue(ready);
        count_ready_out(selected);
    }
    else
    {
        // 20% de chance de selecionar da fila 2
        selected = dequeue(ready2);
        count_ready_out(selected);
    }

    if (selected != NULL)
    {
        // Alterando o estado do processo selecionado para executando
        selected->state = RUNNING;
    }
        
    // Retornando o processo selecionado
    return selected;
}
