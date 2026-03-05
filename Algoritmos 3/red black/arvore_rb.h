#ifndef ARVORE_RB
#define ARVORE_RB

// Definições das cores
#define VERMELHO 1
#define PRETO 0

// Estruturas para a árvore e nós da árvore rubro-negra
// Estrutura de um nó da árvore
struct no {
    int valor;          // Valor armazenado no nó
    int cor;            // Cor do nó: VERMELHO ou PRETO
    struct no *pai;     // Ponteiro para o nó pai
    struct no *esquerdo;  // Ponteiro para o filho esquerdo
    struct no *direito;   // Ponteiro para o filho direito
};

// Estrutura da árvore rubro-negra
struct arvore {
    struct no *raiz;    // Ponteiro para a raiz da árvore
};

// Funções relacionadas à árvore rubro-negra

// Inicializa uma nova árvore rubro-negra
void inicializar_arvore(struct arvore *nova_arvore);

// Insere um elemento na árvore rubro-negra
void insere_elemento(struct arvore *t, int x);

// Busca um nó na árvore com base no valor
struct no* buscar_no(struct arvore *t, int valor);

// Deleta um nó da árvore rubro-negra
void rb_delete(struct arvore *t, struct no *z);

// Função para destruir toda a árvore (liberar memória)
void destroi_arvore(struct arvore *t);

// Função para imprimir a árvore (visualização em formato de árvore)
void imprimir_arvore(struct arvore *t);

// Função auxiliar para imprimir a árvore, chamada recursivamente
void imprimir_arvore_aux(struct no *n, int nivel);

#endif
