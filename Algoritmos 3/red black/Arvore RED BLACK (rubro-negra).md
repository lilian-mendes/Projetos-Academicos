
# Implementação da Árvore Vermelho-Preta (Red-Black Tree) em C

## Descrição

A **Árvore Vermelho-Preta (Red-Black Tree)** é uma árvore binária de busca autobalanceada, que garante uma altura máxima de O(log n), mesmo no pior caso. Ela é amplamente utilizada em sistemas onde o desempenho nas operações de busca, inserção e remoção deve ser garantido, independentemente da ordem das operações. A árvore é implementada de acordo com o livro:

**Algoritmos (3ª edição)**  
Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, Clifford Stein  
Tradução de Arlete Simille Marques  
Elsevier, 2012

### Características Principais:

- Cada nó é vermelho ou preto.
- A raiz é sempre preta.
- Nenhum caminho de um nó até um filho nulo possui duas vezes mais nós vermelhos do que qualquer outro caminho.
- Inserção e remoção mantêm as propriedades da árvore por meio de rotações e alterações de cores.

## Operações Suportadas

A implementação oferece as principais operações para manipulação da árvore:

- **Busca:** Pesquisa de um valor na árvore.
- **Inserção:** Adição de um novo nó, mantendo as propriedades da árvore.
- **Remoção:** Exclusão de um nó, mantendo o equilíbrio da árvore.
- **Rotações:** Rotações à esquerda e à direita para manter o balanceamento.
- **Exibição:** Imprime a estrutura da árvore de forma legível.

## Complexidade das Operações

- **Busca, Inserção e Remoção:** O(log n), onde `n` é o número de elementos na árvore.
