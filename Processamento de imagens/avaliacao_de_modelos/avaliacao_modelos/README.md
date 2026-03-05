# Análise de Resultados de Detecção com YOLOv4

Este projeto realiza a análise de resultados de detecção de objetos usando o modelo **YOLOv4**. O script principal em **Bash** processa arquivos de resultados, calcula métricas de desempenho e gera gráficos de precision-recall.

---

## Estrutura do Projeto

```
.
├── auxiliar_grafico.py   # Script Python para gerar gráficos de precision-recall
├── resultados/           # Diretório para armazenar os resultados
├── graficos/             # Diretório para armazenar os gráficos gerados
└── avaliacao_modelos.sh  # Script principal em Bash
```

---

## Requisitos

Antes de executar os scripts, certifique-se de ter os seguintes requisitos instalados:

- **Python 3**
- **Matplotlib**
- **NumPy**
- **scikit-learn**
- **Bash** (para executar o script principal)

Para garantir o funcionamento correto dos scripts Python, recomenda-se a criação de um ambiente virtual e instalação das dependências necessárias

---

## Como Usar

### 1 Organize seus arquivos:
- Coloque os arquivos de saída do YOLOv4 no diretório onde o script Bash está localizado.
- Os arquivos devem seguir o padrão de nomenclatura: `*_yolov4_*.txt`.

### 2️ Execute o script Bash:
```bash
bash avaliacao_modelos.sh
```

### 3️ Resultados:
- O **relatório final** e as **métricas calculadas** serão salvos no diretório `resultados/`.
- Os **gráficos de precision-recall** serão salvos no diretório `graficos/`.

---

## Detalhes do Código

### Script Bash (`avaliacao_modelos.sh`)

O script principal realiza as seguintes etapas:

1. Cria os diretórios `graficos/` e `resultados/`, se ainda não existirem.
2. Itera sobre os arquivos de detecção do YOLOv4 (`*_yolov4_*.txt`).
3. Extrai métricas (**Precisão, Recall, F1-Score**) e salva em um arquivo temporário.
4. Calcula a **Média de Precisão (AP)** e encontra o melhor **F1-Score**.
5. Gera um **gráfico Precision-Recall** para cada tamanho de imagem.
6. Salva os resultados e os gráficos e remove arquivos temporários.

🔹 **Principais funções:**
- `calcula_metricas` → Calcula as métricas de Precisão, Recall e F1-Score.
- `melhor_f1` → Identifica o melhor F1-Score e seu threshold correspondente.
- `gera_grafico` → Chama o script Python para gerar os gráficos de precision-recall.

### Script Python (`auxiliar_grafico.py`)

Este script:

- Carrega os dados das métricas calculadas pelo script Bash.
- Plota a **curva de precision-recall**.
- Marca o ponto do **melhor F1-Score** no gráfico.
- Salva o **gráfico gerado** como um arquivo PNG.

🔹 **Principais funcionalidades:**
- Cálculo da **Average Precision (AP)**
- Identificação do melhor **F1-Score**
- Geração de **gráficos otimizados e personalizados**

---

## Saída do Script

### Resultados numéricos:
- `resultados/metricas_por_tamanho.txt` → Contém as melhores métricas para cada tamanho de imagem.
- `resultados/recomendacao_final.txt` → Sugestão do melhor tamanho de imagem baseado no F1-Score.

### Gráficos:
- `graficos/precision_recall_<tamanho>.png` → Gráficos de precision-recall para cada tamanho de imagem testado.

---