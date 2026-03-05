import sys
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import PrecisionRecallDisplay, auc

# Calcula a Average Precision (AP) a partir das listas de precisão e recall.
def calcular_ap(precisoes, recall):
    
    # Ordena os índices pelo valor de recall
    indices = np.argsort(recall)  
    # Garante que começa em 0 e termina em 1
    recall = np.concatenate(([0], recall[indices], [1]))  
    # Garante que começa em 1 e termina em 0
    precisoes = np.concatenate(([1], precisoes[indices], [0]))  
    # Calcula a área sob a curva
    return auc(recall, precisoes)  

# Função principal que carrega os dados, processa métricas e gera o gráfico P-R.
def main():

    try:
        # Carregar os dados a partir do arquivo fornecido
        arquivo = np.loadtxt(sys.argv[2])  # Carrega os dados do arquivo (espera-se colunas: índice, precisão, recall, F1-score, referência opcional)
        precisao = arquivo[:, 1]  # Segunda coluna: precisão
        recalls = arquivo[:, 2]     # Terceira coluna: recall
        f1_scores = arquivo[:, 3]   # Quarta coluna: F1-score

        # Calcular a métrica Average Precision (AP)
        ap = calcular_ap(precisao, recalls)

        # Encontrar o melhor ponto de F1-score
        melhor_f1_indice = np.argmax(f1_scores)  
        melhor_f1 = f1_scores[melhor_f1_indice]  
        melhor_precisao = precisao[melhor_f1_indice] 
        melhor_recall = recalls[melhor_f1_indice]

        # Definir um nível de referência (caso a quinta coluna exista, usa-se a média dela)
        if arquivo.shape[1] >= 5:
            nivel_referencia = np.mean(arquivo[:, 4])
        else:
            nivel_referencia = np.mean(precisao) * 0.8  # Caso contrário, usa 80% da média das precisões

        # Configuração do gráfico
        plt.figure(figsize=(9, 7))  # Define o tamanho da figura
        ax = plt.gca()  # Obtém os eixos atuais

        # Linha de referência
        plt.axhline(y=nivel_referencia, color='#000000', linestyle='--', linewidth=2, alpha=0.7, label=f'Referência (AP = {nivel_referencia:.2f})')

        # Curva de Precisão-Recall
        disp = PrecisionRecallDisplay(precision=precisao, recall=recalls)
        disp.plot(name=f'Modelo (AP = {ap:.2f})', linewidth=2.5, color='#1E90FF', ax=ax)

        # Melhor ponto F1-score 
        plt.scatter(melhor_recall, melhor_precisao, color='#FF69B4', s=100, marker='o', label=f'Melhor F1: {melhor_f1:.3f}')

        # Ajustes visuais do gráfico
        plt.title(f"Curva Precisão-Recall\n({sys.argv[1]})", pad=20, fontsize=14)  # Define o título do gráfico
        plt.xlabel('Recall', fontsize=12, labelpad=10)  # Define o rótulo do eixo X
        plt.ylabel('Precisão', fontsize=12, labelpad=10)  # Define o rótulo do eixo Y

        # Ajuste dos limites dos eixos
        y_min = min(0.6, nivel_referencia * 0.9)
        plt.xlim(0.5, 1.03)  # Define os limites do eixo X
        plt.ylim(y_min, 1.03)  # Define os limites do eixo Y

        ax.grid(True, linestyle=':', alpha=0.3)  # Adiciona uma grade ao gráfico

        # Legenda posicionada à esquerda do gráfico
        plt.legend(loc='center left', bbox_to_anchor=(1, 0.5), fontsize=11)

        # Exibição do AP 
        plt.figtext(0.5, 0.05, f"Average Precision (AP) = {ap:.3f}", ha="center", fontsize=12,
                    bbox={"facecolor": "#F0E68C", "alpha": 0.8, "pad": 5})

        plt.tight_layout(rect=[0, 0.05, 0.85, 1])  # Ajusta automaticamente o layout para evitar sobreposição
        plt.savefig(sys.argv[3], dpi=300, bbox_inches='tight')  # Salva o gráfico como imagem

    except Exception as e:
        print(f"Erro: {str(e)}")
        sys.exit(1)

if __name__ == "__main__":
    main()