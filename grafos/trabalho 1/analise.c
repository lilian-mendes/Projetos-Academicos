#include "analise.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* retorna o id de um elemento(metabolito ou reacao) e caso ele nao exista cria um novo */
int obtem_id_elemento(const char *nome, enum tipo_vertice tipo, struct tabela_reacoes *tabela, grafo G) {
    
    for (int i = 0; i < tabela->qnt_vertices; i++) {
        /* verifica se o elemento ja esta na tabela, se sim retorna o ID dele */
        if (strcmp(tabela->nomes[i], nome) == 0) 
            return i; 
    }
    
    if (tabela->qnt_vertices >= MAX_VERTICES) {
        fprintf(stderr, "Erro: Limite de vértices atingido.\n");
        return -1; 
    }

    /* se nao, adiciona ele a tabela */
    int novo_id = tabela->qnt_vertices;

    strcpy(tabela->nomes[novo_id], nome);
    tabela->tipos[novo_id] = tipo;
    
    tabela->qnt_vertices++; 
    
    adiciona_vertice(novo_id, G);
    
    return novo_id;
}

/* le uma linha do arquivo e transfere as informacoes para a estrutura de reacao metabolica */
struct reacao_metabolica *processa_linha(const char *linha) {
    struct reacao_metabolica *reacao = calloc(1, sizeof(struct reacao_metabolica));
    char buffer[MAX_LINHA];
    
    /* copia uma linha do arquivo e armazena em um buffer */
    strncpy(buffer, linha, MAX_LINHA - 1);
    buffer[MAX_LINHA - 1] = '\0';

    /* divide a string em duas partes: 
        substratos: tudo antes do '->'
        produtos: tudo depois do '->' */
        
    char *produtos_reacao = strstr(buffer, "->");

    if (produtos_reacao == NULL) 
        return reacao; 

    *produtos_reacao = '\0';

    char *substrato_reacao = buffer;
    produtos_reacao += 2; 

    /* criamos as strings com o tamanho ate MAX_NOME -1 para evitar que o buffer estoure */
    char nome[20];
    char elemento[20];
    sprintf(nome, "%%%ds%%n", MAX_NOME - 1);     
    sprintf(elemento, " %%%ds%%n", MAX_NOME - 1); 

    char elementos[MAX_NOME];
    int caracteres_lidos = 0;
    char *atual;

    /* processa a parte dos substratos */
    atual = substrato_reacao;
    
    if (sscanf(atual, nome, reacao->nome_reacao, &caracteres_lidos) == 1) 
        atual += caracteres_lidos;

    while (sscanf(atual, elemento, elementos, &caracteres_lidos) == 1) {
        atual += caracteres_lidos;
        
        /* processa o caso em que possui mais de um substrato */
        if (strcmp(elementos, "+") != 0) {
            
            char *ptr = elementos; 
            
            /* percorre toda a string */
            while (*ptr) { 
                char substratos_separados[MAX_NOME];
                
                /* procura um '+' para separar os substratos */
                char *proximo_mais = strchr(ptr, '+');
                
                if (proximo_mais) {
                    int comprimento = proximo_mais - ptr;
                    if (comprimento >= MAX_NOME) 
                        comprimento = MAX_NOME - 1;
                    
                    strncpy(substratos_separados, ptr, comprimento);
                    substratos_separados[comprimento] = '\0'; 

                    /* avanca para depois do '+' para pegar o outro substrato */
                    ptr = proximo_mais + 1;
                } 
                else {
                    /* caso em que nao tem mais nenhum '+', portanto o resto da sring eh um substrato */
                    strncpy(substratos_separados, ptr, MAX_NOME - 1);
                    substratos_separados[MAX_NOME - 1] = '\0';
                    
                    /* move o ponteiro para o final da string */
                    ptr += strlen(substratos_separados); 
                }

                /* adiciona os substratos encontrados a lista de substratos presentes na reacao */
                if (reacao->qnt_substratos < MAX_VERTICES)
                    strcpy(reacao->substratos[reacao->qnt_substratos++], substratos_separados);
            }
        }
    }

    /* processa a parte dos produtos seguindo a mesma logica aplicada para os substratos */
    atual = produtos_reacao;
    while (sscanf(atual, elemento, elementos, &caracteres_lidos) == 1) {
        atual += caracteres_lidos;
        if (strcmp(elementos, "+") != 0) {
            
            char *ptr = elementos;
            
            while (*ptr) {
                char produtos_separados[MAX_NOME];
                char *proximo_mais = strchr(ptr, '+');
                
                if (proximo_mais) {
                    int comprimento = proximo_mais - ptr;
                    if (comprimento >= MAX_NOME) 
                        comprimento = MAX_NOME - 1; 

                    strncpy(produtos_separados, ptr, comprimento);
                    produtos_separados[comprimento] = '\0';
                    ptr = proximo_mais + 1;
                } 
                else {
                    strncpy(produtos_separados, ptr, MAX_NOME - 1);
                    produtos_separados[MAX_NOME - 1] = '\0';
                    ptr += strlen(produtos_separados);
                }
                
                if (reacao->qnt_produtos < MAX_VERTICES) 
                    strcpy(reacao->produtos[reacao->qnt_produtos++], produtos_separados);
            }
        }
    }

    return reacao;
}

/* constroi o grafo com base no arquivo .met formecido */
void gerar_grafo_da_reacao(const char *nome_arquivo, grafo G, struct tabela_reacoes *tabela) {

    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Nao foi possivel abrir o arquivo para leitura.\n");
        exit(EXIT_FAILURE);
    }

    char buffer_linha[MAX_LINHA];
    int id_aresta = 0;

    while (fgets(buffer_linha, sizeof(buffer_linha), arquivo)) {

        buffer_linha[strcspn(buffer_linha, "\n")] = 0; 

        /* pula linha em branco */
        if (strlen(buffer_linha) == 0)
            continue; 

        struct reacao_metabolica *reacao_atual = processa_linha(buffer_linha);

        /* pula a linha se a reacao for invalida */
        if (strlen(reacao_atual->nome_reacao) == 0) {
            free(reacao_atual);
            continue;
        }

        int id_reacao = obtem_id_elemento(reacao_atual->nome_reacao, REACAO, tabela, G);

        if (id_reacao == -1) {
            fprintf(stderr, "Aviso: Falha ao obter ou criar ID para a reação '%s'.\n", reacao_atual->nome_reacao);
            free(reacao_atual);
            continue;
        }

        /* processa os substratos */
        for (int i = 0; i < reacao_atual->qnt_substratos; i++) {
            int id_substrato = obtem_id_elemento(reacao_atual->substratos[i], METABOLITO, tabela, G);

            if (id_substrato == -1) {
                fprintf(stderr, "Aviso: Falha ao obter ou criar ID para o substrato '%s'.\n", reacao_atual->substratos[i]);
                continue; 
            }

            aresta e = adiciona_aresta(id_aresta++, id_substrato, id_reacao, 1, G);
            if (e == NULL) 
                fprintf(stderr, "Aviso: Falha ao adicionar aresta do substrato %d -> %d.\n", id_substrato, id_reacao);
        }

        /* processa os produtos */
        for (int i = 0; i < reacao_atual->qnt_produtos; i++) {
            int id_produto = obtem_id_elemento(reacao_atual->produtos[i], METABOLITO, tabela, G);

            if (id_produto == -1) {
                fprintf(stderr, "Aviso: Falha ao obter ou criar ID para o produto '%s'.\n", reacao_atual->produtos[i]);
                continue; 
            }

            aresta e = adiciona_aresta(id_aresta++, id_reacao, id_produto, 1, G);
            if (e == NULL) 
                fprintf(stderr, "Aviso: Falha ao adicionar aresta do produto %d -> %d.\n", id_reacao, id_produto);
        }

        free(reacao_atual);
       
    }

    fclose(arquivo);
}

/* funcao que realiza a dfs para todo vertice ainda nao visitado */
void dfs(grafo G, struct estado_dfs *estado) {
 
    estado->resultado_topologico = cria_lista(); 
    estado->tem_ciclo = 0;
    estado->tempo_global = 0;

    lista l_vertices = vertices(G); 
    
    for (no n_v = primeiro_no(l_vertices); n_v; n_v = proximo(n_v)) { 
        vertice v = (vertice)conteudo(n_v); 
        int v_id = vertice_id(v); 
        
        estado->cor[v_id] = BRANCO;

        /* inicializa todos os vertices "sem pai" */
        estado->pai[v_id] = -1; 
    }

    /* chama o dfs recursivo para todo vertice que ainda esta aberto */
    for (no n_u = primeiro_no(l_vertices); n_u; n_u = proximo(n_u)) {
        vertice u = (vertice)conteudo(n_u);
        if (estado->cor[vertice_id(u)] == BRANCO) {
            if (estado->tem_ciclo) 
                break; 
            dfs_recursivo(u, G, estado);
        }
    }
}

/* funcao recursiva de visita do dfs 
    - detecta ciclos e caso nao tenha um fornece a ordenacao topologica */
void dfs_recursivo(vertice u, grafo G, struct estado_dfs *estado) {
    if (estado->tem_ciclo) 
        return;

    int u_id = vertice_id(u); 
    
    estado->tempo_global++;

    /* marca o tempo de pre do vertice */
    estado->tempo_pre[u_id] = estado->tempo_global;

    /* muda a cor do vertice para dizer que ja foi visitado */
    estado->cor[u_id] = CINZA; 

    /* percorre os vizinnhos de u */
    lista fronteira_u = vertice_fronteira(u); 

    for (no n_aresta = primeiro_no(fronteira_u); n_aresta; n_aresta = proximo(n_aresta)) { 
        aresta e = (aresta)conteudo(n_aresta); 

        vertice v = vertice_v(e); 
        int v_id = vertice_id(v); 

        /* se encontra um vizinho que ja foi visitado, detecta um ciclo */
        if (estado->cor[v_id] == CINZA) {
            estado->tem_ciclo = 1;
            return;
        }

        /* se o vizinho encontrado ainda nao foi visitado, define u como pai de v */
        if (estado->cor[v_id] == BRANCO) {
            estado->pai[v_id] = u_id;
            dfs_recursivo(v, G, estado);
        }
    }

    /* fecha o vertice  */
    estado->cor[u_id] = PRETO; 
    estado->tempo_global++;

    /* marca o tempo de pos do vertice */
    estado->tempo_pos[u_id] = estado->tempo_global;

    /* empilha para formar a ordenacao topologica */
    empilha((obj)u, estado->resultado_topologico); 
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.met>\n", argv[0]);
        return 1;
    }

    grafo G = cria_grafo(); 
    struct tabela_reacoes tabela = {0}; 

    gerar_grafo_da_reacao(argv[1], G, &tabela); 
    
    struct estado_dfs estado_atual_dfs;
    dfs(G, &estado_atual_dfs);

    /* verifica se tem um  ciclo na reacao, se sim imprime a mensagem */
    if (estado_atual_dfs.tem_ciclo) 
        printf("rede cíclica\n");

    /* caso nao, imprime a ordenacao topologica */
    else {
        no n = primeiro_no(estado_atual_dfs.resultado_topologico); 
        while (n) {
            vertice v = (vertice)conteudo(n); 
            int v_id = vertice_id(v); 
            
            if (tabela.tipos[v_id] == METABOLITO)
                printf("%s ", tabela.nomes[v_id]); 
            
            n = proximo(n); 
        }
        printf("\n");
    }

    /* desaloca todas as estruturas utilizadas */
    while (!vazio(estado_atual_dfs.resultado_topologico)) 
        desempilha(estado_atual_dfs.resultado_topologico); 
    free(estado_atual_dfs.resultado_topologico); 

    destroi_grafo(G); 

    return 0;
}