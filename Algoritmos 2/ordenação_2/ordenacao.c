#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ordenacao.h"

/* VERSÃO ATUAL */

typedef struct pilha {
	int *v_indice;
	int tamanho;
	int topo;
} pilha;

void getNome(char nome[]) {
    strncpy(nome, "Lilian Miranda Mendes", MAX_CHAR);
    // adicionada terminação manual para caso de overflow
    nome[MAX_CHAR - 1] = '\0';
}

uint32_t getGRR() { 
    return 20222549; 
}

/*   funções auxiliares para a estrutura pilha   */

/* função para criar uma pilha */
pilha* cria_pilha(int tam) {
    struct pilha* p;
    
    p = (pilha *)malloc(sizeof(pilha));
    
    /* retorna 1 se não foi possível alocar a pilha */
    if (p == NULL) {
        printf("Falha fatal. Impossível alocar memoria.");
        return NULL;
    }
    
    p->v_indice = (int *)malloc(tam * sizeof(int));
    
    /* retorna 1 se não foi possível alocar o v_indice */
    if (p->v_indice == NULL) {
        printf("Falha fatal. Impossível alocar memoria.\n");
        return NULL;
    }
    
    p->tamanho = tam; 
    p->topo = -1;
    
    return p;
} 

/* retorna 1 se a pilha estiver vazia e 0 caso contrario */
int vazia(pilha* p) {
	return p->topo == -1;
}

void destroi_pilha(pilha* p){
	free(p->v_indice);
	free(p);
}

/* função para empilhar */
void empilhar(pilha* p, int i) {
	if(p->topo == p->tamanho -1) {
		printf("Impossível empilhar. Pilha cheia\n");
		return;
	}
	p->v_indice[++p->topo] = i;
}

/* função para desempilhar */
int desempilhar(pilha* p) {
	if(!vazia(p))
        return p->v_indice[p->topo--];

    printf("Impossível desempilhar. Pilha vazia\n");
	return -1;	
}

/*   FUNÇÕES AUXILIARES   */

/* função auxiliar para troca de elementos */
void troca(int vetor[], int a, int b) {
  int temp = vetor[a];
  vetor[a] = vetor[b];
  vetor[b] = temp;
}

/* função auxiliar particiona */
int particiona(int vetor[], int inicio, int fim, uint64_t *numComp) {
    /* seleciona o último elemento como pivô */
    int pivo = vetor[fim];

    /* índice do menor elemento */
    int menor = (inicio - 1);

    for (int i = inicio; i < fim; i++) {
        (*numComp)++;
        if (vetor[i] <= pivo) {
            menor++;
            troca(vetor, menor, i);
        }
    }
    troca(vetor, menor + 1, fim);

    /* retorna a posição do pivô */
    return (menor + 1);
}

/* função auxiliar para juntar dois vetores */
void merge(int vetor[], int inicio, int meio, int fim, uint64_t *numComp) {
    int i = inicio;
    int j = meio + 1;
    int tamanho = fim - inicio + 1;

    /* aloca o vetor auxiliar da função */
    int *vetorAux = (int*)malloc(tamanho * sizeof(int));
    
    /* retorna se não for possível alocar */
    if (vetorAux == NULL) {
        printf("Erro ao alocar o vetor auxiliar na função merge.\n");
        return;
    }

    /* loop principal que junta os sub vetores */
    for (int k = 0; k < tamanho; k++) {
        (*numComp)++;

        if ( i <= meio && (j > fim || vetor[i] <= vetor[j])) {
            vetorAux[k] = vetor[i];
            i++;
        } 
        else {
            vetorAux[k] = vetor[j];
            j++;
        }
    }

    /* copia o vetor ordenado para o vetor original */
    for (int l = 0, m = inicio; l < tamanho; l++, m++) {
        vetor[m] = vetorAux[l];
    }

    /* libera a memória alocada */
    free(vetorAux);
}

/* função auxiliar que constrói uma max heap */
void max_heapfy(int vetor[], int i, size_t tam, uint64_t *numComp) {
    int maior; /* indice do pai */
    int esq = 2 * i + 1;   /* filho da esquerda */
    int dir = 2 * i + 2; /* filho da direita */

    /* compara o elemento pai com os filhos e encontra o maior */
    if (esq < tam && vetor[esq] > vetor[i]) {
        (*numComp)++;
        maior = esq;
    }
    else
    	maior = i;

    if (dir < tam && vetor[dir] > vetor[maior]) {
        (*numComp)++;
        maior = dir;
    }
    /* se o maior elemento não for o pai, troca e chama */
    /* recursivamente max_heapfy no filho afetado */
    if (maior != i) {
        troca(vetor, i, maior);
        max_heapfy(vetor, maior, tam, numComp);
    }
}
void construirMaxHeap(int vetor[], size_t tam, uint64_t *numComp) {
    for (int i = (tam / 2 - 1); i >= 0; i--)
        max_heapfy(vetor, i, tam, numComp);
}

void heapfy_iterativo(int vetor[], size_t tam, int i, uint64_t *numComp) {
    int maior = i; /* índice do pai */
    int temp = maior;

    do {
        maior = temp;
        int esq = 2 * maior + 1; /* filho da esquerda */
        int dir = 2 * maior + 2; /* filho da direita */

        if (esq < tam) {
            (*numComp)++;
            /* se o filho da esquerda é maior que o pai */
            if (vetor[esq] > vetor[temp])
                temp = esq;
        }

        if (dir <tam) {
            (*numComp)++;
            /* se o filho da direita é maior que o pai */
            if (vetor[dir] > vetor[temp])
                temp = dir;
        }

        if (maior != temp) {
            troca(vetor, maior, temp);
        }
    } while (maior != temp);
}

void mergeSortIterativo(int vetor[], int tam, uint64_t *numComp) {
	int a, b;
	int c = 1;
	
	while(c < tam) {
	    a = 0;
	    while (a + c < tam) {
	        b = (a + 2 * c);
	        if (b > tam)
	            b = tam;
	        merge(vetor, a, (a + c - 1), b - 1, numComp);
	        a = a + 2 * c;
	    }
	    c = 2 * c;
	}
}

/* função auxiliar do merge sort recursivo */
void mergeSortRecursivo(int vetor[], int esquerda, int direita, uint64_t *numComp) {
    /* caso base */
    if (esquerda >= direita) 
        return;
    
    /* define o meio do vetor */
    int meio = (esquerda + (direita - esquerda) / 2);

    /* chamada recursiva para a parte da esquerda do vetor */
    mergeSortRecursivo(vetor, esquerda, meio, numComp);

    /* chamada recursiva para a parte da direita do vetor */
    mergeSortRecursivo(vetor, meio + 1, direita, numComp);

    /* chama a função merge para juntar as duas metades */
    merge(vetor, esquerda, meio, direita, numComp);
}

/* função auxiliar do quick sort recursivo */
void quickSortRecursivo(int vetor[], int inicio, int fim, uint64_t *numComp) {
    /* caso base */
    if(inicio >= fim) 
        return;

    /* particiona o vetor */
    int meio = particiona(vetor, inicio, fim, numComp);

    /* chamada recursiva para o lado esquerdo do pivô */
    quickSortRecursivo(vetor, inicio, meio - 1, numComp);

    /* chamada recursiva para o lado direito do pivô */
    quickSortRecursivo(vetor, meio + 1, fim, numComp);
}

/* função auxiliar do heap sort recursivo */
void heapSortRecursivo(int vetor[], size_t tam, uint64_t *numComp) {
    /* constrói a MaxHeap */
    construirMaxHeap(vetor, tam, numComp);

    for (int i = tam - 1; i > 0; i--) {
        troca(vetor, 0, i);
        max_heapfy(vetor, 0, i, numComp);
    }
}

/*   VERSÕES RFECURSIVAS   */

/* funcao merge sort recursivo */
uint64_t mergeSort(int vetor[], size_t tam) {
    uint64_t numComp = 0;
    mergeSortRecursivo(vetor, 0, tam - 1, &numComp);
    return numComp;
}

/* funcao quick sort recursivo */
uint64_t quickSort(int vetor[], size_t tam) {
    uint64_t numComp = 0;
    quickSortRecursivo(vetor, 0, tam - 1, &numComp);
    return numComp;
}

/* funcao heap sort recursivo */
uint64_t heapSort(int vetor[], size_t tam) {
    uint64_t numComp = 0;
    heapSortRecursivo(vetor, tam, &numComp);
    return numComp;
}


/*    VERSÕES ITERATIVAS   */

/* Função principal do Merge Sort iterativo */
uint64_t mergeSortSR(int vetor[], size_t tam) {
    uint64_t numComp = 0;
    mergeSortIterativo(vetor, tam, &numComp);
    return numComp;
}

/* funcao principal do quick sort iterativo */
uint64_t quickSortSR(int vetor[], size_t tam) {
    uint64_t numComp = 0;
    struct pilha *p;
    int inicio = 0;
    int fim = tam - 1;

    /*cria e inicializa a pilha */
    p = cria_pilha((fim - inicio + 1) * 2);
    empilhar(p, inicio);
    empilhar(p, fim);

    /* enquanto a pilha não estiver vazia */
    do {
        fim = desempilhar(p);
        inicio = desempilhar(p);

        /* se a pilha tiver mais de um elemento */
        if (inicio < fim) {
            /*particiona o vetor */
            int meio = particiona(vetor, inicio, fim, &numComp);

            /* empilha elementos à esquerda do pivô */
            empilhar(p, inicio);
            empilhar(p, meio - 1);

            /* empilha elementos à direita do pivô */
            empilhar(p, meio + 1);
            empilhar(p, fim);
        }
    } while (!vazia(p));

    /* libera a memória alocada para a pilha */
    destroi_pilha(p);
    return numComp;
}

/* funcao principal do heap sort iterativo */
uint64_t heapSortSR(int vetor[], size_t tam) {
    uint64_t numComp = 0;
    
    /* constrói o heap */
    for(int i = (tam / 2 - 1); i >= 0; i--) 
        heapfy_iterativo(vetor, tam, i, &numComp);
    
    /* tira os elementos do heap um por um */
    for (int i = (tam - 1); i > 0; i--) {
        /* move a raíz atual para o final */
        troca(vetor, 0, i);

        /* chama heapfy na raíz reduzida */
        heapfy_iterativo(vetor, i, 0, &numComp);
    }
    return numComp;
}
