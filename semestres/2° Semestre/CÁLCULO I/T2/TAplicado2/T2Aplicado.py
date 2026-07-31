# Universidade Federal da Fronteira Sul – UFFS
# Curso: Ciência da Computação (2ª fase)
# Disciplina: Cálculo 1
# Professor: Milton Kist
#
# Trabalho Aplicado 2
#
# O presente trabalho, de implementação, deve ser feito preferencialmente em dupla, caso 
# não seja possível em dupla, ele pode ser feito individualmente. O trabalho precisa estar 
# identificado por todos os integrantes. Apenas um integrante da dupla precisa postar o 
# trabalho no Moodle.
#
# Abaixo seguem os problemas a serem entregues:
#
# Problema: Dada uma função f:[a,b] → (0,∞), definido por y=f(x), contínua.
# Determine uma aproximação para a área da região plana limitada pelas retas x=a, x=b,
# y=0 e a curva y=f(x).
#
# Dados de entrada: a, b; y=f(x) e número n de subintervalos (podem ser todos de mesmo 
# tamanho) em que [a,b] pode ser dividido.
#
# Dados de saída: Uma aproximação da região plana.
#
# Observação 1: Caso não seja possível considerar uma função qualquer como dado de 
# entrada, considerar a seguinte função fixa: f(x) = e^(-x) + 2.
#
# Observação 2: Uma alternativa ao número n de subintervalos do intervalo [a,b], pode 
# ser feito considerando número de intervalos fixos. Neste caso deve ter pré-definido 
# para: n=4, n=10 e n=50.
#
# Nota: Os programas (linhas de comando) dos dois problemas deverão ser entregues em 
# arquivo pdf. Além disso devem ser disponibilizados os programas de cada problema.

# para usar o e(exponeencial)
import math  

# f(x)
def f(x):
    return math.exp(-x) + 2 # f(x) = e^(-x) + 2

# f(x) que calc a area com a integral
def calcular_area(a, b, n):
    h = (b - a) / n # subintervalos
    area_total = 0 # inicia a area total em zero

    i = 0
    while i < n:
        x1 = a + i * h # inicio do subintervalo
        x2 = a + (i + 1) * h # Fim do subintervalo
        # calcula a area do trrapezioo formado entre x1 e x2
        area_trapezio = (h / 2) * (f(x1) + f(x2))
        # adiciona essa area calculada a arrea final
        area_total += area_trapezio
        i += 1  # passa para o prox intervalo
    return area_total

# main
def main():
    # cabecario
    print("===== Cálculo da área aproximada =====\n\nCurva: f(x) = e^(-x) + 2\n")
    # entrada
    print("Digite o Valor do Intervalo [a, B]")
    a = float(input("Digite o valor de A: "))
    b = float(input("Digite o valor de B: "))
    print()
    print("Escolha o número de subintervalos:\n\t1) 4\n\t2) 10\n\t3) 50\n\t4) Outro Valor\n")

    escolha = input("Opcao (1 a 4): ")
    # switch de escolhas
    if escolha == "1":
        n = 4
    elif escolha == "2":
        n = 10
    elif escolha == "3":
        n = 50
    elif escolha == "4":
        n = int(input("Valor de n: "))
    else:
        print("Opção inválida! Usando n = 10 por padrão.") # caso o User nao digite certo
        n = 10 # usa o v10 parra n se extressarmos

    # chamada da f(x) calc de area
    resultado = calcular_area(a, b, n)

    # saida
    print()
    print(f"A área aproximada no Intervalo [{a}, {b}]")
    print(f"R: {resultado:.6f}")  # 6 casas decimais

# chamada da main
main()