#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "libs/bubble/bubble.h"
#include "libs/heap/heap.h"
#include "libs/insertion/insertion.h"
#include "libs/quick/quicksort.h"
#include "libs/utils.h"

int main() {

  inicializarAleatorio();

  printf("Gerando vetores, por favor aguarde...\n");
  int v100[100];
  gerarNumerosAleatorios(v100, 100);

  int v1000[1000];
  gerarNumerosAleatorios(v1000, 1000);

  int v5000[5000];
  gerarNumerosAleatorios(v5000, 5000);

  int v10000[10000];
  gerarNumerosAleatorios(v10000, 10000);

  int *v50000 = malloc(50000 * sizeof(int));
  gerarNumerosAleatorios(v50000, 50000);

  int *v100000 = malloc(100000 * sizeof(int));
  gerarNumerosAleatorios(v100000, 100000);
  printf("Todos os vetores foram gerados com sucesso!\n\n");

  int opcao_vetor, opcao_algoritmo;
  do {
    printf("Escolha o TAMANHO do vetor para o teste:\n\n");
    printf("1 - 100 elementos\n");
    printf("2 - 1.000 elementos\n");
    printf("3 - 5.000 elementos\n");
    printf("4 - 10.000 elementos\n");
    printf("5 - 50.000 elementos\n");
    printf("6 - 100.000 elementos\n");
    printf("0 - Sair do programa\n\n");
    printf("Opção de Vetor: ");
    scanf("%d", &opcao_vetor);
    printf("\n");

    if (opcao_vetor == 0) {
      break;
    }

    int *vetor_original = NULL;
    int tamanho_selecionado = 0;

    switch (opcao_vetor) {
    case 1:
      vetor_original = v100;
      tamanho_selecionado = 100;
      break;
    case 2:
      vetor_original = v1000;
      tamanho_selecionado = 1000;
      break;
    case 3:
      vetor_original = v5000;
      tamanho_selecionado = 5000;
      break;
    case 4:
      vetor_original = v10000;
      tamanho_selecionado = 10000;
      break;
    case 5:
      vetor_original = v50000;
      tamanho_selecionado = 50000;
      break;
    case 6:
      vetor_original = v100000;
      tamanho_selecionado = 100000;
      break;
    default:
      printf("Opção de vetor inválida!\n");
      continue;
    }

    printf("Agora, escolha um método de Ordenação para o vetor de %d "
           "elementos:\n",
           tamanho_selecionado);
    printf("\n1 - Bubble Sort\n2 - Insertion Sort\n3 - Heap Sort\n4 - "
           "Quick Sort\n\n");
    printf("Opção de Algoritmo: ");
    scanf("%d", &opcao_algoritmo);

    int *vetor_copia = (int *)malloc(tamanho_selecionado * sizeof(int));
    copia(vetor_original, vetor_copia, tamanho_selecionado);

    long long int contador_trocas = 0;
    clock_t inicio, fim;
    double tempo_gasto;
    char nome_algoritmo[20];

    inicio = clock();

    switch (opcao_algoritmo) {
    case 1:
      strcpy(nome_algoritmo, "Bubble Sort");
      bubbleSort(vetor_copia, tamanho_selecionado, &contador_trocas);
      break;
    case 2:
      strcpy(nome_algoritmo, "Insertion Sort");
      insertionSort(vetor_copia, tamanho_selecionado, &contador_trocas);
      break;
    case 3:
      strcpy(nome_algoritmo, "Heap Sort");
      heapSort(vetor_copia, tamanho_selecionado, &contador_trocas);
      break;
    case 4:
      strcpy(nome_algoritmo, "Quick Sort");
      quickSort(vetor_copia, 0, tamanho_selecionado - 1, &contador_trocas);
      break;
    default:
      printf("Opção de algoritmo inválida!\n");
      free(vetor_copia);
      continue;
    }

    fim = clock();
    tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\n----------- RESULTADO DO TESTE -----------\n");
    printf("Tamanho do Vetor: %d\n", tamanho_selecionado);
    printf("Algoritmo:        %s\n", nome_algoritmo);
    printf("Tempo de Execução:  %.6f segundos\n", tempo_gasto);
    printf("Trocas/Operações: %lld\n", contador_trocas);
    printf("------------------------------------------\n");

    free(vetor_copia);

  } while (opcao_vetor != 0);

  printf("\nEncerrando o programa e liberando memória...\n");
  free(v50000);
  free(v100000);

  return 0;
}