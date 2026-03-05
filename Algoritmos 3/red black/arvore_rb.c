#include <stdio.h>
#include <stdlib.h>
#include "arvore_rb.h"


// Função para imprimir a árvore com nível, cor e valores
void imprimir_arvore_aux(struct no *n, int nivel) {
    if (n == NULL) {
        return;
    }

    // Imprime os elementos da subárvore direita primeiro (visualização em árvore invertida)
    imprimir_arvore_aux(n->esquerdo, nivel + 1);

    printf("%d,%d,%d\n", n->valor,nivel, n->cor);

    // Imprime os elementos da subárvore esquerda
    imprimir_arvore_aux(n->direito, nivel + 1);
}
void imprimir_arvore(struct arvore *n){
      if (n->raiz != NULL) {
        imprimir_arvore_aux(n->raiz, 0); // Chama a função auxiliar, começando do nível 0
    }
}
// Função auxiliar recursiva para destruir a árvore
void destroi_arvore_aux(struct no *n) {
    if (n == NULL) {
        return;
    }
    
    // Destruir os filhos esquerdo e direito
    destroi_arvore_aux(n->esquerdo);
    destroi_arvore_aux(n->direito);
    
    // Libera a memória alocada para o nó atual
    free(n);
}
// Função para destruir a árvore
void destroi_arvore(struct arvore *t) {
    if (t == NULL || t->raiz == NULL) {
        return;
    }
    
    // Chama a função recursiva para destruir todos os nós
    destroi_arvore_aux(t->raiz);
    
    // Depois de destruir todos os nós, zera a árvore
    t->raiz = NULL;
}
void destruir_no(struct no **n) {
    if (n == NULL || *n == NULL) {
        return;
    }

    // Libera o nó e define o ponteiro como NULL
    free(*n);
    *n = NULL;
}


// Funções de rotação (esquerda e direita)
void rotacao_esq(struct arvore *t, struct no *x) {
    struct no *y = x->direito;
    x->direito = y->esquerdo;
    if (y->esquerdo != NULL) {
        y->esquerdo->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == NULL) {
        t->raiz = y;
    } else if (x == x->pai->esquerdo) {
        x->pai->esquerdo = y;
    } else {
        x->pai->direito = y;
    }
    y->esquerdo = x;
    x->pai = y;
}


void rotacao_dir(struct arvore *t, struct no *x) {
    struct no *y = x->esquerdo;
    x->esquerdo = y->direito;

    if (y->direito != NULL) {
        y->direito->pai = x;
    }
    y->pai = x->pai;

    if (x->pai == NULL) {
        t->raiz = y;
    } else if (x == x->pai->direito) {
        x->pai->direito = y;
    } else {
        x->pai->esquerdo = y;
    }

    y->direito = x;
    x->pai = y;
}
// Função para inicializar um nó com um valor
struct no* inicializar_no(int n) {
    struct no* novo;

    // Aloca memória para o novo nó e verifica se a alocação foi bem-sucedida
    if (!(novo = malloc(sizeof(struct no)))) {
        printf("ERRO AO CRIAR O NÓ\n");
        return NULL;
    }

    // Inicializa os campos do nó
    novo->valor = n;
    novo->cor = VERMELHO; // Novos nós são inicialmente vermelhos
    novo->pai = NULL;
    novo->direito = NULL;
    novo->esquerdo = NULL;

    return novo;
}
// Função para corrigir a árvore após inserção
void rb_insere_fixup(struct arvore *t, struct no *z) {
    while (z->pai != NULL && z->pai->cor == VERMELHO) { // Enquanto o pai for vermelho
        if (z->pai == z->pai->pai->esquerdo) { // Caso: pai é filho esquerdo do avô
            struct no *y = z->pai->pai->direito; // Tio de z

            if (y != NULL && y->cor == VERMELHO) { // Caso 1: tio é vermelho
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->direito) { // Caso 2: z é filho direito
                    z = z->pai;
                    rotacao_esq(t, z); // Rotação à esquerda
                }
                // Caso 3: z é filho esquerdo
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacao_dir(t, z->pai->pai); // Rotação à direita
            }
        } else { // Simétrico: pai é filho direito do avô
            struct no *y = z->pai->pai->esquerdo; // Tio de z

            if (y != NULL && y->cor == VERMELHO) { // Caso 1: tio é vermelho
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esquerdo) { // Caso 2: z é filho esquerdo
                    z = z->pai;
                    rotacao_dir(t, z); // Rotação à direita
                }
                // Caso 3: z é filho direito
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacao_esq(t, z->pai->pai); // Rotação à esquerda
            }
        }
    }

    // A raiz sempre deve ser preta
    t->raiz->cor = PRETO;
}


// Função para inserir um nó na árvore Rubro-Negra
void rb_insere(struct arvore *t, struct no *z) {
    struct no *y = NULL;        // Nó auxiliar para rastrear o pai
    struct no *x = t->raiz;     // Começa pela raiz da árvore

    // Procura o local correto para inserir o nó z
    while (x != NULL) {
        y = x;
        if (z->valor < x->valor) // Caminha para a subárvore esquerda
            x = x->esquerdo;
        else                     // Caminha para a subárvore direita
            x = x->direito;
    }

    // Define o pai do nó z
    z->pai = y;
    if (y == NULL)              // Se a árvore estiver vazia
        t->raiz = z;
    else if (z->valor < y->valor) // Insere à esquerda
        y->esquerdo = z;
    else                        // Insere à direita
        y->direito = z;

    // Inicializa os ponteiros dos filhos e a cor do nó z
    z->esquerdo = NULL;
    z->direito = NULL;
    z->cor = VERMELHO;

    // Ajusta a árvore após a inserção para manter as propriedades da RB
    rb_insere_fixup(t, z);
}


// Função para inserir um elemento na árvore
void insere_elemento(struct arvore *t, int x) {
    struct no* z = inicializar_no(x); // Inicializa o nó
    if (z != NULL) {
        rb_insere(t, z); // Insere o nó na árvore
    }
}
// Função para inicializar a árvore
void inicializar_arvore(struct arvore *nova_arvore) {
   
    nova_arvore->raiz = NULL;
}
// Função para encontrar o menor elemento de uma subárvore
struct no* menor_elemento(struct no *n) {
    // Itera para o nó mais à esquerda
    while (n->esquerdo != NULL) {
        n = n->esquerdo; // Move para o filho esquerdo
    }
    return n; // Retorna o menor elemento
}

// Função para substituir um nó na árvore (transplante)
void rb_transplante(struct arvore *t, struct no *u, struct no *v) {
    if (u->pai == NULL) {
        t->raiz = v; // Se u é a raiz, v se torna a nova raiz
    } else if (u == u->pai->esquerdo) {
        u->pai->esquerdo = v; // Substitui o filho esquerdo
    } else {
        u->pai->direito = v; // Substitui o filho direito
    }

    if (v != NULL) {
        v->pai = u->pai; // Atualiza o pai de v
    }
}
void rb_delete_fixup(struct arvore *t, struct no *x) {
    // Enquanto x não for a raiz e sua cor for preta
    while (x != t->raiz && x != NULL && x->cor == PRETO) {
        
        // Caso 1: x é filho esquerdo de seu pai
        if (x == x->pai->esquerdo) {
            struct no *w = x->pai->direito; // w é o irmão de x

            // Caso 1a: Se o irmão w for vermelho, rotaciona à esquerda no pai de x
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacao_esq(t, x->pai); // Rotação à esquerda
                w = x->pai->direito;    // Atualiza w após a rotação
            }

            // Caso 1b: Se ambos os filhos de w são pretos ou nulos, pinte w de vermelho
            if ((w->esquerdo == NULL || w->esquerdo->cor == PRETO) &&
                (w->direito == NULL || w->direito->cor == PRETO)) {
                w->cor = VERMELHO;
                x = x->pai; // Sobe para o pai
            } else {
                // Caso 1c: Se o filho direito de w é preto, rotaciona à direita em w
                if (w->direito == NULL || w->direito->cor == PRETO) {
                    if (w->esquerdo != NULL) {
                        w->esquerdo->cor = PRETO;
                    }
                    w->cor = VERMELHO;
                    rotacao_dir(t, w); // Rotação à direita
                    w = x->pai->direito; // Atualiza w após a rotação
                }

                // Caso 1d: Ajusta cores e faz rotação à esquerda
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                if (w->direito != NULL) {
                    w->direito->cor = PRETO;
                }
                rotacao_esq(t, x->pai); // Rotação à esquerda
                x = t->raiz; // Termina a correção
            }
        } else { // Caso 2: x é filho direito de seu pai
            struct no *w = x->pai->esquerdo; // w é o irmão de x

            // Caso 2a: Se o irmão w for vermelho, rotaciona à direita no pai de x
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacao_dir(t, x->pai); // Rotação à direita
                w = x->pai->esquerdo;   // Atualiza w após a rotação
            }

            // Caso 2b: Se ambos os filhos de w são pretos ou nulos, pinte w de vermelho
            if ((w->direito == NULL || w->direito->cor == PRETO) &&
                (w->esquerdo == NULL || w->esquerdo->cor == PRETO)) {
                w->cor = VERMELHO;
                x = x->pai; // Sobe para o pai
            } else {
                // Caso 2c: Se o filho esquerdo de w é preto, rotaciona à esquerda em w
                if (w->esquerdo == NULL || w->esquerdo->cor == PRETO) {
                    if (w->direito != NULL) {
                        w->direito->cor = PRETO;
                    }
                    w->cor = VERMELHO;
                    rotacao_esq(t, w); // Rotação à esquerda
                    w = x->pai->esquerdo; // Atualiza w após a rotação
                }

                // Caso 2d: Ajusta cores e faz rotação à direita
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                if (w->esquerdo != NULL) {
                    w->esquerdo->cor = PRETO;
                }
                rotacao_dir(t, x->pai); // Rotação à direita
                x = t->raiz; // Termina a correção
            }
        }
    }

    // Garante que x (pode ser nulo) tenha a cor preta após a correção
    if (x != NULL) {
        x->cor = PRETO;
    }
}


// Função para excluir um nó da árvore rubro-negra
void rb_delete(struct arvore *t, struct no *z) {
    struct no *y = z; // Nó que será realmente removido ou movido
    struct no *x;     // Nó que ocupará a posição de y
    int y_cor_original = y->cor; // Salva a cor original de y

    if (z->esquerdo == NULL) {
        x = z->direito; // Substituto é o filho direito
        rb_transplante(t, z, z->direito);
    } else if (z->direito == NULL) {
        x = z->esquerdo; // Substituto é o filho esquerdo
        rb_transplante(t, z, z->esquerdo);
    } else {
        y = menor_elemento(z->direito);  // Encontra o menor elemento na subárvore direita
        y_cor_original = y->cor; // Salva a cor original de y
        x = y->direito;

        if (y->pai == z) {
            if (x) x->pai = y;
        } else {
            rb_transplante(t, y, y->direito);
            y->direito = z->direito;
            y->direito->pai = y;
        }

        rb_transplante(t, z, y);
        y->esquerdo = z->esquerdo;
        y->esquerdo->pai = y;
        y->cor = z->cor; // Mantém a cor original de z
    }

    // Corrige possíveis violações das propriedades da árvore rubro-negra
    if (y_cor_original == PRETO) {
        rb_delete_fixup(t, x);
    }
}


// Função para buscar um nó na árvore
struct no* buscar_no(struct arvore *t, int valor) {
    struct no *no_atual = t->raiz; // Começa a busca pela raiz

    while (no_atual != NULL && no_atual->valor != valor) {
        if (valor < no_atual->valor)
            no_atual = no_atual->esquerdo; // Vai para a subárvore esquerda
        else
            no_atual = no_atual->direito; // Vai para a subárvore direita
    }

    return no_atual; // Retorna o nó encontrado ou NULL se não encontrado
}
// Função para remover um valor da árvore
void remover_elemento(struct arvore *t, int valor) {
    struct no *no_remover = buscar_no(t, valor);
    if (no_remover != NULL) 
        rb_delete(t, no_remover);
}



