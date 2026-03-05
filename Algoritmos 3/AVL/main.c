#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <myavl.h>



int main()


    /* imprime a arvore */
    if (T == NULL || T->raiz == NULL){
            printf("Árvore vazia\n");
            return;
        }

        imprime_arvore(A->raiz);

    /* desaloca as estruturas */
    if (T == NULL)
        return;

    /* destroi a estrutura a partir da raiz */
    destroi_arvore(T->raiz);
    free(T);