
#include "ordenacao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getNome(char nome[]){
	strncpy(nome, "Lilian Miranda Mendes", MAX_CHAR_NOME);
	nome[MAX_CHAR_NOME-1] = '\0';
}

uint32_t getGRR(){
	return 20222549;
}

/*    FUNCOES AUXILIARES    */

/* funcao para trocar dois elementos de posicao */
void troca(int vetor[], int a, int b){

	int temp = vetor[a];
	vetor[a] = vetor[b];
	vetor[b] = temp;
}

/* funcao que auxilia o insertion sort */
void insere(int vetor[], size_t tam, uint64_t* comp){

	int P = buscaBinariaRec(vetor,tam, vetor[tam], comp);
    int i = tam;
    
	while (i > (P + 1)){
		troca(vetor, i, i - 1);
		i--;
	}
	return;
}

/* funcao para achar o menor valor do vetor */
int minimo(int vetor[], int a, int b, uint64_t *comp) {
    if (a > b)
        return -1;
    if (a == b)
        return a;

    int m = minimo(vetor, a, b - 1, comp);

    (*comp)++;
    if (vetor[b] < vetor[m]) {
        m = b;
    }
    return m;
}

/* funcao intercala auxiliar do merge sort */
uint64_t merge(int vetor[], int a, int meio, int b, uint64_t *comp){    
    int i = a;
    int j = meio + 1;
    int tamanho = (b - a) + 1;
    int *u;

    if (!(u = malloc(tamanho * sizeof(int)))) {
        printf("Erro de alocação de memória.\n");
        return 0; // Retorna 0 para indicar falha na alocação de memória
    }

    if (a > b) {
        // Retorna o número de comparações realizadas
        free(u);
        return *comp;
    }

    for (int k = 0; k < tamanho; k++) {
        (*comp)++; // Incrementa o número de comparações
        if (j > b || (i <= meio && vetor[i] <= vetor[j])) {
            u[k] = vetor[i];
            i++;
        } else {
            u[k] = vetor[j];
            j++;
        }
    }

    for (int x = 0, y = a; x < tamanho; x++, y++) {
        vetor[y] = u[x];
    } 

    free(u);
    return *comp;
}


/* ALGORITMOS DE ORDENACAO */
/*    VERSOES ITERATIVAS    */

/* codigo iterativo do insertion sort */
uint64_t insertionSort(int vetor[], size_t tam) {
    /* percorre o vetor e chama o insere para ordenar*/
    uint64_t numComparacoes = 0;
    int j,aux;
    for (size_t i = 1; i < tam; i++) {
        aux = vetor[i];
        j = i - 1;
        while (j >= 0 && vetor[j] > aux){
            vetor [ j + 1] = vetor[j];
            numComparacoes++;
            j--;
        }
        vetor[j + 1] = aux;
    }

    /* retorna o numero de comparacoes */
    return numComparacoes;
}

uint64_t selectionSort(int vetor[], size_t tam) {
    uint64_t  numComparacoes = 0;
    for (size_t i = 0; i < tam - 1; i++) {
        int indice_min = i;
        for (size_t j = i + 1; j < tam; j++) {
            numComparacoes++;
            if (vetor[j] < vetor[indice_min]) {
                indice_min = j;
            }
        }
        if (indice_min != i) {
            troca(vetor, i, indice_min);
        }
    }
    /* retorna o numero de comparacoes */
    return numComparacoes;
}

/*    VERSOES RECURSIVAS    */
/* funcao auxiliar do insertion sort recursivo */
void insertionSortRecursivo(int vetor[], size_t tam, uint64_t* numComparacoes){
	int a = 0;
	if (a >= tam) 
        return;

    /* percorre o vetor e chama o insere para ordenar*/
        insertionSortRecursivo(vetor, tam - 1, numComparacoes);
        insere(vetor, tam, numComparacoes);
    
    /* retorna o numero de comparacoes */
    return;
}

uint64_t insertionSortRec(int vetor[], size_t tam) {
    uint64_t numComparacoes = 0;
    insertionSortRecursivo(vetor, tam - 1, &numComparacoes);
    return numComparacoes;
}

/* funcao auxiliar do selection sort recursivo */
uint64_t selectionSortRecursivo(int vetor[], int a, int b, uint64_t* numComparacoes) {
	if (a >= b){
		return *numComparacoes;
	}
 
    /* acha o menor valor do vetor e troca pelo primeiro indice nao ordenado */
	int m = minimo(vetor, a, b, numComparacoes);
	troca(vetor, a, m);
	selectionSortRecursivo(vetor, a + 1, b, numComparacoes);

    /* retorna o numero de comparacoes */
	return *numComparacoes;
}

uint64_t selectionSortRec(int vetor[], size_t tam) {
    uint64_t numComparacoes = 0;
    return selectionSortRecursivo(vetor, 0, tam - 1, &numComparacoes);
}

/* Funcao auxiliar do merge sort */
void mergeSortAux(int vetor[], int a, int b, uint64_t* numComparacoes) {
    
    if (a >= b) 
        return ;
    
    int m = (a + b) / 2;
 
 
    mergeSortAux(vetor, a, m, numComparacoes);
    mergeSortAux(vetor, m + 1, b, numComparacoes);
    merge(vetor,a,m,b,numComparacoes);

    (*numComparacoes)++;

    return;
}

/* Funcao de ordenacao por merge sort */

uint64_t mergeSortRec(int vetor[], size_t tam) {
    uint64_t numComparacoes = 0;
    mergeSortAux(vetor, 0, tam - 1, &numComparacoes);
    return numComparacoes;
}

/* ALGORITMOS DE BUSCA */ 
/*    VERSOES ITERATIVAS    */

/* codigo iterativo da funcao busca sequencial */
ssize_t buscaSequencial(int vetor[], size_t tam, int valor, uint64_t* numComparacoes) {
    /* Percorre o vetor a partir da primeira posicao */
    for (ssize_t i = 0; i < tam; i++) {
        (*numComparacoes)++; 
        /* Retorna o indice da posicao em que x esta */
        if (valor == vetor[i])
            return i;
    }
    /* Retorna -1 se nao achar */
    return -1;
}

/* codigo da busca binaria iterativa */
ssize_t buscaBinaria(int vetor[], size_t tam, int valor, uint64_t* numComparacoes) {
    ssize_t a = 0;
    ssize_t b = tam;
    while (a <= b) {
        /* encontra o meio do vetor */
        ssize_t m = (a + b) / 2;

        (*numComparacoes)++; 

        /* retorna o índice do valor no vetor */
        if (valor == vetor[m]) {
            return m;
        }

        (*numComparacoes)++; 

        /* se o valor de x for menor do que o do meio do vetor, procura na metade direita do vetor */
        if (valor < vetor[m]) 
            b = m - 1;
        /* se o valor de x for maior do que o do meio do vetor, procura na metade esquerda do vetor */
        else 
            a = m + 1;
    }
    /* retorna -1 se nao achar ou se o vetor nao for valido */
    return -1;
}


// VERSOES RECURSIVAS 
// codigo recursivo da funcao busca sequencial 
ssize_t buscaSequencialRec(int vetor[], size_t tam, int valor, uint64_t* numComparacoes) {
    /* retorna -1 se o tamanho do vetor for menor ou igual a 0 */
    if (tam <= 0)
        return -1;

    /* incrementa o contador de comparacoes */
    (*numComparacoes)++;

    /* retorna o indice da posicao onde x esta */
    if (valor == vetor[tam - 1])
        return tam - 1;

    /* chama recursivamente a funcao com o vetor de tamanho reduzido */
    return buscaSequencialRec(vetor, tam - 1, valor, numComparacoes);
}


/* codigo auxiliar recursiva da busca binaria */
ssize_t buscaBinariaRecursiva(int vetor[], int a, int b, int valor, uint64_t* numComparacoes) {
    /* retorna -1 se nao achar */
    if (a > b) {
        return -1;
    }

    /* acha o meio do vetor */
    int m = ((a + b) / 2);

    (*numComparacoes)++;
    if (valor == vetor[m]) {
        /* retorna o indice do x no vetor */
        return m;
    }

    (*numComparacoes)++;
    if (valor < vetor[m]) {
        /* se o valor de x for menor do que o do meio do vetor, procura na metade direita do vetor */
        return buscaBinariaRecursiva(vetor, a, m - 1, valor, numComparacoes);
    }
    /* se o valor de x for maior do que o do meio do vetor, procura na metade esquerda do vetor */
    return buscaBinariaRecursiva(vetor, m + 1, b, valor, numComparacoes);
}

ssize_t buscaBinariaRec(int vetor[], size_t tam, int valor, uint64_t* numComparacoes) {
    *numComparacoes = 0; 
    return buscaBinariaRecursiva(vetor, 0, tam - 1, valor, numComparacoes);
}
