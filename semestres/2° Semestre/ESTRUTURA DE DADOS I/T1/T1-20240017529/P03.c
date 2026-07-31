#include <stdio.h>
//3. Faça um programa que calcule a duração de um evento qualquer. 
//Para tal, o programa pede a hora de início e hora de fim (sem os minutos), 
//as horas serão informadas de 0 a 23. Perceba que um evento pode começar em um 
//dia e acabar em outro. Os eventos nunca terão duração de mais de 24 horas.

int main() {
    int inicio, fim, duracao;
    
    // hora de início e fim do evento
    printf("Digite o orario de inicio do evento: ");
    scanf("%d", &inicio);
    
    printf("Digite o horario de Encerramento: ");
    scanf("%d", &fim);
    
    // Calcula a duração
    if (fim >= inicio) {
        duracao = fim - inicio;
    } else {
        duracao = (24 - inicio) + fim;
    }
    
    // duração
    printf("O evento teve a duracao de: %d horas\n", duracao);
    
    return 0;
}