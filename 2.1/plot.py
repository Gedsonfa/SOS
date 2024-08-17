import matplotlib.pyplot as plt

# Quantidade de processos
num_processos = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]

# Médias calculadas
medias = {
    "FIFO": 7258.03,
    "SJF": 4138.88,
    "LJF": 6650.94,
    "PRIO-STATIC": 7885.98,
    "PRIO-DYNAMIC": 8368.29
}

# Variâncias calculadas
variancias = {
    "FIFO": 9016842.46,
    "SJF": 5996012.43,
    "LJF": 12173415.74,
    "PRIO-STATIC": 18806308.03,
    "PRIO-DYNAMIC": 36187532.95
}

# Criando o gráfico
plt.figure(figsize=(10, 6))

for algoritmo in dados.keys():
    plt.errorbar(num_processos, dados[algoritmo], yerr=np.sqrt(variancias[algoritmo]), label=algoritmo, capsize=5)

plt.xlabel("Quantidade de Processos")
plt.ylabel("Média do Tempo de Espera (TME)")
plt.title("Tempo Médio de Espera (TME) com Barras de Erro")
plt.legend()
plt.grid(True)
plt.show()
