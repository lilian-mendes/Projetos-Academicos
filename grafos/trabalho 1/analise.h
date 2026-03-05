#include "grafo.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 600
#define MAX_NOME 600
#define MAX_VERTICES 800

enum tipo_vertice {
    METABOLITO,
    REACAO
}tipo_vertice;

enum cor_vertice {
    BRANCO, // 0: nao visitado / aberto
    CINZA,  // 1: visitado
    PRETO   // 2: fechado
}cor_vertice;

struct estado_dfs {
    int cor[MAX_VERTICES];
    int tempo_pre[MAX_VERTICES];
    int tempo_pos[MAX_VERTICES];
    int pai[MAX_VERTICES];
    int tempo_global;
    lista resultado_topologico;
    int tem_ciclo;
}estado_dfs;

struct tabela_reacoes {
    char nomes[MAX_VERTICES][MAX_NOME];
    enum tipo_vertice tipos[MAX_VERTICES];
    int qnt_vertices;
}tabela_reacoes;

struct reacao_metabolica {
    char nome_reacao[MAX_NOME];

    char substratos[MAX_VERTICES][MAX_NOME];
    int qnt_substratos;

    char produtos[MAX_VERTICES][MAX_NOME];
    int qnt_produtos;

}reacao_metabolica;

/* retorna o id de um elemento(substrato, produto ou reacao) e caso ele nao exista cria um novo */
int obtem_id_elemento(const char *nome, enum tipo_vertice tipo, struct tabela_reacoes *tabela, grafo G);

/* le uma linha do arquivo e transfere as informacoes para a estrutura de reacao metabolica */
struct reacao_metabolica *processa_linha(const char *linha);

/* constroi o grafo com base no arquivo .met formecido */
void gerar_grafo_da_reacao(const char *nome_arquivo, grafo G, struct tabela_reacoes *tabela);

/* funcao que realiza a dfs para todo vertice ainda nao visitado/ aberto */
void dfs(grafo G, struct estado_dfs *estado);

/* funcao recursiva de visita do dfs 
    - detecta ciclos e caso nao tenha um fornece a ordenacao topologica */
void dfs_recursivo(vertice u, grafo G, struct estado_dfs *estado);