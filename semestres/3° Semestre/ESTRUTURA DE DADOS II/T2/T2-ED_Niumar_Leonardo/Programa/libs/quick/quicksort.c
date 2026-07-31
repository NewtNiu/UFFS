#include "../utils.h"
#include "quicksort.h"

int partition(int *vetor, int esquerda, int direita,
  long long int *contador_trocas) {
  int pivo = vetor[direita];
  int i = esquerda - 1;

  for (int j = esquerda; j < direita; j++) {
    if (vetor[j] <= pivo) {
      i++;
      swap(&vetor[i], &vetor[j]);
      (*contador_trocas)++;
    }
  }
  swap(&vetor[i + 1], &vetor[direita]);
  (*contador_trocas)++;
  return i + 1;
}

void quickSort(int *vetor, int esquerda, int direita,
  long long int *contador_trocas) {
  if (esquerda < direita) {
    int pi = partition(vetor, esquerda, direita, contador_trocas);
    quickSort(vetor, esquerda, pi - 1, contador_trocas);
    quickSort(vetor, pi + 1, direita, contador_trocas);
  }
}
