#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#include "ordenacao.h"

#define MAX_CHAR_NOME 50

int main() {
    char nome[MAX_CHAR_NOME];
    size_t idxBusca;
    size_t tamVetor;
    uint64_t numComp;
    int valor;
    
    getNome(nome);
    printf("Trabalho de %s\n", nome);
    printf("GRR %u\n", getGRR()); 
    
    /* le o tamanho do vetor */
    printf("Digite o tamanho do vetor: ");
    scanf("%zd", &tamVetor);

    /* aloca o vetor original e um auxiliar para ajudar na comparacao dos tipos de ordenacao */
    int* vetor = malloc(tamVetor * sizeof(int));
    int* vetor_original = malloc(tamVetor * sizeof(int));

    /* testa se as alocacoes funcionaram */
    if (vetor == NULL) {
        printf("Falha fatal. Impossível alocar memoria.");
        return 1;
    }

    if (vetor_original == NULL) {
        printf("Falha fatal. Impossível alocar memoria.");
        return 1;
    }

    /* gera valores aleatorios para o vetor */
    srand(time(NULL));
    for (size_t i = 0; i < tamVetor; i++) {
        vetor[i] = rand() % 100;
    }

    /* armazena as posicoes originais dos elementos */
    for (int i = 0; i < tamVetor; i++) {
        vetor_original[i] = vetor[i];
    }
    
    /* imprime o vetor gerado */
    for (int i = 0; i < tamVetor; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
    
    /* variaveis do tipo clock_t */
    clock_t start, end;
    double total;

    /*    VERSOES ITERATIVAS DE ORDENACAO   */
    /* calcula o tempo e o numero de comparacoes de cada algoritmo de ordenacao */
    
    /*   insertion sort   */
    start = clock(); 
    numComp = insertionSort(vetor, tamVetor);
    end = clock();
    total = ((double)end - start) / CLOCKS_PER_SEC;
    printf("\n");
    printf("Insertion sort iterativo tempo total: %f\n", total);
    printf("Insertion sort iterativo numero de comparacoes: %lu\n", numComp);

    /* restaura o vetor para continuar os testes */
    for (int i = 0; i < tamVetor; i++) {
        vetor[i] = vetor_original[i];
    }
    
    /* reinicializa o contador de comparacoes */
    numComp = 0;
    
    /*   selection sort   */
    start = clock(); 
    numComp = selectionSort(vetor, tamVetor);
    end = clock();
    total = ((double)end - start) / CLOCKS_PER_SEC;
    printf("\n");
    printf("Selection sort iterativo tempo total: %f\n", total);
    printf("Selection sort iterativo numero de comparacoes: %lu\n", numComp);
    
    /* restaura o vetor para continuar os testes */
    for (int i = 0; i < tamVetor; i++) {
        vetor[i] = vetor_original[i];
    }
    /* reinicializa o contador de comparacoes */
    numComp = 0;
    
    /*    VERSOES RECURSIVAS DE ORDENACAO   */
    /* calcula o tempo e o numero de comparacoes de cada algoritmo de ordenacao */
    
    /*   insertion sort   */
    start = clock(); 
    numComp = insertionSortRec(vetor, tamVetor);
    end = clock();
    total = ((double)end - start) / CLOCKS_PER_SEC;
    printf("\n");
    printf("Insertion sort recursivo tempo total: %f\n", total);
    printf("Insertion sort recursivo numero de comparacoes: %lu\n", numComp);
    
    /* restaura o vetor para continuar os testes */
    for (int i = 0; i < tamVetor; i++) {
        vetor[i] = vetor_original[i];
    }
    
    /* reinicializa o contador de comparacoes */
    numComp = 0;
    
    /*   selection sort   */
    start = clock(); 
    numComp = selectionSortRec(vetor, tamVetor);
    end = clock();
    total = ((double)end - start) / CLOCKS_PER_SEC;
    printf("\n");
    printf("Selection sort recursivo tempo total: %f\n", total);
    printf("Selection sort recursivo numero de comparacoes: %lu\n", numComp);
    
    /* restaura o vetor para continuar os testes */
    for (int i = 0; i < tamVetor; i++) {
        vetor[i] = vetor_original[i];
    }
    
    /* reinicializa o contador de comparacoes */
    numComp = 0;
    
    /* merge sort */
    start = clock();
    numComp = mergeSortRec(vetor, tamVetor);
    end = clock();
    total = ((double)end - start) / CLOCKS_PER_SEC;
    printf("\n");
    printf("Merge sort recursivo tempo total: %f\n", total);
    printf("Merge sort recursivo numero de comparacoes: %lu\n", numComp);
    printf("\n");

    /* imprime o vetor ordenado */
    printf("Vetor ordenado: ");
    for (int i = 0; i <tamVetor; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
    
    /* reinicializa o contador de comparacoes */
    numComp = 0;
    
    /* nos testes de busca seram feitos sempre com vetores ja ordenados
       tanto nos iterativos quanto os recursivos */
    
    /*   VERSOES ITERATIVAS DE BUSCA */
    /* imprime o indice do valor a ser buscado e o numero de comparacoes feitas */
    printf("\n");
    printf("Digite o elemento a ser buscado: ");
    scanf("%d", &valor);
    printf("\n");
    
    /* busca sequencial */
    idxBusca = buscaSequencial(vetor, tamVetor, valor, &numComp);
    printf("Busca sequencial iterativa indice do elemento: %zd\n", idxBusca);
    printf("Busca sequencial iterativa numero de comparacoes : %lu\n", numComp);
    printf("\n");
    /* reinicializa o contador de comparacoes */
    numComp = 0;
    
    /* busca binaria */
    idxBusca = buscaBinaria(vetor, tamVetor, valor, &numComp);
    printf("Busca binaria iterativa indice do elemento: %zd\n", idxBusca);
    printf("Busca binaria iterativa numero de comparacoes : %lu\n", numComp);
    printf("\n");
    /* reinicializa o contador de comparacoes */
    numComp = 0;
    
    /*   VERSOES RECURSIVAS DE BUSCA */
    /* imprime o indice do valor a ser buscado e o numero de comparacoes feitas */
    
    /* busca sequencial */
    idxBusca = buscaSequencialRec(vetor, tamVetor, valor, &numComp);
    printf("Busca sequencial recursiva indice do elemento: %zd\n", idxBusca);
    printf("Busca sequencial recursiva numero de comparacoes : %lu\n", numComp);
    printf("\n");
    /* reinicializa o contador de comparacoes */
    numComp = 0;
    
    /* busca binaria */
    idxBusca = buscaBinariaRec(vetor, tamVetor, valor, &numComp);
    printf("Busca binaria recursiva indice do elemento: %zd\n", idxBusca);
    printf("Busca binaria recursiva numero de comparacoes : %lu\n", numComp);
    printf("\n");
    /* reinicializa o contador de comparacoes */
    numComp = 0;
    
    /* desaloca os vetores */     
    free(vetor);
    free(vetor_original);

    return 0;
}