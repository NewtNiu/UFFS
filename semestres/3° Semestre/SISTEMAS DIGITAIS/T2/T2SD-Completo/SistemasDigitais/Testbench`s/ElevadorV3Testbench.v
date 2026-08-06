`timescale 1ns/1ns

module tb_elevador_codigo3;

    reg CLOCK_50;
    reg [9:0] SW;
    reg [3:0] KEY;
    wire [6:0] HEX0, HEX1, HEX2, HEX3;
    wire [9:0] LEDR;

    integer i;
    integer total_movimentos;
    integer req_andar;
    
    reg [2:0] andar_anterior;
    reg [2:0] andar_atual_mon;

    // Instancia o novo elevador V3
    Elevador_v3 dut (
        .CLOCK_50(CLOCK_50),
        .SW(SW),
        .KEY(KEY),
        .HEX0(HEX0),
        .HEX1(HEX1),
        .HEX2(HEX2),
        .HEX3(HEX3),
        .LEDR(LEDR)
    );

    initial CLOCK_50 = 0;
    always #10 CLOCK_50 = ~CLOCK_50;

    // Monitoramento
    always @(dut.andar) begin
        andar_atual_mon = dut.andar;
        
        if (andar_atual_mon != andar_anterior) begin
            if (andar_atual_mon > andar_anterior)
                total_movimentos = total_movimentos + (andar_atual_mon - andar_anterior);
            else
                total_movimentos = total_movimentos + (andar_anterior - andar_atual_mon);
            
            andar_anterior = andar_atual_mon;
        end
    end

    // Teste
    initial begin
        $display("=== INICIANDO SIMULACAO - CODIGO 3 (SIMPLES, BASE: ANDAR 3) ===");
        
        SW = 0;
        KEY = 4'b1111; 
        total_movimentos = 0;
        
        andar_anterior = 3; 

        // Reset
        #100;
        KEY[1] = 0; 
        #100;
        KEY[1] = 1; 
        #100;

        // Aguarda estabilizar no andar 3
        wait(dut.andar == 3);

        for (i = 1; i <= 100; i = i + 1) begin
            
            req_andar = $urandom_range(1, 5);
            
            while (req_andar == dut.andar) begin
                 req_andar = $urandom_range(1, 5);
            end

            case(req_andar)
                1: SW[5] = 1;
                2: SW[6] = 1;
                3: SW[7] = 1;
                4: SW[8] = 1;
                5: SW[9] = 1;
            endcase

            wait(dut.andar == req_andar);

            case(req_andar)
                1: SW[5] = 0;
                2: SW[6] = 0;
                3: SW[7] = 0;
                4: SW[8] = 0;
                5: SW[9] = 0;
            endcase

            #500;
        end

        $display("--------------------------------------------------");
        $display("FIM DA SIMULACAO (CODIGO 3 - SIMPLES/ANDAR 3)");
        $display("TOTAL DE ANDARES PERCORRIDOS: %d", total_movimentos);
        $display("--------------------------------------------------");
        $stop;
    end
endmodule