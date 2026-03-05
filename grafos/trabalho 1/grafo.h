#ifndef _GRAFO_
#define _GRAFO_

#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

// a declaracao real das estruturas foi feita no arquivo .c
typedef struct t_vertice *vertice;
typedef struct t_aresta *aresta;
typedef struct t_grafo *grafo;

//---------------------------------------------------------
// funcoes auxiliares
int _pega_id_vertice(obj v);
vertice busca_vertice(int id, grafo G);
void define_aresta_peso(aresta e, int peso, grafo G);
aresta busca_aresta(int u_id, int v_id, grafo G);

//---------------------------------------------------------
// getters

int vertice_id(vertice v);
int aresta_id(aresta e);
int aresta_peso(aresta e);
vertice vertice_u(aresta e);
vertice vertice_v(aresta e);
lista vertices(grafo G);
lista arestas(grafo G);
lista vertice_fronteira(vertice v);

//---------------------------------------------------------
// funcoes para construcaoo e desconstrucao do grafo

grafo cria_grafo();
void destroi_grafo(grafo G);
vertice adiciona_vertice(int id, grafo G);
aresta adiciona_aresta(int id, int u_id, int v_id, int peso, grafo G);

//---------------------------------------------------------
// funcoes para operacoes com o grafo pronto

int grau(vertice v, grafo G);
void imprime_aresta(aresta e);
void imprime_vertice(vertice v, grafo G);
void imprime_grafo(grafo G);
void imprime_representacao(grafo G);


#endif