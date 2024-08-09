#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "proc.h"

struct queue * initqueue(struct queue *q)
{
    q = (struct queue *) malloc(sizeof(struct queue));
    q->head = NULL;
    q->tail = NULL;

    return q;
}

int isempty(struct queue *q)
{
    return (q->head == NULL && q->tail == NULL);
}

// inserindo processo no final da fila
void enqueue(struct queue *q, struct proc *p)
{
    // adicionando em uma fila vazia
    if (isempty(q))
    {
        // ajustando os ponteiros da fila
        q->head = p;
        q->tail = p;

        // ajustando os ponteiros do processo
        p->prev = NULL;
        p->next = NULL;
    }
    // adicionando ao final da fila
    else
    {
        // ajustando ponteiros do processo
        p->prev = q->tail;
        p->next = NULL;

        // ajustando os ponteiros da fila
        q->tail->next = p;
        q->tail = p;
    }
}


// removendo processo do inicio da fila
struct proc * dequeue(struct queue *q)
{    
    if (isempty(q))
    {
        printf("Fila vazia.\n");
        return NULL;
    }

    // obtendo o processo do inicio da fila
    struct proc *p;
    p = q->head;

    // andando a fila para proximo processo
    q->head = q->head->next;

    if (q->head == NULL)
    {
        q->tail = NULL;
    }

    return p;
}

struct proc * dequeue_bypid(struct queue * q, int pid)
{
    struct proc * aux;

    // removendo o processo selecionado da fila de aptos
    for ( aux = q->head; aux != NULL; aux = aux->next )
    {
        // removendo o processo - aux aponta para ele
        if (aux->pid == pid)
        {
            // tratando o processo prev
            if (aux->prev != NULL)
                aux->prev->next = aux->next;
            else
                // ajustando a fila
                q->head = aux->next;

            // tratando o processo next
            if (aux->next != NULL)
                aux->next->prev = aux->prev;
            else
                // ajustando a fila
                q->tail = aux->prev;

            aux->prev = NULL;
            aux->next = NULL;

            break;
        }
    }

    return aux;
}

void printqueue(struct queue *q)
{
    if (isempty(q))
    {
        printf("Fila vazia.\n");
        return;
    }

    struct proc *aux;

    printf("\t____________ FILA _____________\n");

    printf("\t head: %d\n",q->head->pid);
    printf("\t tail: %d\n",q->tail->pid);
        
    printf("\t_______________________________\n");

    for(aux = q->head; aux != NULL; aux = aux->next)
    {
        printproc(aux);
        printf("\t_______________________________\n");
    }
    
}

int countqueue(struct queue *q)
{
    struct proc *aux;
    int num = 0;

    for(aux = q->head; aux != NULL; aux = aux->next)
        num++;

    return num;
}
