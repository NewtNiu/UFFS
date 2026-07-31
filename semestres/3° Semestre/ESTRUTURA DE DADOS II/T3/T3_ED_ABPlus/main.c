#include <stdio.h>
#include <stdlib.h>
#include "bptree.h"

// Função que mostra o menu
void mostrarMenu() {
    printf("\n====== MENU B+ TREE ======\n");
    printf("1 - Inserir valores\n");     
    printf("2 - Buscar valor\n");        
    printf("3 - Remover valor\n");       
    printf("4 - Ver arvore\n");          
    printf("0 - Sair\n");                
    printf("Escolha: ");                 
}

// Função que cuida da inserção de valores na árvore
void inserirValores(BPTree *tree) {

    int opc; 

    // Mostra opções de inserção
    printf("\n1 - Inserir valores padrao\n");
    printf("2 - Inserir valor manualmente\n");
    printf("Escolha: ");

    if (scanf("%d", &opc) != 1) { while(getchar()!='\n'); return; }

    // Caso o usuário escolha inserir valores automáticos
    if (opc == 1) {
        // Lista de valores pré-definidos
        int valores[] = {10, 20, 5, 6, 12, 30, 7, 17, 29, 23, 12, 111, 10, 23};
        int n = sizeof(valores) / sizeof(int); // Calcula quantos valores têm no vetor

        printf("\nInserindo valores padrao:\n");
        // Percorre o vetor e insere cada valor na árvore
        for (int i = 0; i < n; i++) {
            printf("%d ", valores[i]); // Mostra o valor inserido
            insert(tree, valores[i]);  // Chama a função de inserção
        }
        printf("\nValores inseridos com sucesso!\n");

    // Caso o usuário escolha inserir um valor manualmente
    } else if (opc == 2) {
        int v;
        printf("\nDigite o valor para inserir: ");
        if (scanf("%d", &v) != 1) { while(getchar()!='\n'); return; }

        insert(tree, v);  // Insere o valor único
        printf("\nValor %d inserido!\n", v);

    } else {
        printf("Opcao invalida\n");   // Caso o usuário digite um número errado
    }
}

// Função que busca um valor na árvore
void buscarValor(BPTree *tree) {
    int v;
    printf("\nDigite o valor para buscar: ");
    if (scanf("%d", &v) != 1) { while(getchar()!='\n'); return; }

    // Chama a função search() que retorna verdadeiro ou falso
    if (search(tree, v))
        printf("\nValor %d encontrado na arvore!\n", v);
    else
        printf("\nValor %d NAO encontrado!\n", v);
}

// Função que remove um valor da árvore
void removerValor(BPTree *tree) {
    int v;
    printf("\nDigite o valor para remover: ");
    if (scanf("%d", &v) != 1) { while(getchar()!='\n'); return; }

    removeKey(tree, v); // Remove o valor da árvore

    printf("\nValor %d removido!\n", v);
}

int main() {

    BPTree *tree = createTree(); // Cria a árvore B+ vazia
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

    return 0;
}