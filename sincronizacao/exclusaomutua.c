#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock;
pthread_cond_t cond0, cond1;
int turn = 0;

pthread_t tid0, tid1;

long conta = 0;

void * t0()
{
    long i;
    for (i = 0; i < 1000000; i++)
    {

            while (turn != 0);
            conta = conta + 5;
            turn = 0;

    }

    printf("Encerrei t0\n");
}

void * t1()
{
    long i;
    for (i = 0; i < 1000000; i++)
    {
        while (turn != 1);
            conta = conta + 2;
            turn = 0;
    }

    printf("Encerrei t1 \n");
}

int main (int argc, char ** argv)
{
    pthread_create(&tid0, NULL, t0, NULL);
    pthread_create(&tid1, NULL, t1, NULL);

    pthread_join(tid0, NULL);
    pthread_join(tid1, NULL);

    printf("O valor de conta eh: %ld\n", conta);

    return 0;
}
