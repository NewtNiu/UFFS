#include "../utils.h"
#include "insertion.h"

void insertionSort(int *vetor, int tamanho, long long int *contador_trocas) {
  for (int i = 1; i < tamanho; i++) {
    int chave = vetor[i];
    int j = i - 1;

    while (j >= 0 && vetor[j] > chave) {
      vetor[j + 1] = vetor[j];
      (*contador_trocas)++;
      j--;
    }
    vetor[j + 1] = chave;
  }
}
