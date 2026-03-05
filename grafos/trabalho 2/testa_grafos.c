/*******************************************
 * Leitura e impressao de grafos.
 *
 * Algoritmos em Grafos e Otimização
 * Departamento de Informática - UFPR
 * prof. Guilherme Derenievicz
 *******************************************/

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

// Função auxiliar para o busca_chave funcionar com inteiros puros
int get_int_id(int *id) {
    return *id;
}

int main() {
  
  // cria grafo vazio
  grafo G = cria_grafo();
  
  // le n e adiciona vertices 1..n
  int n;
  scanf("%d", &n); // qtd vertices
  
  for (int i = 1; i <= n; ++i)
    adiciona_vertice(i, G);
  
  // le pares u v e adiciona arestas {u,v}
  // termina quando digitar 0 0
  int id = 1;
  int u, v;
  scanf("%d %d", &u, &v);
  while (u && v) {
    adiciona_aresta(id, u, v, G);
    ++id;
    scanf("%d %d", &u, &v);
  }

  // Le o conjunto de vértices X para o subgrafo induzido
  int k;
  scanf("%d", &k); // qtd de vértices em X
  
  lista X = cria_lista();
  for (int i = 0; i < k; ++i) {
      int vertice_id_x;
      scanf("%d", &vertice_id_x);
      // Para usar a lista genérica, precisamos de um ponteiro
      vertice v_dummy = malloc(sizeof(t_vertice));
      if (!v_dummy) exit(_ERRO_MALLOC_);
      v_dummy->id = vertice_id_x;
      empilha(v_dummy, X);
  }

  // Transforma G em G[X]
  induz_subgrafo(X, G);

  // Desaloca a lista auxiliar X
  while(!vazio(X)) {
    free(desempilha(X));
  }
  free(X);

  // Calcula os pesos das arestas do grafo resultante
  calcula_pesos_arestas(G);
  
  // imprime informacoes do grafo e o destroi
  imprime_grafo(G);
  destroi_grafo(G);
  
  return 0;
}