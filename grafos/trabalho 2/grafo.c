#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

//---------------------------------------------------------
// getters:

int vertice_id(vertice v) {
  return v->id;
};
lista fronteira(vertice v) {
  return v->fronteira;
};
int aresta_id(aresta e) {
  return e->id;
};
vertice vertice_u(aresta e) {
  return e->u;
};
vertice vertice_v(aresta e) {
  return e->v;
};
lista vertices(grafo G) {
  return G->vertices;
};
lista arestas(grafo G) {
  return G->arestas;
};

//---------------------------------------------------------
// funcoes para construcao/desconstrucao do grafo:

// cria grafo vazio e o retorna
grafo cria_grafo() {
  grafo G = (grafo) malloc(sizeof(t_grafo));
  if (!G)
    exit(_ERRO_MALLOC_);
  G->vertices = cria_lista();
  G->arestas = cria_lista();
  return G;
}

// Funcao auxiliar para desalocar os nós de uma lista sem desalocar o conteudo
void destroi_nos_lista(lista l) {
    while (!vazio(l)) {
        desempilha(l);
    }
    free(l);
}

// destroi grafo G (desaloca toda a memoria)
void destroi_grafo(grafo G) {
    while(!vazio(arestas(G))) {
        aresta e = desempilha(arestas(G));
        free(e);
    }
    while(!vazio(vertices(G))) {
        vertice v = desempilha(vertices(G));
        destroi_nos_lista(fronteira(v));
        free(v);
    }
    free(arestas(G));
    free(vertices(G));
    free(G);
}

// cria novo vertice com id <id> e adiciona ao grafo G
void adiciona_vertice(int id, grafo G) {
  vertice v = (vertice) malloc(sizeof(t_vertice));
  if (!v)
    exit(_ERRO_MALLOC_);
  v->id = id;
  v->fronteira = cria_lista();
  empilha(v, vertices(G));
}

// remove vertice com id <id> do grafo G e o destroi
// [deve remover e destruir tambem as arestas incidentes]
void remove_vertice(int id, grafo G) {
    vertice v = busca_chave(id, vertices(G), (int_f_obj) vertice_id);
    if (!v)
        return;

    // Remove arestas incidentes
    while(!vazio(fronteira(v))) {
        aresta e = topo(fronteira(v)); // Pega a aresta sem remover da fronteira
        remove_aresta(aresta_id(e), G);
    }

    // Remove o vertice da lista do grafo
    vertice v_removido = remove_chave(id, vertices(G), (int_f_obj) vertice_id);
    if (v_removido) {
        destroi_nos_lista(fronteira(v_removido));
        free(v_removido);
    }
}

// cria aresta com id <id> incidente a vertices com
// ids <u_id> e <v_id> e adiciona ao grafo G
void adiciona_aresta(int id, int u_id, int v_id, grafo G) {
    vertice u = busca_chave(u_id, vertices(G), (int_f_obj) vertice_id);
    vertice v = busca_chave(v_id, vertices(G), (int_f_obj) vertice_id);

    if (!u || !v)
        return; // Vértice não encontrado

    aresta e = (aresta) malloc(sizeof(t_aresta));
    if (!e)
        exit(_ERRO_MALLOC_);
    
    e->id = id;
    e->u = u;
    e->v = v;
    e->peso = 0; // Peso inicializado com 0

    empilha(e, arestas(G));
    empilha(e, fronteira(u));
    empilha(e, fronteira(v));
}

// remove aresta com id <id> do grafo G e a destroi
void remove_aresta(int id, grafo G) {
    aresta e = busca_chave(id, arestas(G), (int_f_obj) aresta_id);
    if (!e)
        return;

    vertice u = vertice_u(e);
    vertice v = vertice_v(e);

    // Remove a aresta das listas de fronteira
    remove_chave(id, fronteira(u), (int_f_obj) aresta_id);
    remove_chave(id, fronteira(v), (int_f_obj) aresta_id);

    // Remove a aresta da lista principal do grafo e a desaloca
    aresta e_removida = remove_chave(id, arestas(G), (int_f_obj) aresta_id);
    if (e_removida)
        free(e_removida);
}

//---------------------------------------------------------
// funcoes para operacoes com o grafo pronto:

// calcula e devolve o grau do vertice v
int grau(vertice v) {
  int d_v = 0;
  for (no n = primeiro_no(fronteira(v)); n; n = proximo(n))
    ++d_v;
  return d_v;
}

// imprime o grafo G
void imprime_grafo(grafo G) {
  printf("\nVertices: <id> - [grau]( <fronteira> )");
  printf("\nVertices: ");
  imprime_lista(vertices(G), (void_f_obj) imprime_vertice);
  printf("\nArestas: <id>:{u,v}(peso)");
  printf("\nArestas: ");
  imprime_lista(arestas(G), (void_f_obj) imprime_aresta);
  printf("\n");
}

// imprime o vertice v
void imprime_vertice(vertice v) {
  printf("%d - [%d]( ", vertice_id(v), grau(v));
  imprime_lista(fronteira(v), (void_f_obj) imprime_aresta);
  printf(")");
}

// imprime a aresta e
void imprime_aresta(aresta e) {
  int u_id = vertice_id(vertice_u(e));
  int v_id = vertice_id(vertice_v(e));
  printf("%d:{%d,%d}(%d)", aresta_id(e), u_id, v_id, e->peso);
}

//---------------------------------------------------------
// funcoes novas:

// Transforma G no subgrafo induzido pelo conjunto de vértices com IDs em X
void induz_subgrafo(lista X, grafo G) {
    lista a_remover = cria_lista();
    
    // Identifica todos os vértices que não estão em X
    for (no n = primeiro_no(vertices(G)); n; n = proximo(n)) {
        vertice v = conteudo(n);
        // busca_chave retorna NULL se o id do vertice v não estiver em X
        if (!busca_chave(vertice_id(v), X, (int_f_obj) vertice_id)) {
            empilha(v, a_remover);
        }
    }

    // Remove os vértices identificados
    while(!vazio(a_remover)) {
        vertice v = desempilha(a_remover);
        remove_vertice(vertice_id(v), G);
    }
    
    destroi_nos_lista(a_remover);
}

// Calcula o peso de todas as arestas em G
void calcula_pesos_arestas(grafo G) {
    for (no n = primeiro_no(arestas(G)); n; n = proximo(n)) {
        aresta e = conteudo(n);
        e->peso = grau(vertice_u(e)) + grau(vertice_v(e));
    }
}