#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

// Semáforos para exclusão mútua e controle da disponibilidade dos garfos
sem_t *chopstick_sem;  // Semáforo para cada garfo (chopstick)
sem_t mutex;            // Semáforo de exclusão mútua

int *chopstick_use;

// numero de filosofos
int N_FILOS;

// prototipos das funcoes
void * filosofo(void *);
void pegar(int, int);
void liberar(int, int);
int gera_rand(int);

int main(int argc, char ** argv)
{
    // threads dos filosofos
    pthread_t * tids;
    long i;

    srand(time(NULL));

    if ( argc < 2 )
    {
        printf("Usage: %s num_filosofos\n", argv[0]);
        return 0;
    }

    // numero de filosofos
    N_FILOS = atoi(argv[1]);

    // gerando uma lista de threads de filosofos
    tids = malloc(N_FILOS * sizeof(pthread_t));

    // gerando uma lista de uso dos chopsticks
    chopstick_use = malloc(N_FILOS * sizeof(int));

    // um chopstick esta livre se estiver -1
    for (i = 0; i < N_FILOS; i++)
    {
        chopstick_use[i] = -1;
    }
 
    // iniciando as threads dos filosofos
    chopstick_sem = malloc(N_FILOS * sizeof(sem_t));
    for (i = 0; i < N_FILOS; i++)
    {
        sem_init(&chopstick_sem[i], 0, 1); // Inicialização dos semáforos dos garfos
    }
    sem_init(&mutex, 0, 1); // Inicialização do semáforo de exclusão mútua

    // aguardando as threads dos filosofos terminarem
    for (i = 0; i < N_FILOS; i++)
    {
        pthread_create(&tids[i], NULL, filosofo, (void *)i);
    }

    for (i = 0; i < N_FILOS; i++)
    {
        pthread_join(tids[i], NULL);
    }

    // liberando a memoria alocada
    for (i = 0; i < N_FILOS; i++)
    {
        sem_destroy(&chopstick_sem[i]); // Destruição dos semáforos dos garfos
    }
    sem_destroy(&mutex); // Destruição do semáforo de exclusão mútua

    free(tids);
    free(chopstick_use);
    free(chopstick_sem);

    return 0;
}

void * filosofo(void * id)
{
    // convertendo o Id do filosofo para int
    long i = (long)id;
    
    printf("\t> Filosofo %d pensando\n",i);
    usleep(gera_rand(1000000));

    int c1, c2;

    // OBS: alterando a ordem de pegar o chopstick para evitar deadlock
    if (i%2 == 0) // com base no id do filosofo (par ou impar)

    {
        c1 = i;             // esquerda
        c2 = (i+1)%N_FILOS; // direita
    }
    else
    {
        c1 = (i+1)%N_FILOS; // direita
        c2 = i;             // esquerda
    }

    pegar(i, c1); 
    pegar(i, c2);
    
    printf("\t> Filosofo %d comendo\n",i);
    usleep(gera_rand(1000000));
    
    liberar(i, c1);
    liberar(i, c2);
}

// filosofo 'i' quer pegar o chopstick definido por 'num'
void pegar(int i, int num)
{
    sem_wait(&chopstick_sem[num]); // Espera até que o garfo esteja disponível
    sem_wait(&mutex); // Entrada na região crítica
    if (chopstick_use[num] != -1)
    {
        printf("===== ALERTA DO FILOSOFO %d =====\n===== CHOPSTICK[%d] EM USO POR %d =====\n",
                i,num,chopstick_use[num]);
    }
    chopstick_use[num] = i;
    printf("+ Filosofo %d pegou o chopstick[%d]\n",i,num);
    sem_post(&mutex); // Saída da região crítica
}

// filosofo 'i' quer liberar o chopstick definido por 'num'
void liberar(int i, int num)
{
    sem_wait(&mutex); // Entrada na região crítica
    chopstick_use[num] = -1;
    printf("- Filosofo %d liberou o chopstick[%d]\n",i,num);
    sem_post(&mutex); // Saída da região crítica
    sem_post(&chopstick_sem[num]); // Libera o garfo
}

int gera_rand(int limit)
{
    // 0 a (limit -1)
    return rand()%limit;
}
