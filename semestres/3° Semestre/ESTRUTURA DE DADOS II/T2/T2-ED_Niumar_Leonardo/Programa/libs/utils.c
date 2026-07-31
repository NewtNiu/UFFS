#include "utils.h"
#include <stdlib.h>
#include <time.h>

void inicializarAleatorio() { srand(time(NULL)); }

void gerarNumerosAleatorios(int vetor[], int tamanho) {
  for (int i = 0; i < tamanho; i++) {
    vetor[i] = rand() % 1000000;
  }
}

void copia(int *A, int *V, int size) {
  for (int i = 0; i < size; i++) {
    V[i] = A[i];
  }
}

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}
