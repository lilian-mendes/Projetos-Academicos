#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <myavl.h>


struct no *cria_no(int chave) {
    struct no *n = malloc(sizeof(struct no));
    if (n == NULL) {
        printf("Alocação falhou\n");
        return NULL;
    }
    n->chave = chave;
    n->filho_e = NULL;
    n->filho_d = NULL;
    n->pai = NULL;
    n->nivel_no = 0; 
    return n; 
}

/* funcao para inicializar a arvore */
struct no *cria_arvore(struct arvore *T, int chave) {
    if (T == NULL) {
        return cria_no(chave);
    }

    if (T->raiz == NULL) {
        T->raiz = cria_no(chave);
        return T->raiz;
    }

    struct no *atual = T->raiz;
    struct no *pai = NULL;

    while (atual != NULL) {
        pai = atual;
        if (chave < atual->chave) 
            atual = atual->filho_e;
        else if (chave > atual->chave) 
            atual = atual->filho_d;
        else 
            return T->raiz;
    }

    struct no *novo_nodo = cria_no(chave);
    novo_nodo->pai = pai;

    if (chave < pai->chave) 
        pai->filho_e = novo_nodo;
    
    else
        pai->filho_d = novo_nodo;

    /* define o nivel do novo nodo */
    novo_nodo->nivel_no = pai->nivel_no + 1;

    return T->raiz;
}

/* funcao para achar o menor no */
int find_min(struct arvore *T) {
    if (T == NULL || T->raiz == NULL)
        return -1;

    struct no *atual = T->raiz;

    while (atual->filho_e != NULL)
        atual = atual->filho_e;  

    return atual->chave;
}

/* funcao para achar o maior no */
int find_max(struct arvore *T) {
    if (T == NULL || T->raiz == NULL) 
        return -1;
    
    struct no *atual = T->raiz;

    while (atual->filho_d != NULL)
        atual = atual->filho_d;  

    return atual->chave;
}

/* Atualiza a altura de um no */
void atualiza_altura(struct no *n) {
    if (n == NULL)
        return;

    if (n->filho_e->nivel_no > n->filho_d->nivel_no) 
        n->altura = altura_e + 1;
    else
        n->altura = altura_d + 1;
}

/* Funcao para verificar se a arvore esta balanceada*/
/* Se a diferenca de altura entre dois irmaos for > 1 tem que balancear */
int verifica_balanceamento(struct no *n) {
    if (n == NULL)
        return 0;
    return (n->filho_e->nivel_no - n->filho_d->nivel_no);
}

/* funcao para rotacionar para a esquerda */
void rotacao_esq(struct arvore *T, struct no *x) {
    if (x == NULL || x->filho_d == NULL) {
        return; 
    }

    struct no *y = x->filho_d;
    x->filho_d = y->filho_e;

    if (y->filho_e != NULL) 
        y->filho_e->pai = x;

    y->pai = x->pai;

    if (x->pai == NULL)
        a->raiz = y; 
    
    else if (x == x->pai->filho_e)
        x->pai->filho_e = y;
    
    else 
        x->pai->filho_d = y;

    y->filho_e = x;
    x->pai = y;
}

/* funcao para rotacionar para a direita */
void rotacao_dir(struct arvore *T, struct no *x) {
    if (x == NULL || x->filho_e == NULL) {
        return;
    }

    struct no *y = x->filho_e;
    x->filho_e = y->filho_d;

    if (y->filho_d != NULL) 
        y->filho_d->pai = x;
    
    y->pai = x->pai;

    if (x->pai == NULL) 
        a->raiz = y; 
     
    else if (x == x->pai->filho_d) 
        x->pai->filho_d = y;
    
    else 
        x->pai->filho_e = y;

    y->filho_d = x;
    x->pai = y;
}

/* funcao para remover um no da arvore */
int remove_nodo(struct arvore *T, struct nodo *z) {
    struct nodo *y; 

    if (z == NULL) 
        return NULL;

    /*caso em que z tem apenas um filho */
    if (z->filho_e == NULL || z->filho_d == NULL)
        y = z;
    else
        y = acha_sucessor(z);

    /* filho de y */
    struct no *x;

    /* quando y tem um filho a esquerda */
    if (y->filho_e != NULL)
        x = y->filho_e;

    /* quando y tem um filho a direita */
    else
        x = y->filho_d;

    /* muda o ponteiro do pai de x para o pai de y */
    if (x != NULL)
        x->pai = y->pai;

    /* quando y eh a raiz da arvore */
    if (y->pai == NULL) 
        T->raiz = x; 

    /* quando y nao eh a raiz */
    /* quando y eh filho a esquerda */
    else if (y == y->pai->filho_e)
        y->pai->filho_e = x;

    /* quando y eh filho a direita */
    else 
        y->pai->filho_d = x;

    if (y != z)
        z->chave = y->chave;

    /* retorna o nodo retirado */
    return y;
}

/* funcao para inserir um no na arvore */
void insere_nodo(struct arvore *T, int z) {
    /* aloca o novo nodo */
    struct no *novo = cria_no(z);
    if (novo == NULL) 
        return; 

    struct no *y = NULL;      
    struct no *x = T->raiz;   

    /* percorre a arvore para encontrar a posicao correta para z */
    while (x != NULL) {
        y = x;
        if (novo->chave < x->chave) 
            x = x->filho_e; 
        else
            x = x->filho_d; 
    }

    /* armazena o pai de z */
    novo->pai = y;

    /* insere z na arvore */
    if (y == NULL) 
         /* se y eh NULL, z se torna a raiz */
        T->raiz = novo;
    else if (novo->chave < y->chave)
        /* z eh o filho esquerdo de y */
        y->filho_e = novo;
    else
        /* z eh o filho direito de y */
        y->filho_d = novo; 
}

/* funcao para imprimir a arvore */
void imprime_arvore(struct no *x) {
    if (x == NULL) 
        return;
    imprime_arvore(x->filho_e);
    printf("%d ", x->chave);
    imprime_arvore(x->filho_d);
}

/* funcao para desalocar as memorias alocadas para os nos */
void destroi_arvore(struct no *x) {
    if (x == NULL)
        return;
    
    destroi_arvore(x->filho_e);  
    destroi_arvore(x->filho_d);

    free(x);
}