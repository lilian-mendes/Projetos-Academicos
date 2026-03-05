#include<stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "lef.h"

/* 
 * Cria uma LEF vazia 
 * Retorna um ponteiro para a LEF ou NULL se falhar.FEITO
 */
struct lef_t *cria_lef (){
    struct lef_t *l;

    // Falha na alocação de memória
    if(!(l = malloc(sizeof(struct lef_t))))
        return NULL;

    l->primeiro = NULL;

    return l;
}
    
/* 
 * Cria um evento contendo os parâmetros informados.
 * Retorna um ponteiro para o evento ou NULL se falhar.
 FEITO*/
struct evento_t *cria_evento (int tempo, int tipo, int dado1, int dado2){
    struct evento_t *e;

    // Falha na alocação de memória
    if (!(e = malloc(sizeof(struct evento_t))))
        return NULL;

    e->tempo = tempo;
    e->tipo = tipo;
    e->dado1 = dado1;
    e->dado2 = dado2;
    
    return e;
}

/* 
 * Destroi um evento e retorna NULL.
 FEITO
 */ 
struct evento_t *destroi_evento (struct evento_t *e){
    
    if (e){
        free(e);
        return NULL;
    }

    return NULL;//retorna NULL se o evento nao existir    
}
    
/* 
 * Destroi a LEF, liberando a memória ocupada por ela, inclusive os eventos.
 * Retorna NULL.
 FEITO*/ 
struct lef_t *destroi_lef (struct lef_t *l){
    if (l){
        // Inicializa um ponteiro auxiliar para percorrer a lista
        struct nodo_lef_t *aux_evento = l->primeiro;
        while (aux_evento){
            // Armazena o próximo nó da lista para que possamos liberar o atual e avançar
            struct nodo_lef_t *aux_prox = aux_evento->prox;
            free (aux_evento);
            // Avança para o próximo nó na lista
            aux_evento = aux_prox;
        }
        // Após liberar todos os nós da lista, libera a estrutura da lista
        free(l);
    }
    return NULL;
}   
/*
 * Insere o evento apontado na LEF na ordem de tempos crescentes.
 * Eventos com o mesmo tempo devem respeitar a politica FIFO.
 * Retorna 1 em caso de sucesso ou 0 caso contrario.
*/
int insere_lef (struct lef_t *l, struct evento_t *e){
    // Verifica se a lista ou o evento são nulos
    if (!l || !e) 
        return 0;
    
    struct nodo_lef_t *novo_no = malloc(sizeof(struct nodo_lef_t));
    if(!novo_no)
        return 0;
    
    novo_no->evento = e;
    novo_no->prox = NULL;

    // Se a lista está vazia, insira o novo nodo como o primeiro elemento
    if(!l->primeiro)
        l->primeiro = novo_no;
    else{
        // Encontre a posição correta na lista com base no tempo
        struct nodo_lef_t *aux = NULL;
        struct  nodo_lef_t *aux_prox = l->primeiro;
        
        while (aux_prox && aux_prox->evento->tempo <= e->tempo){
            aux = aux_prox;
            aux_prox = aux_prox->prox;
        }
        // Se o novo evento deve ser o primeiro, atualize o primeiro no ou insira o novo nodo entre o anterior e o atual
        if (!aux){
            novo_no->prox = l->primeiro;
            l->primeiro = novo_no;        
        }else {
            aux->prox = novo_no;
            novo_no->prox = aux_prox;
        }
    }
    return 1;
}

/* 
 * Retira o primeiro evento da LEF.
 * Retorna ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *retira_lef(struct lef_t *l) {
    // Retorna NULL se a lista estiver vazia ou for nula
    if (!l || !l->primeiro) 
        return NULL;
    

    struct nodo_lef_t *aux_no = l->primeiro;
    struct evento_t *ev_retirado = aux_no->evento;

    // Atualiza o primeiro nó da lista
    l->primeiro = aux_no->prox; 

    // Libera o nó removido da lista
    free(aux_no); 

    return ev_retirado; 
}

/* 
 * Informa se a LEF está vazia.
 * Retorna 1 se estiver vazia, 0 se não estiver ou -1 se falhar.
 */
int vazia_lef(struct lef_t *l) {
    // Verifica se a lista é nula
    if (!l) 
        return -1;

    // Verifica se a lista é nula
    if (!l->primeiro) 
        return 1; 
     else 
        return 0;       
}


/* 
 * Imprime a LEF. Não faz parte do TAD, mas serve para depuração.
 * Formato de saída:
 * tempo tipo dado1 dado2
 * tempo tipo dado1 dado2
 * ...
 * total XX eventos
 */
void imprime_lef (struct lef_t *l){ 

    struct nodo_lef_t *aux = l->primeiro;
    int total_ev= 0;

    if (!l || !l->primeiro) {
        printf("LEF:\n");
        printf("Total %d eventos\n", total_ev);        
        return;
    }else{ 
    while (aux) {
        struct evento_t *ev = aux->evento;
        printf("%d %d %d %d\n", ev->tempo, ev->tipo, ev->dado1, ev->dado2);
        total_ev++;
        aux = aux->prox;
    }
    printf("LEF:\n");
    printf("Total %d eventos\n", total_ev);
    }
}

