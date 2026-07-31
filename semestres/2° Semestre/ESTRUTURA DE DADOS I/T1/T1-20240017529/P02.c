#include <stdio.h>
// 2. Uma loja vende produtos à vista e a prazo (pagamento 30 dias depois da compra). 
//À vista tem um desconto de 5% e a prazo um acréscimo de 10%. Faça um programa em C 
//que peça o preço do produto e a forma de pagamento: 1 para à vista e 2 para a prazo. 
//Depois apresente o preço final do produto.

int main() {
    float preco, preco_final;
    int forma_pagamento;
    
    // preço do produto
    printf("Preco do produto: ");
    scanf("%f", &preco);
    
    // forma de pagamento
    printf("Forma de pagamento (1 - a vista, 2 - a prazo): ");
    scanf("%d", &forma_pagamento);
    
    //  preço final e forma de pagamento
    if (forma_pagamento == 1) {
        preco_final = preco * 0.95; // Desconto de 5%
        printf("Preco a vista: %.2f\n", preco_final);
    } else if (forma_pagamento == 2) {
        preco_final = preco * 1.10; // Acréscimo de 10%
        printf("Preco a prazo: %.2f\n", preco_final);
    } else {
        printf("Forma de pagamento invalida.\n");
    }
    
    return 0;
}