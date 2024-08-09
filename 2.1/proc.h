#include <time.h>
#include <sys/time.h>
#include <pthread.h>

// process states
//enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
enum procstate { READY, RUNNING, BLOCKED, FINISHED };

// process control block
struct proc
{
    // informações do processo
    unsigned int pid;           // Process ID
    enum procstate state;       // Process state
    unsigned int queue;         // Queue the process is inserted

    // auxiliares de tempo
    struct timeval time_start;  // Tempo de inicio da execucao
    struct timeval time_end;    // Tempo de fim da execuçao
    struct timeval time_ready;  // Tempo de entrada na fila de aptos
    struct timeval time_blocked;  // Tempo de entrada na fila de bloqueados
    // FIX: precisa desse tempo de blocked?
    // struct timeval time_blocked;// Tempo de entrada na fila de bloqueados

    // informações de tempo
    int turnaround_time;        // Tempo total de execução do processo
    int waiting_time;           // Tempo que ficou na fila de aptos
    int blocked_time;           // Tempo que ficou na fila de bloqueados
    int process_time;           // Tempo de processamento que o processo precisa
    int remaining_time;         // Tempo de processamento restante do processo

    // estatisticas
    unsigned int num_ready;     // Numero de vezes que ficou na fila de aptos
    unsigned int num_blocked;   // Numero de vezes que ficou na fila de bloqueados

    // informações para a thread
    pthread_t tid;

    // ponteiro para alocação na fila
    struct proc *prev;
    struct proc *next;
};

void * process_thread(void *);

pthread_t createproc_thread(struct proc *p);

struct proc * createproc();

void killproc(struct proc *p);

void printproc(struct proc *p);

void procend(struct queue * q);

