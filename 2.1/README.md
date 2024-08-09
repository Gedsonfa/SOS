# sched_sim - Scheduler Simulator

Simulador de Escalonador baseado em POSIX threads.

Este é um simulador de escalonador educacional, a ser utilizado na disciplina
DCT2101 - Sistemas Operacionais, do BSI, CERES-UFRN.

Esta versão possui a implementação do algoritmo de escalonamento FIFO, sendo
requisitados a implementação dos demais algoritmos de escalonamento.

## Compilação

Para compilação de todos os códigos:

```
make all
```

Para compilação individual dos escalonadores:

FIFO:

```
make fifo
```

SJF:

```
make sjf
```

LJF:

```
make ljf
```

PRIO STATIC:

```
make prio_static
```

PRIO DYNAMIC:

```
make prio_dynamic
```

Serão gerados arquivos executáveis para cada um dos escalonadores.

## Execução

Para executar os escalonadores:

```
./main_fifo numero_de_processos
```

