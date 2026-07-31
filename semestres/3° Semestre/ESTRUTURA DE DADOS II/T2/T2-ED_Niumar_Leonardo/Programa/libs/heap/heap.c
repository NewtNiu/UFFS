#include "../utils.h"
#include "heap.h"

void heapify(int *vetor, int tamanho, int i, long long int *contador_trocas) {
  int maior = i;
  int esq = 2 * i + 1;
  int dir = 2 * i + 2;

  if (esq < tamanho && vetor[esq] > vetor[maior])
    maior = esq;

  if (dir < tamanho && vetor[dir] > vetor[maior])
    maior = dir;

  if (maior != i) {
    swap(&vetor[i], &vetor[maior]);
    (*contador_trocas)++;
    heapify(vetor, tamanho, maior, contador_trocas);
  }
}

void heapSort(int *vetor, int tamanho, long long int *contador_trocas) {
  for (int i = tamanho / 2 - 1; i >= 0; i--) {
    heapify(vetor, tamanho, i, contador_trocas);
  }

  for (int i = tamanho - 1; i > 0; i--) {
    swap(&vetor[0], &vetor[i]);
    (*contador_trocas)++;
    heapify(vetor, i, 0, contador_trocas);
  }
}
