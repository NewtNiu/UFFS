// Bibliotecas Externas
#include <stdio.h>
#include <stdlib.h>
// Bibliotecas Internas
#include "implements.h"
#include "alunos.h"

// Funcoes Auxiliares
// Limpar o terminal de deixar mais organizado(limpa com base no OS do pc)
void clearTerminal() {
    #ifdef _WIN32
        system("cls");   // Windows (32 e 64 bits)
    #elif __linux__
        system("clear"); // Linux
    #elif __APPLE__
        system("clear"); // MacOS
    #else
        // Caso nao seja detectado imprime varias quebras de linha
        for (int i = 0; i < 50; i++) printf("\n");
    #endif
}

// Inicia o arquivo(Mesmo que todas as funcoes ja façam isso e para garantir que seja criado).
void initArquivo() {
    FILE *file = fopen(ARQUIVO, "ab"); 
    if (file == NULL) {
        perror("Erro ao inicializar o arquivo");
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

// Verifica o Id do ultimo elemento adicionado(o codigo adiciona o ID automaticamente quando um novo Aluno e criado, isso e para saber de onde paramos).
void atualizarLastId() {
    FILE *file = fopen(ARQUIVO, "rb");
    if (!file) {
        // Se o arquivo nao existe ou esta vazio, mantem lastId = 5
        printf("Arquivo nao encontrado.\n");
        return;
    }

    fseek(file, 0, SEEK_END); // Vai para o final do arquivo
    long tamanho = ftell(file);

    if (tamanho < sizeof(Aluno)) {
        // Arquivo vazio ou menor que um registro
        printf("Arquivo vazio.\n");
        fclose(file);
        return;
    }

    // Calcula posicaoo do ultimo registro
    fseek(file, -sizeof(Aluno), SEEK_END);

    Aluno aluno;
    fread(&aluno, sizeof(Aluno), 1, file);
    lastId = aluno.id; // Atualiza a variavel

    fclose(file);
    // printf("lastId atualizado para %d\n", lastId); // Caso haja algo no codigo, usado para testar
}

// Le o conteudo de dentro do arquivo, se estiver vazio preenche os 5 alunos, se possuir so ignora(garantir registros para a manipulação)
void insert5Alunos() {
    FILE *file = fopen(ARQUIVO, "rb"); // Tenta abrir para leitura
    int precisaInserir = 1;

    if (file != NULL) {
        // Verifica se o tamanho do arquivo é múltiplo de sizeof(Aluno)
        fseek(file, 0, SEEK_END);
        long tamanho = ftell(file);
        fclose(file);

        if (tamanho >= sizeof(Aluno) && (tamanho % sizeof(Aluno) == 0)) {
            printf("Arquivo ja existe e contem registros validos. Nenhum aluno inicial sera adicionado.\n");
            precisaInserir = 0;
        } else {
            printf("Arquivo existente corrompido ou incompleto. Sera preenchido com alunos iniciais.\n");
        }
    }

    if (precisaInserir) {
        file = fopen(ARQUIVO, "wb"); // Cria/abre para escrita
        if (file == NULL) {
            perror("Erro ao criar o arquivo para alunos iniciais");
            exit(EXIT_FAILURE);
        }

        Aluno alunos[5] = { // Preenche
            {1, "Malenia",     8.5, 1},
            {2, "Placidusax", 7.0, 1},
            {3, "Malekith",    9.5, 1},
            {4, "Ranni",       6.8, 1},
            {5, "Radahn",      7.9, 0} // Naturalmente desativado
        };

        fwrite(alunos, sizeof(Aluno), 5, file);
        fclose(file);
        printf("Arquivo preenchido com 5 alunos iniciais!\n");
    }
}