import pandas as pd
import matplotlib.pyplot as plt

# Obter o caminho do arquivo
filePath = input("Insira o endereço do arquivo: ")
data = pd.read_csv(filePath)

plt.figure(figsize=(10, 6))

# Definir os limites dos eixos
plt.xlim(0, data["threads"].max() + 1)
plt.ylim(0, data["searchTime"].max() + 50)

# Definir os intervalos nos eixos
plt.xticks(range(0, int(data["threads"].max()) + 1, 10))        # Intervalo de 10 unidades
plt.yticks(range(0, int(data["searchTime"].max()) + 10, 250))   # Intervalo de 100 unidades

# Plotar Tempo de Busca vs Número de threads
plt.plot(data["threads"], data["searchTime"], marker = "o")
plt.title("Tempo de Busca vs Número de Threads")
plt.xlabel("Número de Threads")
plt.ylabel("Tempo de Busca (ms)")
plt.axis()
plt.grid(True)
plt.show()
