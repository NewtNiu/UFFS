#include <stdio.h>
//5. Você vai fazer um programa para classificar os competidores segundo as suas posições. Foram criadas 5 classes: 
//Top 5 (1-5), Top 10 (6-10), Top 20 (11-20), Top 30 (21-30) e Top 100 (outras posições). 
//Faça um programa que peça a posição do competidor e indique a classe do mesmo.

int main() {
    int posicao;
    
    // posição
    printf("Posicao: ");
    scanf("%d", &posicao);
    
    // classificação
    if (posicao >= 1 && posicao <= 5) {
        printf("Top 5\n");
    } else if (posicao >= 6 && posicao <= 10) {
        printf("Top 10\n");
    } else if (posicao >= 11 && posicao <= 20) {
        printf("Top 20\n");
    } else if (posicao >= 21 && posicao <= 30) {
        printf("Top 30\n");
    } else {
        printf("Top 100\n");
    }
    
    return 0;
}
