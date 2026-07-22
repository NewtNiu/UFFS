# Niumar Girardi
# Ciencia da Computacao 
# UFFS
# -----------------------------------------------------------------------------------------------------------------------------------------------


cores_ansi = {
    "Azul": "\033[34m",  # azul
    "Vermelho": "\033[31m",  # vermelho
    "Amarelo": "\033[33m",  # amarelo
    "Verde": "\033[32m",  # verde
    "Reset": "\033[0m"  # resetar cor para o padrão
}



# Biblioteca para embaralhar as cartas
import random  

# Função para criar a tela inicial
def tela_inicial():
    while True:#faz os menus da tela iniciar(baseado na escolha dos users chama a funcao correspondentte)
        print("\n=== Bem-vindo ao UNO em PY ===")
        print("1. Jogar")
        print("2. Regras")
        print("3. Sair")
        #as opcoes
        escolha = input("Escolha uma opção (1/2/3): ")
        if escolha == "1":
            iniciar_jogo()
        elif escolha == "2":
            mostrar_regras()
        elif escolha == "3":
            print("Encerrando o programa. Até logo!")
            break
        else:
            print("Opção inválida. Tente novamente.")

# Conforme a escolhaa ---------------------------------------------------------------------------------------------------------------------------

# regras
def mostrar_regras():# bem basico so apresenta varios print
    print("\n=== Regras do Jogo ===")
    print("1. O baralho possui cartas numeradas de 1 a 9 e de 4 cores (azul, vermelha, amarela e verde).")
    print("2. Cada jogador começa com 5 cartas.")
    print("3. No turno, o jogador deve jogar uma carta que combine com a cor ou número da carta aberta na mesa.")
    print("4. Se não puder jogar, deve comprar uma carta do baralho.")
    print("5. O jogo termina quando um jogador ficar sem cartas ou o baralho acabar.")
    print("6. Ganha quem terminar as cartas primeiro ou quem tiver menos cartas ao final.")
    input("\nPressione Enter para voltar ao menu.")

# ------------------------------------------------------------------------------------------------------------------------------------------------
#  jogo 
def iniciar_jogo():#da play no jogo

# aqui e como se fosse um "requisito nao fucional"(nao aparece para o usuario mas precisa para que o jogo funcione) 
# e como se fosse o inicio do jopo
    # pede nome aos jogadores
    jogador1 = input("\nDigite o nome do Jogador 1: ")
    jogador2 = input("Digite o nome do Jogador 2: ")
    jogadores = [jogador1, jogador2]
    # Cria o baralho
    cores = ['azul', 'vermelha', 'amarela', 'verde']
    numeros = list(range(1, 10))
    baralho = [{'cor': cor, 'numero': numero} for cor in cores for numero in numeros]
    random.shuffle(baralho)  # Embaralha o baralho
    # Distribui as cartas iniciais
    maos = {jogador1: [baralho.pop() for _ in range(5)], jogador2: [baralho.pop() for _ in range(5)]}
    # Define a primeira carta da mesa
    carta_mesa = baralho.pop()
    print(f"\nA primeira carta aberta na mesa é {carta_mesa['numero']} de cor {carta_mesa['cor']}.")

# A estrutura do jogo(e aqui que aplica as regras do jogo de la do pdf)
    # Turno dos jogadores
    jogador_atual = jogador1  # Jogador inicial
    while True:
        print(f"\n=== Turno de {jogador_atual} ===")
        print(f"Carta na mesa: {carta_mesa['numero']} de cor {carta_mesa['cor']}")#para os jogadores se orientarem
        print(f"Suas cartas: {maos[jogador_atual]}")
        # Verifica se o jogador pode jogar
        jogavel = [carta for carta in maos[jogador_atual] if carta['cor'] == carta_mesa['cor'] or carta['numero'] == carta_mesa['numero']]
        if jogavel:
            print("Você pode jogar as seguintes cartas:")
            for i, carta in enumerate(jogavel, start=1):
                print(f"{i}. {carta['numero']} de cor {carta['cor']}")
            escolha = int(input("Escolha a carta a jogar (número correspondente): ")) - 1
            carta_escolhida = jogavel[escolha]
            maos[jogador_atual].remove(carta_escolhida)  # Remove a carta escolhida da mão
            carta_mesa = carta_escolhida  # Atualiza a carta na mesa
        else:
            print("Você não possui cartas jogáveis.\nComprando uma carta...")
            if baralho:
                nova_carta = baralho.pop()
                maos[jogador_atual].append(nova_carta)
                print(f"Você comprou {nova_carta['numero']} de cor {nova_carta['cor']}.")
            else:
                print("O baralho acabou. Finalizando o jogo...")
                break

        # Verifica se o jogador venceu
        if not maos[jogador_atual]:
            print(f"\nParabéns, {jogador_atual}! Você venceu!")
            return
        # muda para o próximo jogador
        jogador_atual = jogador1 if jogador_atual == jogador2 else jogador2
    # verifica o vencedor quando baralho estiver vazio
    print("\n=== O baralho acabou! ===")
    cartas_restantes = {jogador: len(maos[jogador]) for jogador in jogadores} #le a mao do jogador
    for jogador, num_cartas in cartas_restantes.items():
        print(f"{jogador} tem {num_cartas} cartas restantes.")
    vencedor = min(cartas_restantes, key=cartas_restantes.get)
    if list(cartas_restantes.values()).count(cartas_restantes[vencedor]) > 1:
        print("O jogo terminou empatado!")
    else:
        print(f"O vencedor é {vencedor}!")
# ------------------------------------------------------------------------------------------------------------------------------------------------

# aperta o play neymar
tela_inicial()
