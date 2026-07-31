#ifndef ALUNO_H
#define ALUNO_H

// Definicoes
#define TAMANHO_NOME 50
#define ARQUIVO "alunos.bin"
extern int lastId; // Declaracao

// Estrutura do Aluno
typedef struct {
    int id;
    char nome[TAMANHO_NOME];
    float media;
    int ativo; // 1 para ativo, 0 para removido
} Aluno;

// Funcoes CRUD
Aluno createAluno(); // Criar
void insertAluno(); // Inserir na lista
void updateAluno(); // Atualizar
void deleteAluno(); // Remover
void readAluno(); // Buscar(por ID)
void findAllAluno(); // Listar todos

#endif