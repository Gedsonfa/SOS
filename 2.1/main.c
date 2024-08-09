#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

#include "queue.h"
#include "proc.h"
#include "proc_init.h"
#include "sched.h"
#include "utils.h"
#include "thread.h"

// Número de processos
int NPROC;

// Tempo máximo da execução total de um processo (microsegundos)
int MAX_TIME = 100;

// Tempo máximo da execução de um processo por entrada na CPU (microsegundos)
int QUANTUM = 20;
// int QUANTUM = 100;

// Valor que define a chance de haver uma interrupção
double INTERRUPT_PROB = 0.5;

// Identificador da thread do scheduler
pthread_t sched_tid;

// Ponteiro para o processo que está rodando atualmente
struct proc * running = NULL;

// Ponteiros para as filas
struct queue * ready;       // fila de aptos
struct queue * ready2;      // segunda fila de aptos
struct queue * blocked;     // fila de bloqueados

// NOTE: usado apenas para contabilização final
struct queue * finished;    // fila de processos finalizados

// Global buffer para imprimir debug
char gbuffer[100];

// counter for the number of events of the system
int event_num = 1;

// Semaforo utilizado para fazer o controle da execução da thread scheduling
sem_t sem_scheduling;

// Variaveis utilizadas para definir o conjunto de signals que as threads dos 
// processos irão aceitar
int snum;                                                              
sigset_t set;

int main (int argc, char *argv[])
{
    // Só permite o programa executar se seu uso for correto
    if (argc != 2)
    {
        printf("Uso: %s [numero de processos]\n", argv[0]);
        exit(1);
    }
    
    // Obtem o número de processos informados na linha de comando
    NPROC = atoi(argv[1]);

    if( NPROC <= 0)
    {
        printf("Por favor informe um valor válido [num > 0]\n");
        exit(2);
    }

    // FIX: aparentemente ainda está ocorrendo um travamento esporadicamente, 
    // FIX: verificar se está acontecendo algum caso que deixei passar abaixo.

    /*
    Razão para utilizar o semaforo de escalonamento:
    - Durante a troca de contexto entre as threads dos processos e a thread
      do scheduling, estava havendo um problema quando, em algumas ocasiões,
      a thread scheduling sinalizava a thread do processo, e esta última 
      devolvia a execução tão rapidamente para o scheduling com signal e pausava.
      Como a scheduling ainda não tinha pausado sua execução, ela azia o pause
      após a chegada do signal, ficando ambas threads me pause.
      Com o semaforo, a ideia é que a thread scheduling continua executando os 
      processos por meio de signals, mas os processos liberam a execução da
      scheduling por meio do semaforo. Assim, mesmo que a scheduling ainda não
      tenha bloqueado no semaforo, quando o processo executando liberar o semaforo, 
      a thread scheduling não ficará mais bloqueada. 
      Isto serve como uma liberação que irá ser utilizada pela scheduling 
      quando chegar nesta parte do código, não dependendo mais de um signal posterior.
     */
    sem_init(&sem_scheduling, 0, 0);
 
    // definindo o conjunto de signals que as threads dos processos irão tratar
    sigemptyset(&set); 
    if(sigaddset(&set, SIGUSR1) == -1) 
    {
        perror("Sigaddset error");                                                 
        pthread_exit((void *)1);                                                   
    }
    if(sigaddset(&set, SIGUSR2) == -1) 
    {
        perror("Sigaddset error");                                                  
        pthread_exit((void *)1);                                                    
    }

    // Iniciando a semente do random
    srand(time(NULL));

    printf("%s MAIN - Iniciando filas\n", event());

    // Inicializando as filas
    ready = initqueue(ready);
    ready2 = initqueue(ready2);
    blocked = initqueue(blocked);
    finished = initqueue(finished);
    
    printf("%s MAIN - Iniciando os processos\n", event());

    // Inicia os processos, inserindo-os na fila de aptos
    // NOTE: a fila é selecionada internamente à função, a partir das variaveis globais
    proc_init();

    // FIX: debug?
    printf(">> MAIN - imprimindo fila 'ready':\n");
    printqueue(ready);
    
    // printf("main: fila blocked:\n");
    // printqueue(blocked);

    // printproc(ready->head->next->next);

    printf("%s MAIN - Iniciando o escalonador\n", event());
    
    // call scheduler
    start_scheduler();
    
    // printqueue(finished);

    // printf("%s MAIN - Liberando memória utilizada\n", event());

    // Finalizando os processos
    procend(finished);
    
    // Finalizando o semaforo
    sem_destroy(&sem_scheduling);
    
    // printf("%s MAIN - Finalizando o simulador\n", event());
    
    return 0;
}
