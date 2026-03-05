#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* estrutura de um no da arvore */
struct no {
    int chave; /* valor do no */
    struct no *pai;
    struct no *filho_d; /* filho direito */
    struct no *filho_e; /* filho esquerdo */
    int nivel_no; /* nivel do no */
};

/* estrutura de uma arvore */
struct arvore {
    struct no *raiz; /* raiz da arvore */
    int nivel_arvore; /* nivel da arvore */
};

/* funcao para adicionar um no na arvore */
struct no cria_no(int chave);

/* funcao para inicializar a arvore */
struct no *cria_arvore(struct no *n, int chave);

/* funcao para achar o menor no */
int find_min(struct no *n);

/* funcao para achar o maior no */
int find_max(struct no *n);

/* Atualiza a altura de um no */
void atualiza_altura(struct no *n);

/* funcao para verificar se a arvore esta balanceada */
/* Se a diferenca de altura entre dois irmaos for > 1 tem que balancear */
int verifica_balanceamento(struct no *n);

/* funcao para rotacionar para a esquerda */
void rotacao_esq(struct no *a, int x);

/* funcao para rotacionar para a direita */
void rotacao_dir(struct no *a, int x);

/* funcao para retirar um no na arvore */
/* Retorna o no removido */
int remove_no(struct arvore *T, struct no *z);

/* funcao para inserir um no na arvore */
void insere_no(struct arvore *T, int z);

/* funcao para imprimir a arvore */
void imprime_arvore(struct no *x);

/* funcao para destruir a arvore */
void destroi_arvore(struct no *x);

#endif /* AVL_H */