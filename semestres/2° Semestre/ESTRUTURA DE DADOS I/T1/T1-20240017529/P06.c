#include <stdio.h>
// 6. Sabendo que as faixas etárias para votação são: 18 a 70 anos voto obrigatório;
//16, 17 e acima de 70 anos voto facultativo; outras idades (ou seja, menor que 16 anos)
//não pode votar, crie um programa que processa essa verificação.

int main() {
    int idade;
    
    // idade do usuário
    printf("Digite a sua Idade: ");
    scanf("%d", &idade);
    
    // Verificador
    if (idade >= 18 && idade <= 70) {
        printf("Seu voto e Obrigatorio\n");
    } else if ((idade == 16 || idade == 17) || idade > 70) {
        printf("seu voto e Facultativo\n");
    } else {
        printf("Nao pode votar\n");
    }
    
    return 0;
}