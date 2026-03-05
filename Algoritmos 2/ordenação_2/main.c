#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ordenacao.h"

int main() {

    char nome[MAX_CHAR];
    uint64_t numComp = 0;
    int tamVetor;
    
    getNome(nome);
    printf("Trabalho de %s\n", nome);
    printf("GRR %u\n", getGRR());

    /* le o tamanho do vetor */
    printf("Digite um tamanho para o vetor: ");
    scanf("%d", &tamVetor);
     
    /* aloca o vetor e retorna se não for possivel realizar a alocação */
    int* vetor = (int*)malloc(tamVetor * sizeof(int));
    if (vetor == NULL) {
        printf("Falha fatal. Impossível alocar memoria.\n");
        return 1;
    }

    /* aloca um vetor auxiliar que armazena a posição original dos elementos */
    int* vetor_aux = (int*)malloc(tamVetor * sizeof(int));
    if (vetor_aux == NULL) {
        printf("Falha fatal. Impossível alocar memoria.\n");
        return 1;
    }

    /* gera valores aleatorios para o vetor */
    srand(time(NULL));
    for (int i = 0; i < tamVetor; i++) {
        vetor[i] = rand() % 100;
    }
    
    /* passa os valores para o vetor auxiliar */
    for(int i = 0; i < tamVetor; i++) {
        vetor_aux[i] = vetor[i];
    }

    /* imprime o vetor original */
    printf("Vetor original: ");
    printf("[ ");
    for(int i = 0; i < tamVetor; i++)
        printf("%d ", vetor[i]);
    printf("]\n");
    printf("\n");

    /*   MERGE SORT   */
    
    printf("- Testes com o algoritmo Merge Sort\n");
    numComp = mergeSortSR(vetor, tamVetor);
    printf("Número de comparações Merge Sort iterativo: %ld\n", numComp);

    /* restaura o vetor */
    for (int i = 0; i < tamVetor; i++) {
        vetor[i] = vetor_aux[i];
    }
    
    /* ---- versão recursiva ---- */
    numComp = mergeSort(vetor, tamVetor);
    printf("Número de comparações Merge Sort recursivo: %ld\n", numComp);
    
    /* restaura o vetor */
    for (int i = 0; i < tamVetor; i++) {
        vetor[i] = vetor_aux[i];
    }

    /*   QUICK SORT   */

    printf("- Testes com o algoritmo Quick Sort\n");
    
    /* ---- versão iterativa ---- */
    numComp = quickSortSR(vetor, tamVetor);
    printf("Número de comparações Quick Sort iterativo: %ld\n", numComp);
    
    /* restaura o vetor */
    for (int i = 0; i < tamVetor; i++) {
        vetor[i] = vetor_aux[i];
    }
    
    numComp = quickSort(vetor, tamVetor);
    printf("Número de comparações Quick Sort recursivo: %ld\n", numComp);
    
    /* restaura o vetor */
    for (int i = 0; i < tamVetor; i++) {
        vetor[i] = vetor_aux[i];
    }

    /*   HEAP SORT   */

    printf("- Testes com o algoritmo Heap Sort\n");
    
    /* ---- versão iterativa ---- */
    numComp = heapSortSR(vetor, tamVetor);
    printf("Número de comparações Heap Sort iterativo: %ld\n", numComp);
    
    /* restaura o vetor */
    for (int i = 0; i < tamVetor; i++) {
        vetor[i] = vetor_aux[i];
    }

    /* ---- versão recursiva ---- */
    numComp = heapSort(vetor, tamVetor);
    printf("Número de comparações Heap Sort recursivo: %ld\n", numComp);
    
    /* imprime o vetor ordenado */
    printf("Vetor ordenado: ");
    printf("[ ");
    for(int i = 0; i < tamVetor; i++) 
        printf("%d ", vetor[i]);
    printf("]\n");

    /* libera a memória alocada para o vetores original e auxiliar  */
    free(vetor);
    free(vetor_aux);

    return 0;
}
