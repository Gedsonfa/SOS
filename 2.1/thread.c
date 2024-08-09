#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

extern pthread_t sched_tid;

extern sigset_t set;
extern int snum;

extern sem_t sem_scheduling;

// Funcao para tratar algum signal recebido
void thread_handler(int sig)
{
    switch(sig)
    {
        // pause the process thread
        case SIGUSR1:
            pause();
            break;
        // unpause a process thread
        case SIGUSR2:
            break;
    }
}

// Funcao que simula a troca de contexto mas, neste caso, a troca 
// de contexto sempre acontece do scheduling para um processo, e vice-versa
void context_switch(pthread_t tid)
{
    // Se é o scheduling, executa a thread 'tid' e pausa
    // A execução do scheduling é feita por meio do semaforo
    if (pthread_self() == sched_tid)
    {
        // unpause the 'tid' thread
        pthread_kill(tid, SIGUSR2);

        // bloqueia no semaforo
        sem_wait(&sem_scheduling);
    }
    // Se é outro processo, então libera a execução do scheduling
    // Em seguida, pausa aguardando um sinal
    else
    {
        // desbloqueia o scheduling
        sem_post(&sem_scheduling);

        // pause current thread
        // pause();
        sigwait(&set, &snum);  
        // NOTE: using sigwait instead of pause to take more controle on signals
    }
       
    // old version
    // pthread_kill(tid, SIGUSR2);
    // //pause();
    // sigwait(&set, &snum);   
}

