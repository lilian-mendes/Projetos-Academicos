#include <stdio.h>

#include "cuckoo.h"

int main() {
     struct tabela_hash T1[TAM_MAX], T2[TAM_MAX];
     char tipo;
     int chave;

     /* inicializa ambas as tabelas vazias */
     for (int i = 0; i < TAM_MAX; i++) {
          T1[i].estado = 0;
          T2[i].estado = 0;
     }

     /* le a entrada e insere ou remove a chave */
     while (scanf(" %c %d", &tipo, &chave) == 2) {
          if (tipo == 'i')
               insere(T1, T2, chave);
          else if (tipo == 'r')
               remover(T1, T2, chave);
          else {
               printf("Comando inválido\n");
               return -1;
          }
     }

     /* imprime as tabelas finais */
     imprime(T1, T2);
     
     return 0;
}