#include "bubble.h"
#include "../utils.h"

void bubbleSort(int *vetor, int tamanho, long long int *contador_trocas) {
  for (int i = 0; i < tamanho - 1; i++) {
    for (int j = 0; j < tamanho - i - 1; j++) {
      if (vetor[j] > vetor[j + 1]) {
        swap(&vetor[j], &vetor[j + 1]);
        (*contador_trocas)++;
      }
    }
  }
}
