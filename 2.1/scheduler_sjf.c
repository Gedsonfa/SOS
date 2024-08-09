#include <stdio.h>

#include "queue.h" // contem funções uteis para filas
#include "proc.h"  // possui as funções dos processos
#include "stats.h" // possui as funções de estatisticas 
#include "utils.h" // possui funções uteis 

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados
// NOTE: essa fila de finalizados é utilizada apenas para
// as estatisticas finais

extern int INT_MAX;

// Estrutura de um processo
struct proc {
    int pid;               // ID do processo
    int remaining_time;    // Tempo restante para execução
    int state;             // Estado atual do processo
    // Outros atributos do processo
};

// Estados possíveis para os processos
enum {
    READY,     // Apto para execução
    BLOCKED,   // Bloqueado esperando um recurso
    FINISHED,  // Processo finalizado
    RUNNING    // Atualmente em execução
};

// Função que implementa o escalonador de processos usando o algoritmo SJF
struct proc * scheduler(struct proc * current) {
    struct proc * selected = NULL; // Processo selecionado para execução

    // Tratamento do processo atual
    if (current != NULL) {
        switch (current->state) {
            case READY:
                // Adicionando no final da fila de aptos
                enqueue(ready, current);
                // Atualizando estatísticas
                count_ready_in(current);
                break;

            case BLOCKED:
                // Adicionando no final da fila de bloqueados
                enqueue(blocked, current);
                // Atualizando estatísticas
                count_blocked_in(current);
                break;

            case FINISHED:
                // Adicionando na fila de finalizados
                enqueue(finished, current);
                // Atualizando estatísticas
                count_finished_in(current);
                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    // Verifica se a fila de aptos está vazia
    if (isempty(ready)) {
        return NULL; // Retorna NULL se não houver processos aptos
    }

    // Inicializa a busca pelo processo com o menor tempo restante
    struct proc * temp;
    int min_remaining_time = INT_MAX; // Define um valor inicial alto

    // Percorre a fila de aptos para encontrar o processo com o menor tempo restante
    int queue_size = size(ready);
    for (int i = 0; i < queue_size; i++) {
        temp = dequeue(ready); // Retira o processo do início da fila

        // Verifica se o processo atual tem o menor tempo restante
        if (temp->remaining_time < min_remaining_time) {
            if (selected != NULL) {
                // Se já havia um processo selecionado anteriormente, reinserir na fila
                enqueue(ready, selected);
            }
            selected = temp; // Atualiza o processo selecionado
            min_remaining_time = temp->remaining_time;
        } else {
            // Reinserir processos não selecionados de volta na fila
            enqueue(ready, temp);
        }
    }

    // Atualiza as estatísticas para o processo que saiu da fila de aptos
    count_ready_out(selected);

    // Altera o estado do processo selecionado para executando
    selected->state = RUNNING;

    // Retorna o processo selecionado para execução
    return selected;
}
