#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/param.h>

#include "queue.h"
#include "proc.h"
#include "utils.h"
#include "time.h"
#include "thread.h"
#include "stats.h"

extern int NPROC;
extern pthread_t sched_tid;

extern int QUANTUM;
extern double INTERRUPT_PROB;

extern int event_num;

extern struct queue * ready;
extern struct queue * ready2;
extern struct queue * blocked;
extern struct queue * finished;

// Utilizado para imprimir o estado do processo
const char * procstates[] = { "READY", "RUNNING", "BLOCKED", "FINISHED" };

extern sigset_t set;
extern int snum;

void * process_thread(void * procp)
{
    // Configurando uma função de handler para caso precise tratar os signals
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = thread_handler;

    if (sigaction(SIGUSR1, &act, NULL) == -1)
        printf("unable to handle siguser1\n");
    if (sigaction(SIGUSR2, &act, NULL) == -1)
        printf("unable to handle siguser2\n");

    // Variável para simular o tempo de processamento do processo
    int process_time, process_time_tmp;

    // Recebendo o ponteiro da estrutura (PCB) do processo
    struct proc * p = (struct proc *)procp;

    // O processo está inciando
    printf("%s PROC - Iniciando processo: %d [%d u.t.]\n", 
           event(), p->pid, p->process_time);

    // A thread do processo inicia em pausa, 
    // para simular a espera na fila de aptos (ready)
    // pause();
    sigwait(&set, &snum);

    // TODO: Ao receber o comando da primeira execução, 
    // TODO: o processo iniciará a partir deste ponto

    // Enquanto o processo estiver rodando
    while (p->state != FINISHED)
    {
        // usleep(1000);

        // TODO: quando um processo ganha o processador, ele começa
        // TODO: a sua execução a partir deste ponto

        printf("%s PROC - Processo %d executando\n", event(), p->pid);
     
        // Simulando o tempo em que o processo ficará executando:
        process_time_tmp = make_process_time(p);

        // O tempo de processamento (a cada execução) não pode ser maior que o QUANTUM
        process_time = MIN(process_time_tmp, QUANTUM);
        
        // Diminuindo o tempo que o processo ainda precisa
        p->remaining_time = p->remaining_time - process_time;

        printf("%s PROC - Processo %d processando por %d u.t. [%d/%d]\n", 
               event(), p->pid, process_time, p->remaining_time, p->process_time);
        
        // Simulando o tempo de processamento (ocupação da CPU) através 
        // de um sleep de microsegundos
        // usleep(2000*process_time);
        usleep(process_time);

        // Verificando se o processo fará E/S ou se irá usar todo o valor do QUANTUM
        if (process_time_tmp >= QUANTUM)
        {
            // NOTE: se usou todo o quando é porque ainda queria processar
            // NOTE: então, saiu por preempção
            // Alterando o status do processo, para apto, 
            p->state = READY;
            
            printf("%s PROC - Processo %d saiu por preempcao\n", event(), p->pid);
        }
        // Processo fez E/S
        else if (p->remaining_time > 0)
        {
            // NOTE: se usou menos tempo que o quantum, é porque saiu por E/S
            // Alterando o status do processo, para bloqueado
            p->state = BLOCKED;
            
            printf("%s PROC - Processo %d saindo por E/S\n", event(), p->pid);
        }
        else
        {
            // Finalizando o processo

            // Alterando o status do processo, para finalizado
            p->state = FINISHED;

            printf("%s PROC - Processo %d finalizando\n", event(), p->pid);
            
            // TODO: remover?
            // printproc(p);
        }
        
        // fazendo a troca de contexto e pausa
        // devolvendo o processamento para a thread do escalonador
        
        context_switch(sched_tid);
        
    }

    return NULL;
}

pthread_t createproc_thread(struct proc *p)
{
    // starting thread time
    set_time_now(&p->time_start);

    pthread_t tid;

    pthread_create(&tid, NULL, process_thread, (void *)p);

    return tid;
}

// Criando um novo processo
struct proc * createproc()
{
    // valor do PID que será usado por todos os processos
    static int nextpid = 0;

    // alocando memória para um novo pcb
    struct proc * p;
    p = (struct proc *) malloc(sizeof(struct proc));

    // preenchendo seus campos

    // informações do processo
    p->pid = nextpid;
    p->state = READY;
    p->queue = 0;

    // informações de tempo
    p->time_start.tv_usec = 0;
    p->time_end.tv_usec = 0;
    p->time_ready.tv_usec = 0;
    // p->time_blocked.tv_usec = 0;
    
    // informações de requisitos de tempo
    p->turnaround_time = 0;
    p->waiting_time = 0;
    p->blocked_time = 0;
    p->process_time = make_time_max(); // int value
    p->remaining_time = p->process_time;

    // estatísticas
    p->num_ready = 0;
    p->num_blocked = 0;
    
    // informações para a thread do processo
    p->tid = (pthread_t)NULL;
    
    // ponteiro para alocação na fila
    p->prev = NULL;
    p->next = NULL;

    // Incrementa o PID para ser atribuido ao proximo processo
    nextpid++;

    return p;
}

// libera memória do processo
void killproc(struct proc *p)
{
    free(p);
}

// Imprime os dados de um processo
void printproc(struct proc *p)
{
    // TODO: ver se há mais algum campo interessante a ser impresso

    printf("\t| pid: %d\n", p->pid);
    printf("\t| state: %s\n", procstates[p->state]);
    printf("\t| queue: %d\n", p->queue);
    printf("\t| time_start: %f\n", convert_time(p->time_start));
    printf("\t| process_time: %d\n", p->process_time);
    printf("\t| remaining_time: %d\n", p->remaining_time);
    printf("\t| waiting_time: %d\n", p->waiting_time);
    printf("\t| blocked_time: %d\n", p->blocked_time);
    printf("\t| turnaround_time: %d\n", p->turnaround_time);
    printf("\t| num_ready: %d\n", p->num_ready);
    printf("\t| num_blocked: %d\n", p->num_blocked);
    printf("\t| tid: %ld\n", p->tid);
    if (p->prev == NULL)
        printf("\t| prev: NULL\n");
    else
        printf("\t| prev: %d\n", p->prev->pid);
    if (p->next == NULL)
        printf("\t| next: NULL\n");
    else
        printf("\t| next: %d\n", p->next->pid);
}

void procend(struct queue *q)
{
    struct proc *p, *aux;

    int i=0;

    // for (p = q->head; p != NULL; p = p->next) 
    p = q->head; 
    while (p != NULL)
    {
        aux = p->next;
        killproc(p);
        p = aux;
    }
}

