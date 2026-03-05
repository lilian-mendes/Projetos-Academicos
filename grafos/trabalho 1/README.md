# Trabalho 2 - Manipulação de Redes Metabólicas

# Autores:
    Ayne Gabriele Figueiredo - GRR20222555
    Lilian Miranda Mendes - GRR20222549  (Autor do trabalho 1 que foi utilizado para a implementação deste trabalho)

Este trabalho é uma implementação em C no qual, o programa lê um arquivo de definição de reações (formato `.met`), constrói um grafo direcionado para representar e utiliza uma `Busca em Profundidade (DFS)` para realizar duas análises principais: detecção de ciclos e a ordenação topológica da rede metabólica.

# Modificações na Estrutura do Grafo

Os arquivos (`grafo.c` e `grafo.h`) foram originalmente implementados para grafos não direcionados, portanto foi necessário modificar alguns pontos para que pudesse ser usado para grafos direcionados. 

A principal alteração foi feita na função `adiciona_aresta` (em `grafo.c`):

 **Original (Não-Direcionado):** Ao adicionar uma aresta `(u, v)`, a aresta era inserida tanto na lista de adjacência do vértice `u` quanto na lista de adjacência do vértice `v`.
 **Modificado (Direcionado):** A aresta `(u, v)` é inserida apenas na lista de adjacência de `u` (o vértice de origem). A linha `empilha(e, v->fronteira);` foi removida para garantir que a aresta represente uma única direção (de `u` para `v`).

Além dessa modificação, na função `destroi_grafo`foi adicionado um `while`para desalocar a lista de fronteira do vértice, pois durante os testes foi identificado um vazamento de memória.

# Estruturas utilizadas 

Decidimos utilizar do trabalho 1 a lista de adjacência para representar o grafo.

## 1. Lista Encadeada (`lista.h`, `lista.c`)

 **`struct t_lista` / `struct t_no`**: Uma implementação de lista encadeada genérica (usando `void *obj` para o conteúdo) que funciona como uma pilha.
 **Onde é usada**:
     **No Grafo (`grafo.c`):** A estrutura `t_grafo` usa duas listas: uma para armazenar todos os `vertices` do grafo e outra para todas as `arestas`.
     **Nos Vértices (`grafo.c`):** Cada `t_vertice` possui uma lista chamada `fronteira`, que armazena as arestas que saem daquele vértice. Esta é a implementação da **lista de adjacência**.
     **Na Análise (`analise.c`):** A variável `resultado_topologico` é uma `lista` usada para armazenar os vértices na ordem topológica inversa à medida que a DFS os finaliza.

## 2. Estruturas do Grafo (`grafo.h`, `grafo.c`)

 **`struct t_grafo`**: A estrutura principal do grafo, com a lista de vértices e arestas.
 **`struct t_vertice`**: Representa um vértice do grafo. Contém seu `id` e sua lista de adjacência (`fronteira`).
 **`struct t_aresta`**: Representa uma aresta direcionada. Contém um `id`, um `peso`, um ponteiro para o vértice de origem (`u`) e um ponteiro para o vértice de destino (`v`).

## 3. Tabela de Mapeamento (`analise.h`)

 **`struct tabela_reacoes`**: Uma tabela de "tipos", que indica com nomes dos elementos de qual tipo eles são(reação ou metabólito).
 **Onde é usada**: É usada em `gerar_grafo_da_reacao` para garantir que cada metabólito ou reação corresponda a exatamente um vértice no grafo.

## 4. Estrutura de Processamento (`analise.h`)

 **`struct reacao_metabolica`**: Uma estrutura temporária usada exclusivamente pela função `processa_linha`.
 **Onde é usada**: Ao ler uma linha do arquivo `.met`, ela armazena temporariamente o nome da reação, a lista de substratos e a lista de produtos antes que eles sejam convertidos em vértices e arestas no grafo.

# Lógica Principal e Análise (`analise.c`)

## Estruturas de Dados Principais

 **Vetores de Controle da DFS**:
    * `cor[]`: Armazena o estado de visitação de cada vértice (`BRANCO`, `CINZA`, `PRETO`).
    * `pai[]`: Armazena o pai de cada vértice.
    * `tempo_pre[]` / `tempo_pos[]`: Armazenam os tempos de pré-ordem e pós-ordem de cada vértice.
    * `resultado_topologico`: Uma lista onde os vértices são empilhados à medida que são finalizados (cor `PRETO`).

## Funções de Construção do Grafo

Esse processo é dividido entre as funções `gerar_grafo_da_reacao` e `processa_linha`.

1.  **Leitura Linha por Linha:**
     A função `gerar_grafo_da_reacao` abre o arquivo `.met`.
     Ela usa um loop `while` com a função `fgets` para ler o arquivo, uma linha de cada vez, para dentro de um `buffer_linha` (uma string temporária).
     Após ler uma linha, o caractere de quebra de linha (`\n`) é removido. Linhas que ficam vazias após essa limpeza são ignoradas.

2.  **Interpretação da Linha (`processa_linha`):**
     Cada linha lida é passada para a função `processa_linha`.
     **Divisão Principal:** Primeiro a função procura por `"->"` usando `strstr`. Isso divide a linha em duas partes: a string dos "substratos" (que também contém o nome da reação) e a string dos "produtos".
     **Extração da Reação:** A função usa `sscanf` na parte dos "substratos" para extrair o primeiro elemento, que é o nome da reação.
     **Extração dos Elementos:** A função então entra em um loop, usando `sscanf` repetidamente para extrair os elementos seguintes (tanto substratos quanto produtos).
     **Tratamento do "+":**
         Se um elemento lido for apenas o caractere `"+"`, ele é ignorado.
         Usa `strchr` para procurar por `+` dentro de um elemento lido, permitindo que strings com mais de um elemento, seja substrato ou produto, sejam separados.
     **Armazenamento:** Todos os nomes extraídos (nome da reação, lista de substratos, lista de produtos) são armazenados em uma `struct reacao_metabolica` temporária.

3.  **Construção do Grafo:**
     Voltando para `gerar_grafo_da_reacao`, a `struct` preenchida é usada para construir o grafo.
     A função `obtem_id_elemento` é chamada para cada nome (reação, substrato, produto) para obter um ID único, criando um novo vértice no grafo se ele ainda não existir.
     As arestas são então adicionadas: `Substrato -> Reação` e `Reação -> Produto`.

Esse processo se repete para cada linha do arquivo até que `fgets` chegue ao final, e o arquivo seja fechado.

## Funções de Análise (DFS)

A lógica da busca em profundidade é diretamente baseada no algoritmo apresentado no livro **"Algoritmos: Teoria e Prática" (CLRS).** Ela utiliza o sistema de três cores (`BRANCO`, `CINZA`, `PRETO`) para identificar o estado de cada vértice.

Em comparação com o algoritmo `DFS-VISIT` padrão do livro, esta implementação foi adaptada com duas modificações principais para este trabalho:

1.  **Detecção de Ciclo com Parada Imediata:**
    * O algoritmo do livro identifica uma aresta de retorno (uma aresta para um vizinho já `CINZA`), o que indica um ciclo.
    * Esta implementação utiliza uma flag global (`tem_ciclo`) que faz com que todas as chamadas recursivas ativas parem imediatamente, através da verificação `if (tem_ciclo) return;` no início da função `dfs_recursivo`. Como o objetivo é apenas verificar se há um ciclo, ele para assim que identifica um.

2.  **Construção da Ordenação Topológica:**
    * A ordenação topológica em si é uma aplicação da DFS, conforme descrito no livro.
    * Esta implementação integra essa funcionalidade diretamente: quando todos os vizinhos de um vértice foram visitados e ele está prestes a ser marcado como `PRETO`, ele é empilhado na lista `resultado_topologico`.
    * A linha `empilha((obj)u, resultado_topologico);` garante que os vértices sejam armazenados na ordem inversa de seu tempo de pós-ordem, o que gera a ordenação topológica correta.

As funções que implementam isso são:

 **`dfs`**: A função principal que inicializa os vetores de controle (cores, pais, tempos), a lista `resultado_topologico`, e chama `dfs_recursivo` para cada vértice `BRANCO`.
 **`dfs_recursivo`**: A implementação recursiva que visita os vértices, marca as cores, detecta ciclos (verificando vizinhos `CINZA`) e empilha os resultados para a ordenação topológica.

# Estrutura da Função `main`

1.  **Verificação de Argumentos:** O programa primeiro verifica se o usuário forneceu o argumento de linha de comando necessário (o nome do arquivo `.met`). Se `argc` for menor que 2, ele exibe uma mensagem de erro e encerra.
2.  **Inicialização:**
    * Um novo grafo vazio é criado.
    * A tabela de mapeamento é inicializada.
3.  **Construção do Grafo:**
    * A função `gerar_grafo_da_reacao` é chamada, passando o nome do arquivo (`argv[1]`), o grafo `G` e a `tabela`. Esta função é responsável por ler o arquivo, processar todas as linhas e preencher o grafo e a tabela.
    * O programa imprime a representação do grafo (lista de adjacência) e a tabela de mapeamento (ID ---> Nome).
4.  **Análise (DFS):** A função `dfs(G)` é chamada para ser executada.
5.  **Apresentação dos Resultados:**
    * O programa verifica a flag `tem_ciclo`. Se for verdadeira, informa ao usuário: "Rede cíclica encontrada".
    * Caso contrário, ele percorre a lista `resultado_topologico` e imprime o nome de cada vértice que é do tipo `METABOLITO`, exibindo assim a ordenação topológica.
6.  **Limpeza:** Por fim, o programa libera toda a memória alocada.

# Limitações do Algoritmo

1.  **Tamanhos Definidos:**
    * Foi utilizado `DEFINE` para indicar os tamanhos possíveis das estruturas, isso limita o algoritmo a apenas alguns casos.
2.  **Leitura dos Arquivos:**
    * A forma como é lida cada linha, só funcionaria para arquivos que estão formatados com um mesmo padrão, ignorando outros tipos de arquivos.
3.  **Limitações do Algoritmo DFS:**
    * O algoritmo da DFS implementado para no primeiro ciclo encontrado, portanto não seria possível usá-lo para uma análise de quantos ciclos uma rede metabólica possui. Além disso, ele apenas informa que existe um ciclo, mas não imprime quais metabólitos e reações formam o ciclo.