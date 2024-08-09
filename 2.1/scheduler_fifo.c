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

// variavel global que indica o tempo maximo que um processo pode executar ao todo
extern int MAX_TIME;

struct proc * scheduler(struct proc * current)
{
    struct proc * selected; 

    /*
     *   Tratando o processo que está atualmente executando
     */

    // TODO: Neste ponto o escalonador tem a variavel 'current' apontando 
    // TODO: para o processo que acabou de sair da execução, é necessário
    // TODO: verificar o estado em que este processo está, e fazer o seu 
    // TODO: devido tratamento: 
    //          a) verificar o seu estado
    //          b) adicioná-lo na fila adequada a seu estado
    //          c) realizar a contabilização de acordo com sua nova fila

    // Para tratar o caso do início do sistema, quando nao tem ninguem rodando
    if (current != NULL)
    {
        // Verificando o estado em que o processo executando está
        switch (current->state) 
        {
            // Caso esteja 'apto' é porque saiu por preempção
            case READY:

                // Adicionando no final da fila de aptos
                enqueue(ready, current);

                // Realizando as estatisticas para o processo que 
                // entra na fila de aptos
                count_ready_in(current);
                
                break;

            // Caso o processo tenha saído como 'bloqueado', é porque fez uma E/S
            case BLOCKED:

                // Adicionando no final da fila de bloqueados
                enqueue(blocked, current);
                
                // Realizando as estatisticas para o processo que 
                // entra na fila de bloqueados
                count_blocked_in(current);

                break;

            // Caso o processo tenha finalizado, estado 'finished'
            case FINISHED:

                // Adicionando-o na fila de finalizados
                enqueue(finished, current);

                // Realizando as estatisticas para o processo que
                // entra na fila de finalizados
                count_finished_in(current);

                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    /*
     *   Estratégia de seleção de um novo processo para executar
     */

    // Se a fila de aptos está vazia, nao há o que fazer
    if (isempty(ready))
    {
        // printf("== sched FIFO: fila READY is empty\n");
        return NULL;
    }

    // TODO: Neste ponto, é preciso implementar a estrategia do algoritmo
    // TODO: utilizado, para selecionar um processo a ser selecionado.

    // Estratégia para o FIFO:

    // printf(" FILA READY ANTES\n");
    // printqueue(ready);

    // Pegando o primeiro processo da fila de aptos para executar
    selected = dequeue(ready);
    
    // printf(" FILA READY DEPOIS\n");
    // printqueue(ready);
    
    // TODO: Neste ponto, é preciso verificar de qual fila o processo foi
    // TODO: selecionado, e fazer o seu devido tratamento:
    //          a) fazer a contabilização de acordo com a fila que saiu
    //          b) alterar o estado do processo para executando

    // WARNING: ATENÇÃO: é necessário fazer isto antes de alterar o estado do processo
    // Realizando as estatisticas para o processo que
    // saiu da fila de aptos
    count_ready_out(selected);

    // Alterando o estado do processo selecionado para executando
    selected->state = RUNNING;
        
    // Retornando o processo selecionado
    return selected;
}

