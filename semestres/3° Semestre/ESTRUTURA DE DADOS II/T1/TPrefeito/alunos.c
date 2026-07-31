// Bibliotecas Externas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
// Bibliotecas Internas
#include "alunos.h"
#include "implements.h"

// Definicoes
int lastId = 5; // Ultimo ID usado(controle automatic de IDs)

// Funcoes CRUD
// Cria uma var do tipo Aluno
Aluno createAluno() {
    Aluno novoAluno;
    printf("ID do novo Aluno: %d", ++lastId);
    novoAluno.id = lastId;
    printf("\nDigite o nome do aluno: ");
    scanf(" %[^\n]", novoAluno.nome);
    getchar();
    printf("Digite a media do aluno: ");
    scanf("%f", &novoAluno.media);
    getchar();
    novoAluno.ativo = 1; // Ativo por padrao
    return novoAluno;
}

// Isere a variavel tipo Aluno no final do documento
void insertAluno() {
    clearTerminal();
    FILE *file = fopen(ARQUIVO, "ab");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo para inserir aluno");
        return;
    }

    Aluno novoAluno = createAluno(); // Chama a funcação que ira criar um novo registro, apos armazena e adiciona ao final do arquivo
    fwrite(&novoAluno, sizeof(Aluno), 1, file); // origem, tamanho, quantidade, destino
    fclose(file);

    printf("Aluno inserido com sucesso!\n");
}

void updateAluno() {
    clearTerminal();
    int id;
    printf("Digite o ID do aluno a ser atualizado: ");
    scanf("%d", &id);
    getchar();

    FILE *file = fopen(ARQUIVO, "rb+");
    if (!file) {
        perror("Erro ao abrir o arquivo para atualizar aluno");
        return;
    }

    Aluno aluno;
    int found = 0;

    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        if (aluno.id == id) {
            found = 1;
            if (aluno.ativo) {
                // Atualiza normalmente
                long pos = ftell(file) - sizeof(Aluno);

                printf("Atualizando dados do aluno ID %d\n", id);
                printf("Nome atual: %s\n", aluno.nome);
                printf("Deseja alterar o nome? (1: sim, 0: nao): ");
                int op;
                scanf("%d", &op);
                getchar();

                if (op == 1) {
                    printf("Digite o novo nome: ");
                    scanf(" %[^\n]", aluno.nome);
                    getchar();

                    fseek(file, pos + offsetof(Aluno, nome), SEEK_SET);
                    fwrite(&aluno.nome, sizeof(aluno.nome), 1, file);
                }

                printf("Media atual: %.2f\n", aluno.media);
                printf("Deseja alterar a média? (1: sim, 0: nao): ");
                scanf("%d", &op);
                getchar();

                if (op == 1) {
                    printf("Digite a nova media: ");
                    scanf("%f", &aluno.media);
                    getchar();

                    fseek(file, pos + offsetof(Aluno, media), SEEK_SET);
                    fwrite(&aluno.media, sizeof(aluno.media), 1, file);
                }

                printf("Aluno atualizado com sucesso!\n");

            } else {
                int opcao;
                printf("O aluno com ID %d esta inativo. Deseja ativa-lo novamente? (1: sim, 0: nao):\n", id);
                scanf("%d", &opcao);
                getchar();

                if (opcao == 1) {
                    // Reativar
                    aluno.ativo = 1;
                    long pos = ftell(file) - sizeof(Aluno); 
                    printf("Atualizando dados do aluno ID %d\n", id);
                    printf("Nome atual: %s\n", aluno.nome);
                    printf("Deseja alterar o nome? (1: sim, 0: nao): ");
                    int op;
                    scanf("%d", &op);
                    getchar();

                    if (op == 1) {
                        printf("Digite o novo nome: ");
                        scanf(" %[^\n]", aluno.nome);
                        getchar();

                        fseek(file, pos + offsetof(Aluno, nome), SEEK_SET);
                        fwrite(&aluno.nome, sizeof(aluno.nome), 1, file);
                    }

                    printf("Media atual: %.2f\n", aluno.media);
                    printf("Deseja alterar a média? (1: sim, 0: nao): ");
                    scanf("%d", &op);
                    getchar();

                    if (op == 1) {
                        printf("Digite a nova media: ");
                        scanf("%f", &aluno.media);
                        getchar();

                        fseek(file, pos + offsetof(Aluno, media), SEEK_SET);
                        fwrite(&aluno.media, sizeof(aluno.media), 1, file);
                    }

                    // Ativar
                    fseek(file, pos + offsetof(Aluno, ativo), SEEK_SET);
                    fwrite(&aluno.ativo, sizeof(aluno.ativo), 1, file);

                    printf("Aluno atualizado com sucesso!\n");

                } else {
                    printf("Operacao de atualizacao cancelada.\n");
                }
            }

            break; // Encontrou o aluno, sai do loop
        }
    }

    if (!found) {
        printf("Aluno com ID %d nao encontrado.\n", id);
    }

    fclose(file);
}


// Exclui logicamente do arquivo(ativo = 0)
void deleteAluno() {
    clearTerminal();
    int id;
    printf("Digite o ID do aluno a ser removido: ");
    scanf("%d", &id);
    getchar();

    FILE *file = fopen(ARQUIVO, "rb+");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo para remover aluno");
        return;
    }

    Aluno aluno;
    int found = 0;
    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        if (aluno.id == id && aluno.ativo) { // 
            found = 1;
            aluno.ativo = 0; // Marca como inativo
            fseek(file, -sizeof(Aluno), SEEK_CUR); // Move o ponteiro de volta para reescrever
            fseek(file, offsetof(Aluno, ativo), SEEK_CUR); // se move ate o campo ativo, usa a quantidade de bytes pra se mover e evita o problema de alinhamento gambiarra mas funfa
            fwrite(&aluno.ativo, sizeof(aluno.ativo), 1, file); // Escreve no local da variavel o novo valor, so muda a var ativo e nao o bloco inteiro.


            printf("Aluno com ID %d removido com sucesso!\n", id);
            break;
        }
        if (aluno.id == id && aluno.ativo == 0) { // 
            found = 1;
            printf("O aluno com ID %d ja esta inativo.\n", id);
            break;
        }
    }

    if (!found) {
        printf("Aluno com ID %d nao encontrado ou ja esta inativo.\n", id);
    }

    fclose(file);
}

// Verifica 1 a 1 ate achar e imprime.
void readAluno() {
    clearTerminal();
    int id;
    printf("Digite o ID do aluno a ser buscado: ");
    scanf("%d", &id);
    getchar();

    FILE *file = fopen(ARQUIVO, "rb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo para buscar aluno");
        return;
    }

    Aluno aluno;
    int found = 0;
    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        if (aluno.id == id && aluno.ativo) { // 
            found = 1;
            printf("Aluno encontrado:\n");
            // printf("ID\tNome\t\tMedia\n");
            printf("ID: %d\t", aluno.id);
            printf("Nome: %s\t", aluno.nome);
            printf("Media: %.2f\n", aluno.media);
            break;
        }
        if (aluno.id == id && aluno.ativo == 0) { // 
            found = 1;
            printf("O aluno com ID %d esta inativo.\n", id);
            break;
        }
    }

    if (!found) {
        printf("Aluno com ID %d nao encontrado ou esta inativo.\n", id);
    }

    fclose(file);
}

// Imprime todos os alunos ativos
void findAllAluno() {
    clearTerminal();
    FILE *file = fopen(ARQUIVO, "rb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo para listar alunos");
        return;
    }

    Aluno aluno;
    int count = 0;
    printf("Lista de Alunos Ativos:\n");
    printf("ID\tNome\t\tMedia\n");
    printf("-------------------------------------\n");
    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        if (aluno.ativo) { // 
            printf("%d\t%s\t\t%.2f\n", aluno.id, aluno.nome, aluno.media);
            count++;
        }
    }

    if (count == 0) {
        printf("Nenhum aluno ativo encontrado.\n");
    }
    
    fclose(file);
}