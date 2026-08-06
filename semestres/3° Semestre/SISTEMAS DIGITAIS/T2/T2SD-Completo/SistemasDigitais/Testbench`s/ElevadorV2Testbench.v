`timescale 1ns/1ns

module tb_elevador_v2;

    // ==========================================
    // 1. Sinais de Interface
    // ==========================================
    reg CLOCK_50;
    reg [9:0] SW;
    reg [3:0] KEY;
    
    wire [6:0] HEX0, HEX1, HEX2, HEX3;
    wire [9:0] LEDR;

    // ==========================================
    // 2. Variáveis de Controle e Estatística
    // ==========================================
    integer i;                  // Iterador do loop
    integer total_movimentos;   // Acumulador de andares percorridos
    integer req_andar;          // Andar sorteado para a requisição
    
    reg [2:0] andar_anterior;   // Para calcular a diferença
    reg [2:0] andar_atual_mon;  // Monitoramento do sinal interno

    // Instanciação do DUT (Device Under Test)
    Elevador_v2 dut (
        .CLOCK_50(CLOCK_50),
        .SW(SW),
        .KEY(KEY),
        .HEX0(HEX0),
        .HEX1(HEX1),
        .HEX2(HEX2),
        .HEX3(HEX3),
        .LEDR(LEDR)
    );

    // ==========================================
    // 3. Geração de Clock (50MHz)
    // ==========================================
    initial CLOCK_50 = 0;
    always #10 CLOCK_50 = ~CLOCK_50; // Período de 20ns

    // ==========================================
    // 4. Monitoramento de Deslocamento
    // ==========================================
    // Este bloco observa o sinal interno 'andar_atual' dentro da instância 'dut'
    always @(dut.andar_atual) begin
        andar_atual_mon = dut.andar_atual;
        
        // Se houve mudança de andar
        if (andar_atual_mon != andar_anterior) begin
            // Calcula a distância percorrida (Módulo da diferença)
            if (andar_atual_mon > andar_anterior)
                total_movimentos = total_movimentos + (andar_atual_mon - andar_anterior);
            else
                total_movimentos = total_movimentos + (andar_anterior - andar_atual_mon);
            
            // Atualiza o anterior
            andar_anterior = andar_atual_mon;
        end
    end

    // ==========================================
    // 5. Bateria de Testes (Estímulos)
    // ==========================================
    initial begin
        // --- Inicialização ---
        $display("=== INICIANDO SIMULACAO DO ELEVADOR (BASE: ANDAR 3) ===");
        SW = 0;
        KEY = 4'b1111; // Chaves soltas (nível alto)
        total_movimentos = 0;
        andar_anterior = 3; // O código diz que inicia no 3 após reset
        
        // Reset do sistema
        #100;
        KEY[1] = 0; // Pressiona Reset
        #100;
        KEY[1] = 1; // Solta Reset
        #100;

        // Aguarda estabilização inicial
        wait(dut.andar_atual == 3);
        
        // --- Loop de 100 Requisições ---
        for (i = 1; i <= 100; i = i + 1) begin
            
            // 1. Sortear um andar aleatório entre 1 e 5
            req_andar = $urandom_range(1, 5);
            
            // Evitar pedir o mesmo andar que já está 
            while (req_andar == dut.andar_atual) begin
                 req_andar = $urandom_range(1, 5);
            end

            // 2. Acionar o Switch correspondente
            case(req_andar)
                1: SW[5] = 1;
                2: SW[6] = 1;
                3: SW[7] = 1;
                4: SW[8] = 1;
                5: SW[9] = 1;
            endcase

            wait(dut.andar_atual == req_andar);

            // 4. Desligar o Switch (simulando que a pessoa entrou/saiu e o botão soltou)
            case(req_andar)
                1: SW[5] = 0;
                2: SW[6] = 0;
                3: SW[7] = 0;
                4: SW[8] = 0;
                5: SW[9] = 0;
            endcase

            #500; 
        end

        // --- Resultado Final ---
        $display("--------------------------------------------------");
        $display("FIM DA BATERIA DE 100 REQUISICOES");
        $display("TOTAL DE ANDARES PERCORRIDOS: %d", total_movimentos);
        $display("--------------------------------------------------");
        $stop;
    end

endmodule