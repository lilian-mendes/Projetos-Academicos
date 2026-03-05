#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"

struct conjunto *cria_cjt (int max){
    struct conjunto *c = malloc(sizeof(struct conjunto));
    if (c == NULL) 
        return NULL;
    
    /* coloca o valor de max fornecido em c->max */
    c->max = max;
    c->card = 0;
    c->ptr = 0;
    /* aloca um vetor de tamanho max */
    c->v = malloc(max * sizeof(int));
    
    if (c->v == NULL) {
        free(c);
        return NULL;
    }
    
    return c;
}

/* desaloca um conjunto */
struct conjunto *destroi_cjt (struct conjunto *c){
    if (c != NULL) {
        free(c->v);
        free(c);
    }
    return NULL;
}

/* retorna 1 se o conjunto esta vazio e 0 caso contrario */
int vazio_cjt (struct conjunto *c){
    return (c == NULL);
}

/* retorna a cardinalidade do conjunto */
int cardinalidade_cjt (struct conjunto *c){
    if (c == NULL) 
        return 0;

    return c->card;

}

int insere_cjt (struct conjunto *c, int elemento){
    if (c == NULL || c->card == c->max) 
        return 0;
    /* insere o elemento se ele ja nao estiver no conjunto */
    if (!pertence_cjt(c, elemento)) {
        c->v[c->card++] = elemento;
        return 1;
    }
    
    return 1;
}

int retira_cjt (struct conjunto *c, int elemento){
    if (c == NULL) 
        return 0;

    int elemento_removido = 0;

    for (int i = 0; i < c->card; i++) {
        if (c->v[i] == elemento) {
            /* marca que o elemento foi removido*/
            elemento_removido = 1; 
        } 
        else if (elemento_removido) {
            /* move os outros elementos para a esquerda */
            c->v[i - 1] = c->v[i];
        }
    }

    if (elemento_removido) {
        /* se o elemento foi removido decrementa card */
        c->card--;
        return 1;
    }

    return 0;
}

int pertence_cjt(struct conjunto *c, int elemento) {
    if (c == NULL) 
        return 0;
    /* verifica se o elemento esta no conjunto */
    for (int i = 0; i < c->card; i++) {
        if (c->v[i] == elemento) 
            return 1;
    }
    
    return 0;
}

int contido_cjt(struct conjunto *c1, struct conjunto *c2) {
    if (c1 == NULL) 
        return 1;
    /* verifica se o conjunto c1 esta contido no c2*/
    for (int i = 0; i < c1->card; i++) {
        if (!pertence_cjt(c2, c1->v[i])) 
            return 0;
    }
    
    return 1;
}

/* verifica se os conjuntos c1 e c2 sao iguais */
int sao_iguais_cjt (struct conjunto *c1, struct conjunto *c2){
    if (c1 == NULL && c2 == NULL) 
        return 1;

    if (c1 == NULL || c2 == NULL || c1->card != c2->card) 
        return 0;
        
    for (int i = 0; i < c1->card; i++) {
            if (!pertence_cjt(c2, c1->v[i])) 
                return 0;
    }
        
    return 1;
}

struct conjunto *diferenca_cjt (struct conjunto *c1, struct conjunto *c2){
    if (c1 == NULL || c2 == NULL) 
        return NULL;
    
    struct conjunto *result = cria_cjt(c1->max);
    
    if (result == NULL) 
        return NULL;
    
    for (int i = 0; i < c1->card; i++) {
        if (!pertence_cjt(c2, c1->v[i]))
            /* coloca em result os elementos que estao em c1 mas nao estao em c2 */
            insere_cjt(result, c1->v[i]);
    }
    
    return result;
}

struct conjunto *interseccao_cjt (struct conjunto *c1, struct conjunto *c2){
    if (c1 == NULL || c2 == NULL) 
        return NULL;
    
    struct conjunto *result = cria_cjt(c1->max);
    
    if (result == NULL)
        return NULL;

    for (int i = 0; i < c1->card; i++) {
        if (pertence_cjt(c2, c1->v[i])) 
            /* coloca em result os elementos que estao em c1 e c2 */
            insere_cjt(result, c1->v[i]);
    }
    
    return result;
}

struct conjunto *uniao_cjt (struct conjunto *c1, struct conjunto *c2){
    if (c1 == NULL && c2 == NULL) 
        return NULL;
        
    struct conjunto *result = cria_cjt(c1->max);
    
    if (result == NULL) 
        return NULL;

    /* coloca em result todos os elementos de c1 */
    for (int i = 0; i < c1->card; i++) {
        insere_cjt(result, c1->v[i]);
    }
    /* coloca em result todos os elementos de c2 */
    for (int i = 0; i < c2->card; i++) {
        insere_cjt(result, c2->v[i]);
    }
    
    return result;
}

struct conjunto *copia_cjt (struct conjunto *c){
    if (c == NULL) 
        return NULL;
    
    struct conjunto *result = cria_cjt(c->max);
    
    if (result == NULL) 
        return NULL;
        /* copia em result o conjunto c */
    for (int i = 0; i < c->card; i++) {
        insere_cjt(result, c->v[i]);
    }
    
    return result;
}

struct conjunto *cria_subcjt_cjt (struct conjunto *c, int n){
    if (c == NULL) 
        return NULL;
    
    /* se n for maior que card o conjunto a ser copiado eh o proprio c */
    if (n >= c->card) 
        return copia_cjt(c);
    
    struct conjunto *result = cria_cjt(c->max);
    
    if (result == NULL) 
        return NULL;

    /* percorre n elementos e escolhe aleatoriamente um elemento do conjunto e o coloca no subconjunto */
    for (int i = 0; i < n; i++) {
        int randIndice = rand() % c->card;
        insere_cjt(result, c->v[randIndice]);
    }
    
    return result;
}

/* imprime o conjunto caso ele seja valido ou NULL caso contrario */
void imprime_cjt (struct conjunto *c){
     printf("[");

    for (int i = 0; i < c->card; ++i) {
        printf(" %d", c->v[i]);
    }

    printf(" ]\n");
}


void inicia_iterador_cjt (struct conjunto *c){
    if (c != NULL) 
        c->ptr = 0;

    return;
}

int incrementa_iterador_cjt (struct conjunto *c, int *ret_iterador){
    if (c == NULL || c->ptr >= c->card)
        return 0;
    
    *ret_iterador = c->v[c->ptr];
    c->ptr++;
    
    return 1;
}

int retira_um_elemento_cjt (struct conjunto *c){
    if (c->card > 0) {
        int elemento = c->v[c->card - 1];
        c->card--;

        return elemento;
    }

    return 0;
}
