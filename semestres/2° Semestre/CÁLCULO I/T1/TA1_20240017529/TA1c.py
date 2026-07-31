# Codigo by Niumar Girardi
# Bibliotecas
import math

# Funções
def func1(x):
    return math.exp(2 * x) + x**2 - 3

def func2(x):
    return 2 * math.cos(x) - x + 6

# Função principal
def bisc(f, a, b, erro=0.1):
    fa = f(a)
    fb = f(b)

    # Mostra os valores de f(a) e f(b)
    print(f"\nNo intervalo: [{a} = {fa:.4f}, {b} = {fb:.4f}]: ")

    if fa * fb > 0:
        return "Não é possível afirmar que existe solução neste intervalo, tente outros dois números"
    
    a_original, b_original = a, b  # Guardar para mostrar no print final

    while (b - a) / 2 > erro:
        m = (a + b) / 2
        if f(m) == 0:
            return f"A equação tem pelo menos uma solução neste intervalo.\nSolução exata encontrada: x = {m:.4f}"
        elif f(a) * f(m) < 0:
            b = m
        else:
            a = m

    return f"A equação tem pelo menos uma solução neste intervalo.\nSolução aproximada no intervalo: [{a:.4f}, {b:.4f}]"

# Interface
def main():
    print("Escolha a equação: ")
    print("1 - e^(2x) + x^2 = 3")
    print("2 - 2cos(x) + 4 = x - 2")
    escolha = input("Digite 1 ou 2: ")

    try:
        a = float(input("Digite o valor de a (início do intervalo): "))
        b = float(input("Digite o valor de b (fim do intervalo): "))
    except ValueError:
        print("Entrada inválida. Use apenas números.")
        return

    if escolha == "1":
        resultado = bisc(func1, a, b)
    elif escolha == "2":
        resultado = bisc(func2, a, b)
    else:
        resultado = "Escolha inválida."

    print(resultado)

# Corpo
main()