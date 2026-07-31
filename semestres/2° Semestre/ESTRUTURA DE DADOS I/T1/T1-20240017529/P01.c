#include <stdio.h>
//1. Faça um programa que peça o número de horas trabalhadas e o valor da hora de um 
//determinado funcionário. Em seguida, calcule o salário do funcionário. Caso o funcionário 
//tenha trabalhado mais de 200 horas, o salário final é acrescido de 5%.  

int main() {
    float horas_trabalhadas, valor_hora, salario;
    
    // Solicita os dados do user
    printf("Horas trabalhadas: ");
    scanf("%f", &horas_trabalhadas);
    
    printf("Valor da hora: ");
    scanf("%f", &valor_hora);
    
    // Calc de salário
    salario = horas_trabalhadas * valor_hora;
    
    // Acréscimo de 5% 
    if (horas_trabalhadas > 200) {
        salario *= 1.05;
    }
    
    // saida
    printf("Salario: %.2f\n", salario);
    
    return 0;
}