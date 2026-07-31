#include <stdio.h>
//4. Faça um programa que peça 4 números inteiros. Em seguida, apresente quantos números 
//informados são negativos e quantos são positivos (considere o 0 como positivo).

int main() {
    int n1, n2, n3, n4;
    int positivos = 0, negativos = 0;
    
    // numeros 
    printf("Digitee um numero inteiro  para N1: ");
    scanf("%d", &n1);
    printf("Digitee um numero inteiro  para N2: ");
    scanf("%d", &n2);
    printf("Digitee um numero inteiro  para N3: ");
    scanf("%d", &n3);
    printf("Digitee um numero inteiro  para N4: ");
    scanf("%d", &n4);
    
    // positivos e negativos
    if (n1 >= 0) positivos++; else negativos++;
    if (n2 >= 0) positivos++; else negativos++;
    if (n3 >= 0) positivos++; else negativos++;
    if (n4 >= 0) positivos++; else negativos++;
    
    // resultado
    printf("%d (+)Numeros positivos e %d (-)Numeros negativos\n", positivos, negativos);
    
    return 0;
}