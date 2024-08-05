#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

// quantidade de leitores lendo
int readcount = 0;

//
// TODO: Definição dos semáforos (variaveis precisam ser globais)
sem_t mutex, wrt;

// dado compartilhado que os leitores e escritores acessarão
int shared = 0;

// protótipos das funções
void *leitor(void *id);
void *escritor(void *id);
int gera_rand(int limit);

int main(int argc, char **argv) 
{
    // threads dos leitores
    pthread_t *tl;

    // threads dos escritores
    pthread_t *te;
    long i;

    srand(time(NULL));

    if (argc < 3) {
        printf("Usage: %s num_leitores num_escritores\n", argv[0]);
        return 0;
    }

    //
    // TODO: Criação dos semáforos (aqui é quando define seus
    // valores)
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    // num leitores
    int N_LEITORES = atoi(argv[1]);

    // num escritores
    int N_ESCRITORES = atoi(argv[2]);

    // gerando uma lista de threads de leitores
    tl = malloc(N_LEITORES * sizeof(pthread_t));

    // gerando uma lista de threads de escritores
    te = malloc(N_ESCRITORES * sizeof(pthread_t));

    // iniciando as threads dos leitores
    for (i = 0; i < N_LEITORES; i++) {
        pthread_create(&tl[i], NULL, leitor, (void *)i);
    }

    // Criando e iniciando threads de escritores
    for (i = 0; i < N_ESCRITORES; i++) {
        pthread_create(&te[i], NULL, escritor, (void *)i);
    }

    // aguardando as threads leitoras terminarem
    for (i = 0; i < N_LEITORES; i++) {
        pthread_join(tl[i], NULL);
    }

    // aguardando as threads escritores terminarem
    for (i = 0; i < N_ESCRITORES; i++) {
        pthread_join(te[i], NULL);
    }

    //
    // TODO: Excluindo os semaforos
    sem_destroy(&mutex);
    sem_destroy(&wrt);

    // liberando a memoria alocada
    free(tl);
    free(te);

    return 0;
}

void *leitor(void *id) 
{
    usleep(gera_rand(1000000));
    
    // convertendo o Id do leitor para int
    long i = (long)id;
    
    printf("> Leitor %ld tentando acesso\n", i);

    sem_wait(&mutex);
    readcount++;
    if (readcount == 1) {
        sem_wait(&wrt);
    }
    sem_post(&mutex);

    printf("> Leitor %ld conseguiu acesso\n", i);
    int shared_in = shared;
    usleep(gera_rand(1000000));

    printf("\t> Leitor %ld - tmp: %d - shared: %d - readcount: %d\n", i, shared_in, shared, readcount);

    if (shared_in != shared) {
        printf("\t==== ALERTA DO LEITOR ====\n");
        printf("\t> Valor interno diferente do compartilhado\n\tshared_in: %d - shared: %d\n", shared_in, shared);
        printf("\t==========================\n");
    }

    sem_wait(&mutex);
    readcount--;
    if (readcount == 0) {
        sem_post(&wrt);
    }
    sem_post(&mutex);
    printf("< Leitor %ld liberando acesso\n", i);
}

void *escritor(void *id) 
{
    usleep(gera_rand(1000000));
    
    long i = (long)id;
    
    printf("+ Escritor %ld tentando acesso\n", i);

    //
    // TODO: precisa controlar o acesso do escritor ao recurso
    sem_wait(&wrt);

    printf("\t+ Escritor %ld conseguiu acesso\n", i);

    if (readcount > 0)
    {
        printf("==== ALERTA DO ESCRITOR ====\n");
        printf("Readcount possui valor: %d\n",readcount);
        printf("============================\n");
    }

    int rnd = gera_rand(100);
    printf("\t+ Escritor %ld gravando o valor %d em shared\n", i, rnd);
    usleep(gera_rand(1000000));
    shared = rnd;

    //
    // TODO: precisa fazer a saída do escritor e liberação do acesso
    sem_post(&wrt);

    printf("+ Escritor %ld saindo\n", i);
}

int gera_rand(int limit) 
{
    // 0 a (limit -1)
    return rand() % limit;
}
