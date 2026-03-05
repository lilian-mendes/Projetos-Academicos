#!/bin/bash

# Diretórios para armazenar gráficos e resultados
DIR_GRAFICOS="graficos"
DIR_RESULTADOS="resultados"

# Cria os diretórios, se não existirem
mkdir -p "$DIR_GRAFICOS" "$DIR_RESULTADOS"

# Função para calcular métricas
calcula_metricas(){

    # Parâmetros da função
    local arquivo_entrada="$1"  
    local tamanho="$2"        
    local arquivo_saida="$3"    

    # Extrai as informações do arquivo e calcula Precisão, Recall e F1-Score
    grep "for conf_thresh =" "$arquivo_entrada" | awk -v tam="$tamanho" '{
        # Captura valores específicos dentro da linha extraída
        thresh = $4; gsub(",", "", thresh)  # Threshold de confiança (removendo vírgulas)
        tp = $7; gsub(",", "", tp)          # Verdadeiros Positivos (TP)
        fp = $10; gsub(",", "", fp)         # Falsos Positivos (FP)
        fn = $13; gsub(",", "", fn)         # Falsos Negativos (FN)

        # Cálculo de Recall = TP / (TP + FN), garantindo que não haja divisão por zero
        recall = (tp + fn > 0) ? tp / (tp + fn) : 0

        # Cálculo de Precisão = TP / (TP + FP), garantindo que não haja divisão por zero
        precisao = (tp + fp > 0) ? tp / (tp + fp) : 0

        # Cálculo do F1-Score = 2 * (Precisão * Recall) / (Precisão + Recall)
        f1 = (precisao + recall > 0) ? 2 * (precisao * recall) / (precisao + recall) : 0

        # Salva os resultados formatados no arquivo de saída
        printf "%.4f %.4f %.4f %.4f\n", thresh, precisao, recall, f1
    }' > "$arquivo_saida"  # Redireciona a saída para o arquivo de saída
}

# Função para encontrar o melhor F1-Score
melhor_f1(){

    local arquivo_entrada="$1"

    # Verifica se o arquivo existe e não está vazio
    if [ ! -s "$arquivo_entrada" ]; then
        echo "Erro: Arquivo para cálculo de F1 não encontrado ou vazio: $arquivo_entrada" >&2
        return 1
    fi

    # Encontra a linha com o maior F1-score
    awk 'BEGIN {max_f1 = 0; melhor_th = 0; melhor_p = 0; melhor_r = 0}
    {
        if ($4 > max_f1) {max_f1 = $4; melhor_th = $1; melhor_p = $2; melhor_r = $3}
    }
    END {
        printf "%.4f %.4f %.4f %.4f", melhor_th, max_f1, melhor_p, melhor_r
    }' "$arquivo_entrada"
}

# Função para gerar métricas por tamanhos de imagem
gera_metricas_tamanhos(){

    # Limpa ou cria o arquivo de saída
    local arquivo_saida="${DIR_RESULTADOS}/metricas_por_tamanho.txt"
    > "$arquivo_saida"  

    # Itera sobre todos os arquivos de resultados do YOLOv4
    for arquivo in *_yolov4_*.txt; do

        # Ignora arquivos que não seguem o padrão e extrai o tamanho da imagem do nome do arquivo
        [[ $arquivo =~ ^([0-9]+)_ ]] || continue  
        tamanho="${BASH_REMATCH[1]}"
        
        # Arquivo temporário para métricas
        metricas_tmp="${DIR_RESULTADOS}/metricas_${tamanho}.tmp"  

        # Chama a função para calcular métricas
        calcula_metricas "$arquivo" "$tamanho" "$metricas_tmp" || continue  
        
        # Captura os resultados do melhor F1
        resultados=($(melhor_f1 "$metricas_tmp"))
        retorno=$?
        if [ $retorno -ne 0 ]; then
            echo "Erro ao processar métricas para tamanho $tamanho" >&2
            continue
        fi
                
        # Adiciona os resultados ao arquivo de saída
        cat >> "$arquivo_saida" <<EOF
=== TAMANHO: ${tamanho}px ===
Melhor Threshold: ${resultados[0]%.3f}
F1-score: ${resultados[1]%.3f}
Precisao: ${resultados[2]%.3f}
Recall: ${resultados[3]%.3f}
=========================

EOF

    # Remove o arquivo temporário
    rm -f "$metricas_tmp"  
    done
}

# Função principal
main(){

    #  Limpa ou cria o arquivo temporário
    METRICAS_GLOBAIS="${DIR_RESULTADOS}/dados_globais.tmp"
    > "$METRICAS_GLOBAIS"

    # Processa cada arquivo
    for arquivo in *_yolov4_*.txt; do

        # Extrai o tamanho da imagem do nome do arquivo
        if [[ "$arquivo" =~ ^([0-9]+)_ ]]; then
            tamanho="${BASH_REMATCH[1]}"
        else
            # Ignora arquivos que não seguem o padrão
            continue  
        fi

        echo "Processando ${tamanho}px..."  

        # Arquivo temporário para métricas
        metricas="${DIR_RESULTADOS}/metricas_${tamanho}.tmp"  

        # Chama a função para calcular métricas
        calcula_metricas "$arquivo" "$tamanho" "$metricas" || continue  
        
        # Gera gráficos individuais chamando o script Python
        python3 auxiliar_grafico.py "$tamanho" "$metricas" "$DIR_GRAFICOS/precision_recall_${tamanho}px"
        
        # Coleta dados para análise global
        resultados=$(melhor_f1 "$metricas")
        status_retorno=$?
        
        if [ $status_retorno -eq 0 ]; then
            # Salva os resultados globais
            echo "$tamanho $resultados" >> "$METRICAS_GLOBAIS"  
        fi

        # Remove o arquivo temporário
        rm -f "$metricas"  

    done

    # Gera relatório de métricas por tamanho
    gera_metricas_tamanhos

    # Encontra o melhor F1-score global
    read best_tam best_thresh best_f1 best_prec best_recall < <(
        awk '
        BEGIN {max_f1 = 0}
        {
            if ($3 > max_f1) {
                max_f1 = $3
                print $1, $2, $3, $4, $5
            }
        }
        ' "$METRICAS_GLOBAIS"
    )

    # Gera recomendação final com base nos resultados
    cat > "${DIR_RESULTADOS}/recomendacao_final.txt" <<EOF
Recomendação Final:
------------------
O tamanho de imagem ideal é ${best_tam}px com threshold ${best_thresh%.3f}, pois:
- Apresentou o maior F1-score: ${best_f1%.3f}
- Boa combinação de Precision (${best_prec%.3f}) e Recall (${best_recall%.3f})

EOF

    # Remove arquivos temporários
    rm -f "$METRICAS_GLOBAIS"
    
    echo "Processamento concluído."
    echo "Gráficos gerados e métricas calculadas."
}

# Chama a função principal
main