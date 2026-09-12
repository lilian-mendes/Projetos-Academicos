#include <stdio.h>
#include <stdlib.h>

#include "jukefila.h"
#include "pedido.h"

jukefila* criar_jukefila(){

    jukefila *nova_fila = (jukefila*)malloc(sizeof(jukefila));
    if (!nova_fila) {
        nova_fila->inicio = NULL;
        nova_fila->final = NULL;
    }
    return nova_fila;
}

void inserir_jukefila(pedido* elemento, jukefila* fila) {

    /* se a fila esta vazia, o elemento vai ser o primeiro */
    if (fila->inicio == NULL) {
        fila->inicio = elemento;
        fila->final = elemento;
        elemento->proximo = NULL;
        elemento->anterior = NULL;
        return;
    }

    /* se o valor dado for maior que o valor do primeiro pedido */
    if (elemento->valor > fila->inicio->valor) {
        fila->inicio->anterior = elemento;
        elemento->proximo = fila->inicio;
        fila->inicio = elemento;
        elemento->anterior = NULL;
        return;
    }

    /* se o valor dado for menor que o valor do ultimo pedido */
    if (elemento->valor <= fila->final->valor) {
        fila->final->proximo = elemento;
        elemento->anterior = fila->final;
        fila->final = elemento;
        elemento->proximo = NULL;
        return;
    }

    /* acha a posicao do novo pedido e insere */
    pedido* novo_pedido = fila->inicio;
    while (novo_pedido != NULL && novo_pedido->valor >= elemento->valor) {
        novo_pedido = novo_pedido->proximo;
    }

    elemento->proximo = novo_pedido;
    elemento->anterior = novo_pedido->anterior;

    if (novo_pedido->anterior != NULL) {
        novo_pedido->anterior->proximo = elemento;
    }
    if (novo_pedido != NULL) {
        novo_pedido->anterior = elemento;
    }
}

pedido* consumir_jukefila(jukefila* fila){

    if (!fila || !fila->inicio)
        return NULL;

    pedido *retorno = fila->inicio;

    /* se a fila tiver mais de uma pedido, coloca o proximo pedido como inicio */
    if (fila->inicio->proximo) {
        fila->inicio = retorno->proximo;
    } else {
        fila->inicio = NULL;
        fila->final = NULL; 
    }

    /* retorna a pedido */
    return retorno;
}

unsigned int contar_jukefila(jukefila* fila){

    unsigned int pedidos = 0;

    pedido *temp = fila->inicio;

    /* percorre a fila para contar a quantidade de pedidos */
    while (temp != NULL) {
        pedidos++;
        temp = temp->proximo;
    }

    return pedidos;
}

void destruir_jukefila(jukefila *fila){
    
    while(fila->inicio)
        free(consumir_jukefila(fila));
    free(fila);
}