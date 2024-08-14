#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "proc.h"
#include "stats.h"
#include "utils.h"

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de alta prioridade
extern struct queue * ready2;   // fila de baixa prioridade
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados
extern int MAX_TIME;

// Função auxiliar para gerar um número aleatório entre 0 e 99
static int random_0_99() {
    return rand() % 100;
}

struct proc * scheduler(struct proc * current) {
    struct proc * selected = NULL;

    /*
     * Manipulação do processo que está atualmente executando
     */

    if (current != NULL) {
        // Verificando o estado do processo que está sendo executado
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
                printf("@@ ERRO: Estado inválido para o processo %d\n", current->pid);
        }
    }

    /*
     * Estratégia para selecionar um novo processo para executar
     */

    // Verificar se há processos na fila de alta prioridade
    if (!isempty(ready) && random_0_99() < 80) {
        selected = dequeue(ready);
    } 
    // Se a fila de alta prioridade estiver vazia ou a aleatoriedade indica, escolha na fila de baixa prioridade
    else if (!isempty(ready2)) {
        selected = dequeue(ready2);
    } 
    // Se ambas as filas estiverem vazias
    else {
        return NULL;
    }

    // Atualiza as estatísticas para o processo que saiu da fila de prontos
    count_ready_out(selected);

    // Altera o estado do processo selecionado para em execução
    selected->state = RUNNING;

    // Retorna o processo selecionado
    return selected;
}
