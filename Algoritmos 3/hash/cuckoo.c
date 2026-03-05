#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cuckoo.h"

/* funcao de hash para a tabela 1 */
int h1(int chave) {
    return chave % TAM_MAX;
}

/* funcao de hash para a tabela 2 */
int h2(int chave) {
    /* retorna o piso do calculo de h2(k) = ⌊m * (k * 0.9 − ⌊k * 0.9⌋)⌋*/
    return (int)floor(TAM_MAX * (chave * 0.9 - floor(chave * 0.9)));
}

/* funcao de busca em ambas as tabelas */
int busca(struct tabela_hash *T1, struct tabela_hash *T2, int chave) {

    /* busca primeiro na tabela 1 */
    int i = h1(chave);
    if (T1[i].estado == 1 && T1[i].chave == chave) {
        printf("Chave encontrada na tabela T1, posição %d.\n", i);
        return i;  
    }

    /* se nao estiver na tabela 1 busca na tabela 2 */
    i = h2(chave);
    if (T2[i].estado == 1 && T2[i].chave == chave) { 
        printf("Chave encontrada na tabela T2, posição %d.\n", i);
        return i;
    }
  
    /* retorna -1 se nao encontrar a chave sendo buscada */
    printf("Chave não encontrada.\n");
    return -1;
}

/* funcao para ordenar as chaves */
void ordena_chaves(struct tabela_hash *temp, int aux) {
    for (int i = 0; i < aux; i++) {
        int j = i;
        while (j> 0 && temp[j].chave < temp[j - 1].chave) {
            /* troca se estiverem fora de ordem */
            struct tabela_hash temp2 = temp[j];
            temp[j] = temp[j - 1];
            temp[j - 1] = temp2;
            j--;
        }
    }
}

/* verifica se uma chave ja foi incluida na tabela 1*/
int chave_existe_T1(struct tabela_hash *T1, int chave) {
    int i = h1(chave);

    /* se a chave ja esta na T1 entao retorna 1 */
    return (T1[i].estado == 1 && T1[i].chave == chave);
}

/* verifica se uma chave ja foi incluida na tabela 2*/
int chave_existe_T2(struct tabela_hash *T2, int chave) {
    int i = h2(chave);

    /* se a chave ja esta na T2 entao retorna 1 */
    return (T2[i].estado == 1 && T2[i].chave == chave);
}

/* funcao para inserir na tabela 1 ou na tabela 2 */
void insere(struct tabela_hash *T1, struct tabela_hash *T2, int chave) {
    int i, temp_chave;

    /* se a chave ja esta em alguma das duas tabelas ela eh ignorada */
    if (chave_existe_T1(T1, chave) || chave_existe_T2(T2, chave)) 
        return;

    /* calcula o indice usando a funcao hash da tabela 1 */
    i = h1(chave);

    /* se a posicao esta livre ou foi removida insere na tabela 1 */
    if (T1[i].estado == 0 || T1[i].estado == 2) {
        T1[i].chave = chave;
        /* altera o estado da posicao para ocupado */
        T1[i].estado = 1;
        return;
    }

    /* se a posicao na tabela 1 esta ocupada */ 
    /* guarda o valor que esta ocupando em um temp */
    temp_chave = T1[i].chave;

    /* e coloca a chave a ser inserida na tabela 1 */
    T1[i].chave = chave;

    /* atualiza o seu estado */
    T1[i].estado = 1;

    /* atualiza o valor da chave a ser inserida */
    chave = temp_chave;

    /* calcula o indice usando a funcao hash da tabela 2 */
    i = h2(chave);

    /* se a posicao esta livre ou foi removida insere na tabela 2 */
    if (T2[i].estado == 0 || T2[i].estado == 2) {
        T2[i].chave = chave;
        T2[i].estado = 1;
        return;
    }
}

/* funcao para remover uma chave de uma das tabelas */
void remover(struct tabela_hash *T1, struct tabela_hash *T2, int chave) {

    /* calcula o indice usando a funcao hash da tabela 2 */
    int i = h2(chave);

    /* se estiver na tabela 1 altera o seu estado para removido */
    if (T2[i].estado == 1 && T2[i].chave == chave) {
        T2[i].estado = 2; 
        return;
    }

    /* se nao esta na tabela 2, verifica a tabela 1 */
    /* calcula o indice usando a funcao hash da tabela 2 */
    i = h1(chave);
    /* altera o seu estado para removido */
    if (T1[i].estado == 1 && T1[i].chave == chave) {
        T1[i].estado = 2;
        return;
    }
}

/* funcao para imprimir as tabelas de forma ordenada */
void imprime(struct tabela_hash *T1, struct tabela_hash *T2) {
    struct tabela_hash temp[TAM_MAX * 2];
    int aux = 0;

    /* copia para temp todas as chaves validas da tabela 1, ou seja, que estao com estado = 1 */
    for (int i = 0; i < TAM_MAX; i++) {
        if (T1[i].estado == 1) 
            temp[aux++] = T1[i];
    }

    /* copia tambem as chaves validas da tabela 2 so que evitando chaves duplicadas */
    for (int j = 0; j < TAM_MAX; j++) {
        if (T2[j].estado == 1) {
            int k = 0;
            for (k = 0; k < aux; k++) {
                /* se eh uma chave que ja foi copiada sai do loop */
                if (T2[j].chave == temp[k].chave)
                break;
            }

            if (k == aux)
            temp[aux++] = T2[j];
        }
    }

    /* ordena as chaves para poder imprimir */
    ordena_chaves(temp, aux);

    /* imprime a chave, a tabela e o indice de cada elemento */
    for (int n = 0; n < aux; n++) {
        int tabela;
        int indice;

        if (T1[h1(temp[n].chave)].chave == temp[n].chave) {
            tabela = 1;
            indice = h1(temp[n].chave);
        } 
        else {
            tabela = 2;
            indice = h2(temp[n].chave);
        }
        
        printf("%d,T%d,%d\n", temp[n].chave, tabela, indice);
    }
}