#include "grafo.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct t_vertice {
    int id;
    lista fronteira; 
}t_vertice;

struct t_aresta {
	int id;
    int peso;
	vertice u, v;
}t_aresta;

struct t_grafo {
    lista vertices;
    lista arestas;
}t_grafo;

// --- funcoes auxiliares ---
int _pega_id_vertice(obj v) { 
    return vertice_id((vertice)v); 
}

vertice busca_vertice(int id, grafo G) { 
    return (vertice)busca_chave(id, vertices(G), _pega_id_vertice); 
}

void define_aresta_peso(aresta e, int peso, grafo G) {
    if (e) 
        e->peso = peso; 
}

aresta busca_aresta(int u_id, int v_id, grafo G) {
    // percorre a lista de todas as arestas do grafo
    for (no n = primeiro_no(arestas(G)); n; n = proximo(n)) {
        aresta e = (aresta)conteudo(n);
        int aresta_u = vertice_id(vertice_u(e));
        int aresta_v = vertice_id(vertice_v(e));

        // verifica se a aresta conecta u e v
        if ((aresta_u == u_id && aresta_v == v_id) || (aresta_u == v_id && aresta_v == u_id)) 
            return e; 
    }

    return NULL; 
}

//---------------------------------------------------------
// getters:

int vertice_id(vertice v) { 
    return v->id; 
};

int aresta_id(aresta e) { 
    return e->id; 
};

int aresta_peso(aresta e) { 
    return e->peso; 
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

/* funcao adicionada para o trabalho 2 */
lista vertice_fronteira(vertice v) {
    if (v)
        return v->fronteira;
    return NULL;
}

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

void destroi_grafo(grafo G) {
    if (!G) 
        return;

    while (!vazio(arestas(G))) 
        free(desempilha(arestas(G)));
    while (!vazio(vertices(G))) {
        vertice v = (vertice)desempilha(vertices(G));
        /* modificacao feita para o trabalho 2, pois foi identificado que estava sendo alocado mas nao estava sendo liberado */
        while (!vazio(v->fronteira)) 
            desempilha(v->fronteira);
        free(v->fronteira);
        free(v);
    }

    free(G->arestas);
    free(G->vertices);
    free(G);
}

vertice adiciona_vertice(int id, grafo G) {
    if (busca_vertice(id, G)) 
        return busca_vertice(id, G);

    vertice v = (vertice)malloc(sizeof(struct t_vertice));
    if (!v) 
        exit(_ERRO_MALLOC_);

    v->id = id;
    v->fronteira = cria_lista();
    empilha(v, vertices(G));

    return v;
}

/* funcao original modificada para funcionar para grafo direcionado */
aresta adiciona_aresta(int id, int u_id, int v_id, int peso, grafo G) {
    vertice u = busca_vertice(u_id, G);
    vertice v = busca_vertice(v_id, G);

    if (!u || !v) 
        return NULL;

    aresta e = (aresta)malloc(sizeof(struct t_aresta));
    
    if (!e) 
        exit(_ERRO_MALLOC_);

    e->id = id;
    e->peso = peso;
    e->u = u;
    e->v = v;

    empilha(e, arestas(G));
    empilha(e, u->fronteira);

    /* removemos a seguinte linha para que adicione apenas na lista do u e nao na do v tambem, 
    pois como ela eh direcionada nao pode ter nas duas listas (apenas se ambos os vertices apontam para os dois)
    u -> v e v -> u */
    //empilha(e, v->fronteira);

    return e;
}

//---------------------------------------------------------
// funcoes para operacoes com o grafo pronto:

int grau(vertice v, grafo G) {
    int d_v = 0;
    for (no n = primeiro_no(v->fronteira); n; n = proximo(n)) 
        d_v++;
    return d_v;
}


// imprime o vertice v
void imprime_vertice(vertice v, grafo G) {
    if (!v) 
        return;

    printf("%d: [%d](", vertice_id(v), grau(v, G));
    for (no n = primeiro_no(v->fronteira); n; n = proximo(n)) {
        aresta e = (aresta)conteudo(n);
        vertice vizinho;
        if (vertice_u(e) == v) 
            vizinho = vertice_v(e);
        else 
            vizinho = vertice_u(e);
        
        // imprime o ID do vertice vizinho
        printf("%d", vertice_id(vizinho));

        // adiciona um espaco se nao for o ultimo vizinho da lista
        if (proximo(n)) 
            printf(", ");
    }

    printf(") | ");
}

// imprime a aresta e
void imprime_aresta(aresta e) {
    if (!e) 
        return;

    int u_id = vertice_id(vertice_u(e));
    int v_id = vertice_id(vertice_v(e));
    printf("%d: {%d,%d} [%d] | ", aresta_id(e), u_id, v_id, aresta_peso(e));
}

void imprime_grafo(grafo G) {
    printf("\nVertices: <id>: [grau] (<vizinhos>)\n");
    printf("Vertices: ");
    imprime_lista(vertices(G), (void_f_obj) imprime_vertice);
    printf("\nArestas: <id>: {u,v} [peso]");
    printf("\nArestas: ");
    imprime_lista(arestas(G), (void_f_obj) imprime_aresta);
    printf("\n");
}

void imprime_representacao(grafo G) {
    printf("\n--- Representação: Lista de Adjacência ---\n");
    if (!G) 
        return;

    for (no n_v = primeiro_no(vertices(G)); n_v; n_v = proximo(n_v)) {
        vertice v = (vertice)conteudo(n_v);
        printf("  Vértice %d -> [", vertice_id(v));

        for (no n_e = primeiro_no(v->fronteira); n_e; n_e = proximo(n_e)) {
            aresta e = (aresta)conteudo(n_e);
            vertice vizinho;
            if (vertice_u(e) == v) 
                vizinho = vertice_v(e);
            else 
                vizinho = vertice_u(e);
            
            printf("%d", vertice_id(vizinho));

            // adiciona um espaco se nao for o ultimo vizinho da lista
            if (proximo(n_e)) 
                printf(", ");
        }
        printf("]");
        printf("\n");
    }
    printf("------------------------------------------\n");
}