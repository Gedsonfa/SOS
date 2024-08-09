
// estrutura de uma fila generica
struct queue
{
    struct proc * head;
    struct proc * tail;
};

// inicializa uma fila
struct queue * initqueue(struct queue *q);

// verifica se uma fila está vazia
int isempty(struct queue *q);

// adiciona um elemento no final da fila
void enqueue(struct queue *q, struct proc *p);

// remove um elemento do inicio da fila
struct proc * dequeue(struct queue *q);

// remove um elemento da fila pelo seu pid
struct proc * dequeue_bypid(struct queue * q, int pid);

// imprime todos os elementos de uma fila
void printqueue(struct queue *q);

// conta quantos elementos há em uma fila
int countqueue(struct queue *q);

