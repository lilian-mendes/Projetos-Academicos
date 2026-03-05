/*
 * TAD pilha
 * -> versao com lista ligada com nodo cabeca
 * Autores:
 *    Marcos Alexandre Castilho
 *    Luis Carlos Erpen de Bona
 *
 * Versao 3.0.0 de 03/10/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

/* 
 * Cria e retorna uma nova fila.
 * Retorna NULL em caso de erro de alocação.
*/
struct fila *fila_cria (){
    struct fila *nova_fila = (struct fila *)malloc(sizeof(struct fila));
    if (nova_fila != NULL) {
        nova_fila->ini = NULL;
        nova_fila->fim = NULL;
        nova_fila->tamanho = 0;
    }
    return nova_fila;
}

/* Desaloca toda memoria da fila e faz fila receber NULL. */
void fila_destroi (struct fila **fila){
    if (*fila != NULL) {
        while (!fila_vazia(*fila)) {
            int dado;
            dequeue(*fila, &dado);
        }
        free(*fila);
        *fila = NULL; 
    }
}

/* 
 * Insere dado na fila (politica FIFO). Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int enqueue (struct fila *fila, int dado){
    if (fila == NULL) {
        return 0; 
    }

    
    struct nodo *novo_nodo = (struct nodo *)malloc(sizeof(struct nodo));
    if (novo_nodo == NULL) {
        return 0; 
    }

    novo_nodo->chave = dado;
    novo_nodo->prox = NULL;

   
    if (fila->fim == NULL) {
        fila->ini = novo_nodo;
        fila->fim = novo_nodo;
    } else {
        
        fila->fim->prox = novo_nodo;
        fila->fim = novo_nodo;
    }

    fila->tamanho++; 
    return 1; 
}


/* 
 * Remove dado da fila (politica FIFO) e retorna o elemento 
 * no parametro dado. A funcao retorna 1 em caso de 
 * sucesso e 0 no caso da fila estar vazia.
*/
int dequeue (struct fila *fila, int *dado){
    if (fila == NULL || fila_vazia(fila)) {
        return 0; 
    }

    struct nodo *nodo_removido = fila->ini;
    *dado = nodo_removido->chave;

    fila->ini = nodo_removido->prox;

  
    if (fila->ini == NULL) {
        fila->fim = NULL;
    }

    free(nodo_removido); 
    fila->tamanho--;
    return 1;
}
 
/* Retorna o numero de elementos da pilha, que pode ser 0. */
int fila_tamanho (struct fila *fila){
    if (fila == NULL) {
        return 0;
    }
    return fila->tamanho;
}

/* Retorna 1 se fila vazia, 0 em caso contrario. */ 
int fila_vazia (struct fila *fila){
    if (fila == NULL || fila->ini == NULL) {
        return 1;
    }
    return 0; 
}

void fila_imprime(struct fila* fila) {
    if (fila->tamanho == 0) {
        printf(" ");
        return;
    }

    struct nodo* atual = fila->ini;

    while (atual != NULL) {
        printf("%d ", atual->chave);
        atual = atual->prox;
    }
}
