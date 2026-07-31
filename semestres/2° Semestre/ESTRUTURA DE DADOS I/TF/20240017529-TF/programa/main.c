#include <stdio.h>
#include "production.h"

int main() {
    ListaProducoes lista;
    initList(&lista);
    saudacao();
    int opcao;
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Incluir Producao\n");
        printf("2. Consultar Producao\n");
        printf("3. Alterar Producao\n");
        printf("4. Excluir Producao\n");
        printf("5. Listar Producao\n");
        printf("6. Relatorio Resumido\n");
        printf("7. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1: 
                incluirProducao(&lista); 
                break;
            case 2: 
                consultar(&lista); 
                break;
            case 3: 
                alterarProducao(&lista); 
                break;
            case 4: 
                excluirProducao(&lista); 
                break;
            case 5: {
                listarProducoes(&lista);
                break;
            }
            case 6:
                relatorioResumido(&lista);
                break;
            case 7:
                printf("\nPrograma Encerrado!\n");
                liberarLista(&lista);
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 7);
    return 0;
}