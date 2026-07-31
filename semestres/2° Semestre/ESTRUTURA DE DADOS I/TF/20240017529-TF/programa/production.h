#ifndef PRODUCTION_H
#define PRODUCTION_H

// Structs pre definidas para o sistema de gerenciamento de producoes de fardos de feno
typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    char cultivar[20];
    char tipoDeFeno;
    int diametro;
} Fardo;

typedef struct {
    int codigo;
    Data dataProducao;
    Fardo tipoDeFardo;
    int qtDeFardos;
    int tempoEmMin;
    double areaMinima;  // NOVO CAMPO
} Producao;

// Lista simples
typedef struct No { 
    Producao dado;
    struct No* next;
} No;

// Sent
typedef struct {
    No* first;
    No* last;
    int calcID; // obs: nao e utilizado para ver o tamanho mas para saber qual foi o ultimo id adicioonado.
} ListaProducoes;

// Funcoes para manipular a lista de producoes
void initList(ListaProducoes*);
int isEmpty(ListaProducoes*);
void incluirProducao(ListaProducoes*);
void consultar(ListaProducoes*);
void alterarProducao(ListaProducoes*);
void excluirProducao(ListaProducoes*);
void listarProducoes(ListaProducoes*);
void relatorioResumido(ListaProducoes*);
double calcAreaMinima(int diametro, int quantidade);
void liberarLista(ListaProducoes*);

void saudacao();
void secreto();
#endif