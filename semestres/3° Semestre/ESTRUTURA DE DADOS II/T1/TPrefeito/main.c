// Bibliotecas Externas
#include <stdio.h>
// Bibliotecas Internas
#include "alunos.h"
#include "implements.h"

int main() {
    // Preparativos iniciais do sistema
    initArquivo(); // Inicializa o arquivo de alunos ativos(Usado no sistema so para ja criar o arquivo, pois em teoria a funcao insert5Alunos tambem ja cria)

    printf("=================================================================\n");
    printf("\n\tSistema de Registros de Alunos em Arquivo Binario\n");
    printf("\n=================================================================\n");
    atualizarLastId(); // Atualiza o lastId ao iniciar o programa
    insert5Alunos(); // Insere 5 alunos iniciais no arquivo de alunos ativos

    int opcao;
    do {
        printf("\nEscolha uma das Opcoes:");
        printf("\n\t1) Inserir Aluno");
        printf("\n\t2) Atualizar Aluno");
        printf("\n\t3) Remover Aluno");
        printf("\n\t4) Buscar Aluno");
        printf("\n\t5) Listar Alunos");
        printf("\n\t6) Sair");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            // CRUD
            case 1:
                insertAluno();
                break;
            case 2:
                updateAluno();
                break;
            case 3:
                deleteAluno();
                break;
            case 4:
                readAluno();
                break;
            case 5:
                findAllAluno();
                break;
            case 6:
                clearTerminal();
                printf("Saindo do sistema...\n");
                printf("Programa Encerrado!\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 6);

    return 0;
}