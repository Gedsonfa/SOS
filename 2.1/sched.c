#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "queue.h"
#include "proc.h"
#include "thread.h"
#include "time.h"
#include "stats.h"
#include "utils.h"
#include "proc_interrupt.h"
#include "scheduler.h"

extern int NPROC;

extern pthread_t sched_tid;
extern int event_num;

extern struct proc * running;
extern struct queue * ready;
extern struct queue * ready2;      // segunda fila de aptos
extern struct queue * blocked;
extern struct queue * finished;

// TODO: essa é a funcao do escalonamento
void * scheduling_thread()
{
    printf("%s SCHED -> Escalonador iniciando\n",event());

    struct proc *p;

    while (countqueue(finished) != NPROC)
    {
        // checando se há alguem bloqueado
        if (! isempty(blocked))
        {
            // printf("== sched: fila blocked not empty\n");

            // verifica se houve interrupção para ele
            if (interruption())
            {
                // remove o primeiro processo bloqueado
                p = dequeue(blocked);

                // Contabilizando o tempo que ele ficou 
                // na fila de bloqueados
                count_blocked_out(p);
                
                printf("%s SCHED -> Interrupcao ocorreu para o processo %d\n", event(), p->pid);

                // definindo o que fazer com o processo ao voltar de bloqueado
                proc_interrupt(p);
           }
            else
            {
                // TODO: isso deveria gerar um event_num?
                printf("%s SCHED -> Interrupcao nao occorreu!\n",event());
            }
        }
        
        // TODO: AQUI ACONTECE O ESCALONAMENTO !!
        
        // Usando o escalonador definido para selecionar o proximo processo a executar
        // Passando o processo corrente para ser manipulado
        running = scheduler(running);

        // Se não houve processo apto a selecionar,
        // continua o looping até que algum processo apareça
        if (running == NULL)
        {
            continue;
        }

        printf("%s SCHED -> Selecionado o processo: %d\n", event(), running->pid);

        // FIX: ver se isso ficará no DEBUG
        // printf("sched - fila ready:\n");
        // printqueue(ready);

        
        // Fazendo a troca de contexto para o próximo processo
        context_switch(running->tid);
    }

    // printqueue(finished);

    printf("%s SCHED -> Escalonador finalizando\n", event());
    
    // Realizando a contabilização final
    accounting(finished);

    return NULL;
}

void start_scheduler()
{
    // create scheduling thread
    pthread_create(&sched_tid, NULL, scheduling_thread, NULL);

    pthread_join(sched_tid, NULL);
}

