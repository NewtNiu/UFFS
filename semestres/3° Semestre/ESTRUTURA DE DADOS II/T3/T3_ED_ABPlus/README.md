README.md

<div align="center">

## Árvore B+ - Estruturas de Dados II – 2025

</div> <br>

**Tema**: Arvore B+  
**Professor**: Guilherme Dal Bianco  
**Data de Entrega**: 28/11/2025  
**Autor**: Niumar Girardi 

## Descrição

### 🔹 Objetivo:

Este trabalho tem como objetivo compreender o funcionamento de uma árvore B+, amplamente utilizada em sistemas de banco de dados e sistemas de arquivos para indexação e busca eficiente de informações.

### 🔹  O que é uma Árvore B+?

A **B+ Tree** é uma árvore de busca balanceada amplamente usada em:
* Sistemas de arquivos
* Bancos de dados
* Estruturas para indexação de grandes volumes de dados

Características importantes:
* Todas as chaves *buscáveis* estão **somente nas folhas**
* Folhas são encadeadas, permitindo varredura sequencial rápida
* Suporta grandes números de chaves por nó, mantendo profundidade baixa
* Ótima para operações de disco e armazenamento persistente


### 🔹 Projeto

Este repositório contém uma implementação completa de uma Árvore B+ de Ordem 4 em linguagem C, incluindo todas as operações fundamentais:

* **Inserção**
* **Busca**
* **Remoção**
* **Split de folhas e nós internos**
* **Merge e Borrow (empréstimo) após remoção**
* **Impressão da árvore e das folhas encadeadas**

O programa acompanha um menu interativo em `main.c`, que permite testar a estrutura de maneira prática.


```c
int opc;                     // Variável para guardar o que o usuário escolheu

    do {
        mostrarMenu();
        if (scanf("%d", &opc) != 1) { while(getchar()!='\n'); break; }

        switch (opc) {

        case 1:
            inserirValores(tree);  // Chama função de inserção
            break;

        case 2:
            buscarValor(tree);     // Chama função de busca
            break;

        case 3:
            removerValor(tree);    // Chama função de remoção
            break;

        case 4:
            printTree(tree);       // Imprime a árvore inteira
            break;

        case 0:
            printf("\nEncerrando...\n");
            break;

        default:
            printf("\nOpcao invalida!\n");
        }

    } while (opc != 0);
```
*funções de buscas definicas acima do `int main()`*

**Terminal:**
```
1 - Inserir valores
2 - Buscar valor
3 - Remover valor
4 - Ver arvore
0 - Sair
```
Em “*Inserir valores*”, você pode:
* Inserir automaticamente valores padrões
* Inserir manualmente

**Exemplo de Impressão da Árvore:**
```c
Nivel 0: [12]
Nivel 1: [5 7] 
Nivel 1: [12 20 23]

Folhas:
[5 7] -> [12 17 20] -> [23 29 30] -> NULL
```


## Estrutura do Projeto

### Estrutura da pastas:

```
├── main.c  
├── bptree.h    
└── bptree.c
```

### 🔹 Arquivos

`Main.c`

Arquivo responsável pela interface com o usuário.   
Ele contém:

* Funções de menu (`mostrarMenu`)
* Inserção manual e inserção automática de valores
* Busca de chave
* Remoção de chave
* Visualização completa da árvore (`printTree`)
* Loop principal da aplicação

<br>

`bptree.h`

Contém:
* Definições da ordem (ORDER = 4)
* Limites máximos e mínimos de chaves
* Estrutura do nó da B+ Tree
* Estrutura da árvore
* Declarações das funções públicas:
    * `createTree()` 
    * `insert()`
    * `removeKey()`
    * `search()`
    * `printTree()`

<br>

`bptree.c`

Implementa toda a estrutura da Árvore B+, incluindo:

* Criação da árvore
* Navegação até folhas
* Inserção ordenada em folhas
* Divisão (split) de folhas e nós internos
* Promoção de chaves para níveis superiores
* Remoção e correções pós-remoção (borrow e merge)
* Impressão hierárquica + lista encadeada de folhas 

Este arquivo contém toda a lógica essencial da Árvore B+.

---

### Funcionamento da Árvore B+ (Ordem 4) (`/btree.h`)

**🔹 Definições:**
```c
#define ORDER 4

#define MAX_KEYS (ORDER - 1)

#define MIN_KEYS ((MAX_KEYS + 1)/2)
```

Ordem = 4 significa:
* Cada nó pode ter **até 3 chaves**
* Cada nó pode ter **até 4 filhos**
* Cada nó deve ter **no mínimo 2 chaves** (*exceto a raiz*)

<br>

🔹 **Características desta implementação**

**Nó:**

```c
typedef struct BPTreeNode {
    int keys[MAX_KEYS + 1]; // Vetor de chaves armazenadas no nó
    struct BPTreeNode *children[ORDER + 1]; // Ponteiros para os filhos
    int numKeys; // Quantas chaves o nó possui atualmente                   
    bool isLeaf; // Diz se o nó é folha ou não                    
    struct BPTreeNode *next; // Ponteiro para próxima folha (lista encadeada)          
} BPTreeNode;
```

* As folhas são **encadeadas** (via ponteiro `next`)
* As chaves internas são apenas **separadores**
* As folhas armazenam todas as chaves
* Inserções e remoções mantém a árvore balanceada automaticamente

<br>

**Estrutura da Arvore:**
```c
typedef struct {
    BPTreeNode *root;  // Ponteiro para o nó raiz
} BPTree;
```
<br>

**🔹 Funções**
```c
BPTree *createTree();            // Cria uma árvore vazia
void insert(BPTree *tree, int key); // Insere uma chave
void removeKey(BPTree *tree, int key); // Remove uma chave
bool search(BPTree *tree, int key); // Procura uma chave
void printTree(BPTree *tree);    // Imprime a árvore completa
```

## Descrição das Funções Principais

### 🔹 FUNÇÕES PÚBLICAS (`/btree.c`)

`BPTree *createTree()`

Cria uma árvore B+ vazia.
* Cria um nó raiz inicialmente como folha
* Inicializa contadores e ponteiros

<br>

`bool search(BPTree *tree, int key)`

Realiza a busca de uma chave:
* 1. Navega até a folha correta (`findLeaf`)
* 2. Procura linearmente dentro das chaves da folha
* 3. Retorna **true** ou **false**

<br>

`void insert(BPTree *tree, int key)`

Fluxo da inserção:
1. Encontra a folha correta (`findLeaf`)
2. Insere mantendo a ordenação (`insertIntoLeafSorted`)
3. Se a folha passar do limite, ocorre:
    * `splitLeaf()`
    * `insertIntoParent()` (*promoção de chave*)

Essa função mantém a árvore sempre balanceada.

<br>

`void removeKey(BPTree *tree, int key)`

Fluxo da remoção:
1. Localiza a folha onde a chave deveria estar
2. Remove a chave (`removeFromLeaf`)
3. Atualiza chaves dos pais se necessário
4. Verifica se o nó ficou abaixo do mínimo
5. Executa:
    * **Borrow** (emprestar chave do irmão esquerdo/direito)
    * **Merge** (fundir folhas)
    * Ajustes recursivos no nível superior (`fixAfterDeletion`)

O algoritmo garante invariantes da B+ Tree durante toda a operação.

<br>

`void printTree(BPTree *tree)`

Exibe:

1. A árvore completa (pré-ordem)
2. Todas as folhas encadeadas:

```c
[5 7 10] -> [12 17 20] -> [23 29 30] -> NULL
```
Ideal para debug e visualização da estrutura.

<br>

### 🔹 FUNÇÕES INTERNAS (`/btree.c`)


<br>

`createNode(bool isLeaf)`

Cria um nó (folha ou interno).

<br>

`findLeaf(...)`

Desce pela árvore até localizar a folha correta onde a chave deveria estar.

<br>

`insertIntoLeafSorted(...)`

Insere uma chave dentro da folha de forma ordenada, deslocando elementos quando necessário.

<br>

`splitLeaf(...)`

Quando uma folha ultrapassa a capacidade máxima:
* Divide a folha em duas
* Move metade das chaves para a nova folha
* Ajusta encadeamento de folhas
* Promove a menor chave da nova folha para o nível superior

<br>

`insertIntoParent(...)`

Insere a chave promovida no nó pai.     
Se ele ficar cheio, também divide o nó interno.

Este processo pode criar uma nova raiz (crescimento da árvore).

<br>

`removeFromLeaf(...)`

Remove uma chave deslocando os elementos restantes para manter ordem.

<br>

---

**🔹  Empréstimo (Borrow)**

* `borrowFromLeftLeaf(...)`
* `borrowFromRightLeaf(...)`
* `borrowFromLeftInternal(...)`
* `borrowFromRightInternal(...)`

Essas funções evitam merges desnecessários.

<br>

---

**🔹 Merge**

* `mergeLeaves(...)`
* `mergeInternalChildren(...)`

Fundem dois nós quando ambos ficam abaixo do mínimo.

<br>

---

`fixAfterDeletion(...)`

Função que garante:
* Rebalanceamento após remoção
* Empréstimo quando possível
* Merge quando necessário
* Ajustes até a raiz

É o "coração" da remoção.


## Como Compilar e Executar

**🔹  Requisitos:**

* Compilador GCC instalado (padrão em sistemas Linux e disponível no MinGW para Windows).
* Todos os arquivos do projeto devem estar organizados na estrutura indicada anteriormente.

**🔹  Compilar:**  
No terminal, acesse o diretório onde o projeto está salvo e execute o comando:

```c
gcc main.c bptree.c -o bptree
```

**🔹  Executar:**  
Após a compilação bem-sucedida, execute o programa com:

```c
./bptree
```

## Licença e Autor

Este projeto foi desenvolvido como parte do Trabalho Prático 3 (TP3) da disciplina de Programação e Organização de Dados / Estrutura de Dados II (POD / EDII) — Universidade Federal da Fronteira Sul (UFFS), sob orientação do professor Guilherme Dal Bianco.

O código-fonte pode ser utilizado livremente para fins educacionais, estudo e comparação de algoritmos de ordenação, desde que mantidos os devidos créditos aos autores:

### 🔹  Autor:
**Niumar Girardi**  
GitHub: https://github.com/NewtNiu

**Universidade Federal da Fronteira Sul (UFFS)**  
Curso de Ciência da Computação - Campus Chapecó  
Trabalho Prático 3 - POD / EDII - 2025

© 2025 — Todos os direitos reservados aos autores para uso acadêmico.