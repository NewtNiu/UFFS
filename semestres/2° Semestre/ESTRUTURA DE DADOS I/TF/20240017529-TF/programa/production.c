#include <stdio.h> // entrada e saidas
#include <stdlib.h> // alocacao dinamica
#include <string.h> // manipula strings
#include <ctype.h> // manipula strings
#include "production.h" //funcoes da producao(add, excluir, consultar, alterar, listar)

// Inicia o sentinela/lista
void initList(ListaProducoes *l) {
    l->first = NULL;
    l->last = NULL;
    l->calcID = 0;
}

// Verifica se a lista esta vazia
int isEmpty(ListaProducoes *l) {
    return l->first == NULL; // 1s ou 0n
}

double calcAreaMinima(int diametro, int quantidade) {
    int max_por_coluna = 3; // Altura
    // Arredonda pra cima 
    int colunas = (quantidade + max_por_coluna - 1) / max_por_coluna;
    // Converte de cm pra m
    double d_metros = diametro / 100.0;
    // Area de chao ocupada por cada coluna 
    double area_total = colunas * (d_metros * d_metros); // Tratando como quadrado pois fica mais facil de calcular
    return area_total;
}

void incluirProducao(ListaProducoes *l) {
    Producao p;
    p.codigo = l->calcID + 1;

    printf("\n--- Criando Producao | codigo: %d\n", p.codigo);
    printf("Data (DD MM AAAA): ");
    scanf("%d %d %d", &p.dataProducao.dia, &p.dataProducao.mes, &p.dataProducao.ano);
    if (p.dataProducao.dia < 0 || p.dataProducao.mes < 0 || p.dataProducao.ano < 0) return;

    printf("Cultivar: \n");
    int escolhaCultivar;
    printf("\t1. Tifton 85\n\t2. Florakirk\n\t3. Jiggs\n\t4. Coastcross\n\t5. Outros\nEscolha: ");
    scanf("%d", &escolhaCultivar);
    switch (escolhaCultivar) {
        case 1:
            strcpy(p.tipoDeFardo.cultivar, "Tifton 85");
            break;
        case 2:    
            strcpy(p.tipoDeFardo.cultivar, "Florakirk");
            break;
        case 3:
            strcpy(p.tipoDeFardo.cultivar, "Jiggs");
            break;
        case 4:
            strcpy(p.tipoDeFardo.cultivar, "Coastcross");
            break;
        case 5:
            printf("Digite o cultivar: ");
            scanf("%s[^\n]", p.tipoDeFardo.cultivar);
            break;
        default:
            printf("Opcao invalida. Usando cultivar padrao: Tifton 85.\n");
            break;
    }

    printf("Tipo de Feno (A, B ou C): ");
    scanf(" %c", &p.tipoDeFardo.tipoDeFeno);
    if (p.tipoDeFardo.tipoDeFeno == '-') return;
    p.tipoDeFardo.tipoDeFeno = toupper(p.tipoDeFardo.tipoDeFeno);

    printf("Diametro (cm): ");
    scanf("%d", &p.tipoDeFardo.diametro);
    if (p.tipoDeFardo.diametro < 0) return;

    printf("Quantidade fardos: ");
    scanf("%d", &p.qtDeFardos);
    if (p.qtDeFardos < 0) return;

    printf("Tempo (min): ");
    scanf("%d", &p.tempoEmMin);
    if (p.tempoEmMin < 0) return;

    p.areaMinima = calcAreaMinima(p.tipoDeFardo.diametro, p.qtDeFardos); 

    No* novo = malloc(sizeof(No));
    novo->dado = p;
    novo->next = NULL;

    if (isEmpty(l)) {
        l->first = novo;
        l->last = novo;
    } else {
        l->last->next = novo;
        l->last = novo;
    }

    l->calcID++;
    printf("Producao %d adicionada.\n", p.codigo);
}

void consultar(ListaProducoes *l) {
    int opcao;
    printf("\n--- Consultar por:\n| 1. ID | 2. Data | 3. Cultivar | 4. Tipo de feno |\nEscolha: ");
    scanf("%d", &opcao);
    
    int achou = 0; // Ira ajudar para ver se encontramos o dado
    switch (opcao) {

        // Busca linear por ID/ como e uma lista simples, a busca binaria nao e possivel, teria que tornar ela uma lista duplamente encadeada mas mesmo assim seria uma gambiara, e de qualquer forma se nao sabemos o indice ia ter que percorrer ate o meio dela...
        case 1: {
            int id;
            printf("Digite o ID: ");
            scanf("%d", &id);
            printf("\n");
            for (No* aux = l->first; aux != NULL; aux = aux->next) { // percorre ate o final e ve se o id passado e igual o aux->dado.codigo(lista dado var codigo = id passado)
                if (aux->dado.codigo == id) {
                    printf("Codigo: %d | Data: %02d/%02d/%04d | %s - %c - %dcm | Fardos: %d | Tempo: %d min | Area min: %.2f m2\n",
                        aux->dado.codigo,
                        aux->dado.dataProducao.dia,
                        aux->dado.dataProducao.mes,
                        aux->dado.dataProducao.ano,
                        aux->dado.tipoDeFardo.cultivar,
                        aux->dado.tipoDeFardo.tipoDeFeno,
                        aux->dado.tipoDeFardo.diametro,
                        aux->dado.qtDeFardos,
                        aux->dado.tempoEmMin,
                        aux->dado.areaMinima);
                    achou = 1;
                    break;
                }
            }
            if (!achou)
                printf("Producao nao encontrada.\n");
            break;
        }

        // Busca linear por data
        case 2: {
            int dia, mes, ano;
            printf("Digite a data (dd mm aaaa): ");
            scanf("%d %d %d", &dia, &mes, &ano);
            printf("\n");
            for (No* aux = l->first; aux != NULL; aux = aux->next) {
                if (aux->dado.dataProducao.dia == dia &&
                    aux->dado.dataProducao.mes == mes &&
                    aux->dado.dataProducao.ano == ano) {
                    printf("Codigo: %d | Data: %02d/%02d/%04d | %s - %c - %dcm | Fardos: %d | Tempo: %d min | Area min: %.2f m2\n",
                        aux->dado.codigo,
                        aux->dado.dataProducao.dia,
                        aux->dado.dataProducao.mes,
                        aux->dado.dataProducao.ano,
                        aux->dado.tipoDeFardo.cultivar,
                        aux->dado.tipoDeFardo.tipoDeFeno,
                        aux->dado.tipoDeFardo.diametro,
                        aux->dado.qtDeFardos,
                        aux->dado.tempoEmMin,
                        aux->dado.areaMinima);
                    achou = 1;
                }
            }
            if (!achou)
                printf("Nenhuma producao encontrada nessa data.\n");
            break;
        }

        // Busca linear por cultivar
        case 3: {
            char cultivar[20];
            printf("Escolha o cultivar: ");
            printf("\n\t1. Tifton 85\n\t2. Florakirk\n\t3. Jiggs\n\t4. Coastcross\n\t5. Outros\nEscolha: ");
            int escolhaCultivar;
            scanf("%d", &escolhaCultivar);
            printf("\n");
            switch (escolhaCultivar) {
                case 1:
                    strcpy(cultivar, "Tifton 85");
                    break;
                case 2:    
                    strcpy(cultivar, "Florakirk");
                    break;
                case 3:
                    strcpy(cultivar, "Jiggs");
                    break;
                case 4:
                    strcpy(cultivar, "Coastcross");
                    break;
                case 5:
                    printf("Digite o cultivar: ");
                    scanf("%s[^\n]", cultivar);
                    break;
                default:
                    printf("Opcao invalida.\n");
                    break;       
            }

            for (No* aux = l->first; aux != NULL; aux = aux->next) { 
                if (strcmp(aux->dado.tipoDeFardo.cultivar, cultivar) == 0) { // 0 se = e 1 se !=
                    printf("Codigo: %d | Data: %02d/%02d/%04d | %s - %c - %dcm | Fardos: %d | Tempo: %d min | Area min: %.2f m2\n",
                        aux->dado.codigo,
                        aux->dado.dataProducao.dia,
                        aux->dado.dataProducao.mes,
                        aux->dado.dataProducao.ano,
                        aux->dado.tipoDeFardo.cultivar,
                        aux->dado.tipoDeFardo.tipoDeFeno,
                        aux->dado.tipoDeFardo.diametro,
                        aux->dado.qtDeFardos,
                        aux->dado.tempoEmMin,
                        aux->dado.areaMinima);
                    achou = 1;
                }
            }
            if (!achou)
                printf("Nenhuma producao encontrada para a cultivar '%s'.\n", cultivar);
            break;
        }

        // Busca linear por tipo de feno
        case 4: {
            char tipo;
            printf("Digite o tipo de feno (A, B ou C): ");
            scanf(" %c", &tipo);
            tipo = toupper(tipo); // como os tipos que usamos na entrada sao maisculos, na busca tambem deve ser.
            printf("\n");
            for (No* aux = l->first; aux != NULL; aux = aux->next) {
                if (aux->dado.tipoDeFardo.tipoDeFeno == tipo) {
                    printf("Codigo: %d | Data: %02d/%02d/%04d | %s - %c - %dcm | Fardos: %d | Tempo: %d min | Area min: %.2f m²\n",
                        aux->dado.codigo,
                        aux->dado.dataProducao.dia,
                        aux->dado.dataProducao.mes,
                        aux->dado.dataProducao.ano,
                        aux->dado.tipoDeFardo.cultivar,
                        aux->dado.tipoDeFardo.tipoDeFeno,
                        aux->dado.tipoDeFardo.diametro,
                        aux->dado.qtDeFardos,
                        aux->dado.tempoEmMin,
                        aux->dado.areaMinima);
                    achou = 1;
                }
            }
            if (!achou)
                printf("Nenhuma producao encontrada para o tipo '%c'.\n", tipo);
            break;
        }

        default:
            printf("Opcao invalida.\n");
            break;
    }
}

void alterarProducao(ListaProducoes *l) {
    int id;
    printf("Digite o codigo da Producao a alterar: ");
    scanf("%d", &id);
    char opt;
    printf("\n");
    // busca linear por id
    for (No* aux = l->first; aux != NULL; aux = aux->next) {
        if (aux->dado.codigo == id) { // se achar, mostrar as op para mudar

            printf("\n--- Alterando Producao | codigo: %d\n", aux->dado.codigo);
            printf("Alterar data (%02d/%02d/%04d)? (S/N): ", aux->dado.dataProducao.dia, aux->dado.dataProducao.mes, aux->dado.dataProducao.ano);
            scanf(" %c", &opt);
            opt = toupper(opt); // em todos ele converte pra maiculo pro useer nao ter que se preocupar com isso
            if (opt == 'S') { // se sim, muda
                printf("Nova data (DD MM AAAA): ");
                scanf("%d %d %d", &aux->dado.dataProducao.dia, &aux->dado.dataProducao.mes, &aux->dado.dataProducao.ano);
            }

            printf("Alterar cultivar (%s)? (S/N): ", aux->dado.tipoDeFardo.cultivar);
            scanf(" %c", &opt);
            opt = toupper(opt);
            if (opt == 'S') {
                printf("Escolha o cultivar: \n");
                printf("\t1. Tifton 85\n\t2. Florakirk\n\t3. Jiggs\n\t4. Coastcross\n\t5. Outros\nEscolha: ");
                int escolhaCultivar;
                scanf("%d", &escolhaCultivar);
                switch (escolhaCultivar) {
                    case 1:
                        strcpy(aux->dado.tipoDeFardo.cultivar, "Tifton 85");
                        break;
                    case 2:    
                        strcpy(aux->dado.tipoDeFardo.cultivar, "Florakirk");
                        break;
                    case 3:
                        strcpy(aux->dado.tipoDeFardo.cultivar, "Jiggs");
                        break;
                    case 4:
                        strcpy(aux->dado.tipoDeFardo.cultivar, "Coastcross");
                        break;
                    case 5:
                        printf("Digite o cultivar: ");
                        scanf("%s[^\n]", aux->dado.tipoDeFardo.cultivar);
                        break;
                    default:
                        printf("Opcao invalida.\n");
                        break; 
                }
            }

            printf("Alterar tipo de feno (%c)? (S/N): ", aux->dado.tipoDeFardo.tipoDeFeno);
            scanf(" %c", &opt);
            opt = toupper(opt);
            if (opt == 'S') {
                printf("Novo tipo de feno (A, B ou C): ");
                scanf(" %c", &aux->dado.tipoDeFardo.tipoDeFeno);
                aux->dado.tipoDeFardo.tipoDeFeno = toupper(aux->dado.tipoDeFardo.tipoDeFeno);
            }

            printf("Alterar diametro (%d cm)? (S/N): ", aux->dado.tipoDeFardo.diametro);
            scanf(" %c", &opt);
            opt = toupper(opt);
            if (opt == 'S') {
                printf("Novo diametro (cm): ");
                scanf("%d", &aux->dado.tipoDeFardo.diametro);
                aux->dado.areaMinima = calcAreaMinima(aux->dado.tipoDeFardo.diametro, aux->dado.qtDeFardos);
            }

            printf("Alterar quantidade de fardos (%d)? (S/N): ", aux->dado.qtDeFardos);
            scanf(" %c", &opt);
            opt = toupper(opt);
            if (opt == 'S'){
                printf("Nova quantidade de fardos: ");
                scanf("%d", &aux->dado.qtDeFardos);
            }

            printf("Alterar tempo (min) (%d)? (S/N): ", aux->dado.tempoEmMin);
            scanf(" %c", &opt);
            opt = toupper(opt);
            if (opt == 'S') {
                printf("Novo tempo (min): ");
                scanf("%d", &aux->dado.tempoEmMin);
            }          
            printf("Producao %d atualizada com sucesso!\n", id);
            return;
        }
    }
    printf("Producao com codigo %d nao encontrada.\n", id);
}

void excluirProducao(ListaProducoes *l) {
    if (isEmpty(l)) { // esta vazia?
        printf("Lista vazia.\n"); 
        return;
    }

    int codigo;
    printf("\nDigite o codigo da producao a excluir: ");
    scanf("%d", &codigo);

    No *anterior = NULL;
    for (No *atual = l->first; atual != NULL; anterior = atual, atual = atual->next) {
        if (atual->dado.codigo == codigo) {
            // Se for o primeiro DA lista
            if (anterior == NULL) {
                l->first = atual->next;
            } else {
                anterior->next = atual->next;
            }

            // Se for o ultimo da lista
            if (atual == l->last) {
                l->last = anterior;
            }

            free(atual);
            printf("\nProducao com codigo %d excluida com sucesso.\n", codigo);
            return;
        }
    }
}

void listarProducoes(ListaProducoes *l) {
    printf("\n--- Lista Completa de Producoes:\n");
    if (isEmpty(l)) {
        printf("Lista vazia.\n");
        return;
    }
    // Percore toda a lista e printa 1 por 1
    for (No* aux = l->first; aux != NULL; aux = aux->next) {
        printf("Codigo: %d | Data: %02d/%02d/%04d | %s - %c - %dcm | Fardos: %d | Tempo: %d min | Area min: %.2f m2\n",
                                aux->dado.codigo,
                                aux->dado.dataProducao.dia,
                                aux->dado.dataProducao.mes,
                                aux->dado.dataProducao.ano,
                                aux->dado.tipoDeFardo.cultivar,
                                aux->dado.tipoDeFardo.tipoDeFeno,
                                aux->dado.tipoDeFardo.diametro,
                                aux->dado.qtDeFardos,
                                aux->dado.tempoEmMin,
                                aux->dado.areaMinima);
    }
}

void relatorioResumido(ListaProducoes *l) {
    if (isEmpty(l)) {
        printf("\nLista vazia. Nenhuma producao cadastrada.\n");
        return;
    }
    
    // contadores
    int totalProducoes = 0;
    int totalFardos = 0;
    int totalTempo = 0;
    double totalArea = 0.0;
    // Contadores por cultivar
    int countTifton = 0, countFlorakirk = 0, countJiggs = 0, countCoastcross = 0, countOutros = 0;
    // Contadores por tipo de feno
    int countTipoA = 0, countTipoB = 0, countTipoC = 0;

    // Percorre a lista e conta
    for (No* aux = l->first; aux != NULL; aux = aux->next) {
        totalProducoes++;
        totalFardos += aux->dado.qtDeFardos;
        totalTempo += aux->dado.tempoEmMin;
        totalArea += aux->dado.areaMinima;

        // Cultivar
        if (strcmp(aux->dado.tipoDeFardo.cultivar, "Tifton 85") == 0)
            countTifton += aux->dado.qtDeFardos;
        else if (strcmp(aux->dado.tipoDeFardo.cultivar, "Florakirk") == 0)
            countFlorakirk += aux->dado.qtDeFardos;
        else if (strcmp(aux->dado.tipoDeFardo.cultivar, "Jiggs") == 0)
            countJiggs += aux->dado.qtDeFardos;
        else if (strcmp(aux->dado.tipoDeFardo.cultivar, "Coastcross") == 0)
            countCoastcross += aux->dado.qtDeFardos;
        else
            countOutros += aux->dado.qtDeFardos;

        // Tipos de feno
        if (aux->dado.tipoDeFardo.tipoDeFeno == 'A')
            countTipoA += aux->dado.qtDeFardos;
        else if (aux->dado.tipoDeFardo.tipoDeFeno == 'B')
            countTipoB += aux->dado.qtDeFardos;
        else if (aux->dado.tipoDeFardo.tipoDeFeno == 'C')
            countTipoC += aux->dado.qtDeFardos;
    }

    // Area e tempo
    double mediaFardos = (double)totalFardos / totalProducoes;
    double mediaTempo = (double)totalTempo / totalProducoes;

    // Descobre cultivar mais produzido
    char cultivarMaisProduzido[20] = "Nenhum";
    int maior = countTifton;

    strcpy(cultivarMaisProduzido, "Tifton 85");
    if (countFlorakirk > maior) { maior = countFlorakirk; strcpy(cultivarMaisProduzido, "Florakirk"); }
    if (countJiggs > maior) { maior = countJiggs; strcpy(cultivarMaisProduzido, "Jiggs"); }
    if (countCoastcross > maior) { maior = countCoastcross; strcpy(cultivarMaisProduzido, "Coastcross"); }
    if (countOutros > maior) { maior = countOutros; strcpy(cultivarMaisProduzido, "Outros"); }

    printf("\n--- Relatorio Resumido:\n\n");
    printf("Total de producoes registradas: %d\n", totalProducoes);
    printf("Total geral de fardos: %d\n", totalFardos);
    printf("Media de fardos por producao: %.2f\n", mediaFardos);
    printf("Media de tempo gasto por producao: %.2f min\n", mediaTempo);
    printf("Tempo total gasto: %d min\n", totalTempo);
    printf("Cultivar mais produzido: %s (%d fardos)\n", cultivarMaisProduzido, maior);
    printf("Area total minima de armazenagem: %.2f m2\n", totalArea);
    printf("\nEstatisticas por tipo de feno:\n");
    printf("Tipo A: %d fardos\n", countTipoA);
    printf("Tipo B: %d fardos\n", countTipoB);
    printf("Tipo C: %d fardos\n", countTipoC);
    printf(" \n");
}

void liberarLista(ListaProducoes *l) { // limpa a lista apos o uso do programa(opcao 6)
    No* aux = l->first;
    while (aux) {
        No* tmp = aux;
        aux = aux->next;
        free(tmp);
    }
    l->first = l->last = NULL;
    l->calcID = 0;
}

// Personaliza a saudacao do programa
void saudacao() {
    printf("--------------------------------------------------------------------------");
    printf("      |                                                                 \n");
    printf("    \\  | \\/   |                                                        \n");
    printf("   \\| |    |  / /     ______________________________________________     \n");
    printf("      \\ \\ |  /        |                                            |    \n");
    printf("       '\\\\//'         |                  AGROTEC                   |    \n");
    printf("          ||          |         Sistema de Gerenciamento           |    \n");
    printf("          ||          |                 Agricola                   |    \n");
    printf("          ||          |____________________________________________|    \n");
    printf("          ||                        ______                            \n");
    printf("     |\\  ||  |\\                    ||_||_\\`.____                     \n");
    printf("     | | || //                     (   _    _ _  \\|                   \n");
    printf("     \\ \\||/ /                    =-(_)====--(_)-'-                    \n");
    printf("      `\\\\//`                                                          \n");
    printf("--------------------------------------------------------------------------");
    printf(" ");
    printf("\n\tBem-vindo ao Sistema de Gerenciamento de Producoes\n\n");
    printf("Este sistema permite gerenciar producoes de fardos de feno.\n");
    printf("Voce pode incluir, consultar, alterar, excluir e listar producoes.\n");
}

void secreto() {
    printf("\n");
    printf("--------------------------------------------------------------------------\n");
    printf("Universidade Federal da Fronteira Sul - UFFS\n");
    printf("Curso: Ciencia da Computacao\n");
    printf("Disciplina: Estrutura de Dados\n");
    printf("Professor: Denio Duarte\n");
    printf("\nTrabalho Final: Gerenciamento de Producoes de Fardos de Feno\n");   
    
    printf("\nDesenvolvido por: \nAluno: Niumar Girardi\n");
    printf("--------------------------------------------------------------------------\n");
}