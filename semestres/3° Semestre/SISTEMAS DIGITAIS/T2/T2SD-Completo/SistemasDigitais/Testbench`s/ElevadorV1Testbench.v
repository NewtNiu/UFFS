`timescale 1ns/1ns

module tb_elevador_codigo2;

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
    integer i;                  // Iterador
    integer total_movimentos;   // Acumulador de passos
    integer req_andar;          // Andar sorteado
    
    reg [2:0] andar_anterior;   // Para cálculo de delta
    reg [2:0] andar_atual_mon;  // Monitoramento

    // Instanciação do DUT 
    Elevador dut (
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
    // 3. Clock
    // ==========================================
    initial CLOCK_50 = 0;
    always #10 CLOCK_50 = ~CLOCK_50; // 50MHz

    // ==========================================
    // 4. Monitoramento de Movimento
    // ==========================================
    always @(dut.andar) begin
        andar_atual_mon = dut.andar;
        
        if (andar_atual_mon != andar_anterior) begin
            // Calcula diferença absoluta
            if (andar_atual_mon > andar_anterior)
                total_movimentos = total_movimentos + (andar_atual_mon - andar_anterior);
            else
                total_movimentos = total_movimentos + (andar_anterior - andar_atual_mon);
            
            andar_anterior = andar_atual_mon;
        end
    end

    // ==========================================
    // 5. Bateria de Testes
    // ==========================================
    initial begin

        $display("=== INICIANDO SIMULACAO - CODIGO 2 (BASE: ANDAR 1) ===");
        
        // Inicialização
        SW = 0;
        KEY = 4'b1111; // Reset inativo (KEY[1]=1)
        total_movimentos = 0;
        
        // O código 2 inicia no andar 1.
        andar_anterior = 1; 

        // Reset
        #100;
        KEY[1] = 0; // Reset ativo (nivel baixo)
        #100;
        KEY[1] = 1; // Solta reset
        #100;

        // Aguarda estabilizar no andar 1
        wait(dut.andar == 1);

        // --- Loop de 100 Requisições ---
        for (i = 1; i <= 100; i = i + 1) begin
            
            // 1. Sorteia andar (1 a 5)
            req_andar = $urandom_range(1, 5);
            
            // Garante que não pede o andar onde já está
            while (req_andar == dut.andar) begin
                 req_andar = $urandom_range(1, 5);
            end

            // SW[5]=1, SW[6]=2, SW[7]=3, SW[8]=4, SW[9]=5
            case(req_andar)
                1: SW[5] = 1;
                2: SW[6] = 1;
                3: SW[7] = 1;
                4: SW[8] = 1;
                5: SW[9] = 1;
            endcase

            // 3. Espera chegar
            wait(dut.andar == req_andar);

            // 4. Desliga Switch
            case(req_andar)
                1: SW[5] = 0;
                2: SW[6] = 0;
                3: SW[7] = 0;
                4: SW[8] = 0;
                5: SW[9] = 0;
            endcase

            // 5. Delay
            #500;
        end

        // --- Resultado ---
        $display("--------------------------------------------------");
        $display("FIM DA SIMULACAO (CODIGO 2 - INICIO ANDAR 1)");
        $display("TOTAL DE ANDARES PERCORRIDOS: %d", total_movimentos);
        $display("--------------------------------------------------");
        $stop;
    end

endmodule