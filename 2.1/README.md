A tarefa consistirá em, com base na implementação citada, implementar os seguintes algoritmos de escalonamento:

(a) SJF (Shortest Job First)

O escalonador deverá, a cada rodada, selecionar o processo que possui menor tempo restante (remaining_time).

(b) LJF (Longest Job First)

Este é um escalonador apenas de testes, pois pode produzir resultados não ideais.

Ele é o inverso do SJF.

O escalonador deverá, a cada rodada, selecionar o processo que possui maior tempo restante (remaining_time).

(c) PRIO STATIC: Com prioridades estáticas

Possui duas filas de prioridades.
Não há realimentação entre as filas.
Os processos devem ser inseridos em sua fila no início da simulação.
- Para isto, deve-se modificar o arquivo proc_init_prio_static.c.
- Processos com menor tempo restante deverão ir para a primeira fila (ready).
- Processos com maior tempo restante deverão ir para a segunda fila (ready2).