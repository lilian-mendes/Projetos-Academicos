#ifndef CUCKOO_H
#define CUCKOO_H

#define TAM_MAX 11

struct tabela_hash {
  int chave;
  int estado;  /* 0 = vazia, 1 = ocupada, 2 = removida */
};

/* funcao de hash para a tabela 1 */
int h1(int chave);

/* funcao de hash para a tabela 2 */
int h2(int chave);

/* funcao de busca em ambas as tabelas */
int busca(struct tabela_hash *T1, struct tabela_hash *T2, int chave);

/* funcao para ordenar as chaves */
void ordena_chaves(struct tabela_hash *temp, int aux);

/* verifica se uma chave ja foi incluida na tabela 1*/
int chave_existe_T1(struct tabela_hash *T1, int chave);

/* verifica se uma chave ja foi incluida na tabela 2*/
int chave_existe_T2(struct tabela_hash *T2, int chave);

/* funcao para inserir na tabela 1 ou na tabela 2 */
void insere(struct tabela_hash *T1, struct tabela_hash *T2, int chave);

/* funcao para remover uma chave de uma das tabelas */
void remover(struct tabela_hash *T1, struct tabela_hash *T2, int chave);

/* funcao para imprimir as tabelas de forma ordenada */
void imprime(struct tabela_hash *T1, struct tabela_hash *T2);

#endif // CUCKOO_H