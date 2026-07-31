#ifndef BPTREE_H
#define BPTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    // (true/false)
#include <string.h>     // memset usada para limpar estruturas

// Define a ordem da árvore B+
// ORDER = 4 significa que cada nó pode ter até 4 filhos
#define ORDER 4

// Número máximo de chaves em um nó
#define MAX_KEYS (ORDER - 1)

// Número mínimo de chaves em um nó (após remoções)
#define MIN_KEYS ((MAX_KEYS + 1)/2)

// Estrutura que representa um nó na árvore B+
typedef struct BPTreeNode {
    int keys[MAX_KEYS + 1];                 // Vetor de chaves armazenadas no nó
    struct BPTreeNode *children[ORDER + 1]; // Ponteiros para os filhos
    int numKeys;                            // Quantas chaves o nó possui atualmente
    bool isLeaf;                            // Diz se o nó é folha ou não
    struct BPTreeNode *next;                // Ponteiro para próxima folha (lista encadeada)
} BPTreeNode;

// Estrutura principal da Árvore B+
typedef struct {
    BPTreeNode *root;  // Ponteiro para o nó raiz
} BPTree;

// Protótipos das funções disponíveis para usar na árvore
BPTree *createTree();            // Cria uma árvore vazia
void insert(BPTree *tree, int key); // Insere uma chave
void removeKey(BPTree *tree, int key); // Remove uma chave
bool search(BPTree *tree, int key); // Procura uma chave
void printTree(BPTree *tree);    // Imprime a árvore completa

#endif