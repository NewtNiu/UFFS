// =====================================================
//  MÓDULO DIVISOR DE CLOCK
// =====================================================
module clock_divider(
    input clk_in,
    input reset_n,
    output reg clk_out
);
    reg [24:0] counter;
    // 25'd24999999 = Aprox 1Hz (1 segundo).
    always @(posedge clk_in or negedge reset_n) begin
        if (~reset_n) begin
            counter <= 0;
            clk_out <= 0;
        end else begin
            if (counter == 25'd24999999) begin 
                counter <= 0;
                clk_out <= ~clk_out;
            end else begin
                counter <= counter + 1;
            end
        end
    end
endmodule

// =====================================================
//  MÓDULO PRINCIPAL - ELEVADOR V4 (AJUSTADO)
// =====================================================
module Elevador_v2(
    input CLOCK_50,
    input [9:0] SW,      // SW[3]=Trava, SW[9..5]=Andares
    input [3:0] KEY,     // KEY[1]=Reset, KEY[2]=Emergência
    output reg [6:0] HEX0, 
    output reg [6:0] HEX1, 
    output reg [6:0] HEX2,
    output reg [6:0] HEX3,
    output reg [9:0] LEDR
);

    // --- Sinais e Clock ---
    wire slow_clk;
    wire reset_n = KEY[1];
    wire sw_trava = SW[3];         
    wire btn_emergencia = ~KEY[2]; // emergência ativa em nível baixo

    clock_divider clkdiv (
        .clk_in(CLOCK_50),
        .reset_n(reset_n),
        .clk_out(slow_clk)
    );

    // --- Mapeamento dos Andares ---
    wire [5:1] req_sw;
    assign req_sw[1] = SW[5];
    assign req_sw[2] = SW[6];
    assign req_sw[3] = SW[7];
    assign req_sw[4] = SW[8];
    assign req_sw[5] = SW[9];

    // --- Registradores de Estado ---
    reg [2:0] andar_atual;
    reg [1:0] direcao_atual;   // 0=Parado, 1=Subindo, 2=Descendo
    reg [1:0] contador_espera;

    // Codificação dos Estados
    reg [3:0] estado;
    
    parameter ST_PARADO       = 4'd0;
    parameter ST_MOVENDO      = 4'd1;
    parameter ST_ESPERANDO_SW = 4'd2;
    
    parameter ST_TRAVADO_SW3  = 4'd3;
    parameter ST_EMERG_INDO_1 = 4'd4;
    parameter ST_EMERG_ESPERA = 4'd5;
    parameter ST_EMERG_INDO_3 = 4'd6;

    // --- Lógica Combinacional de Requisições ---
    reg tem_req_acima;
    reg tem_req_abaixo;

    always @(*) begin
        tem_req_acima = 0;
        tem_req_abaixo = 0;
        
        if (andar_atual < 5 && req_sw[5]) tem_req_acima = 1;
        if (andar_atual < 4 && req_sw[4]) tem_req_acima = 1;
        if (andar_atual < 3 && req_sw[3]) tem_req_acima = 1;
        if (andar_atual < 2 && req_sw[2]) tem_req_acima = 1;

        if (andar_atual > 1 && req_sw[1]) tem_req_abaixo = 1;
        if (andar_atual > 2 && req_sw[2]) tem_req_abaixo = 1;
        if (andar_atual > 3 && req_sw[3]) tem_req_abaixo = 1;
        if (andar_atual > 4 && req_sw[4]) tem_req_abaixo = 1;
    end

    // --- Lógica Sequencial Principal ---
    always @(posedge slow_clk or negedge reset_n) begin
        if (~reset_n) begin
            andar_atual <= 3;
            direcao_atual <= 0;
            estado <= ST_PARADO;
            contador_espera <= 0;
        end else begin
            
            if (btn_emergencia && estado != ST_EMERG_INDO_1 && estado != ST_EMERG_ESPERA && estado != ST_EMERG_INDO_3) begin
                estado <= ST_EMERG_INDO_1;
                direcao_atual <= 0; 
            end
            else begin
                
                case (estado)
                    ST_EMERG_INDO_1: begin
                        if (andar_atual > 1) begin
                            direcao_atual <= 2;
                            andar_atual <= andar_atual - 1;
                        end else begin
                            direcao_atual <= 0;
                            contador_espera <= 0;
                            estado <= ST_EMERG_ESPERA;
                        end
                    end

                    ST_EMERG_ESPERA: begin
                        if (contador_espera < 2) begin
                            contador_espera <= contador_espera + 1;
                        end else begin
                            estado <= ST_EMERG_INDO_3;
                        end
                    end

                    ST_EMERG_INDO_3: begin
                        if (andar_atual < 3) begin
                            direcao_atual <= 1;
                            andar_atual <= andar_atual + 1;
                        end else if (andar_atual > 3) begin
                            direcao_atual <= 2;
                            andar_atual <= andar_atual - 1;
                        end else begin
                            direcao_atual <= 0;
                            estado <= ST_PARADO;
                        end
                    end

                    ST_TRAVADO_SW3: begin
                        direcao_atual <= 0;
                        if (!sw_trava)
                            estado <= ST_PARADO;
                    end

                    ST_PARADO: begin
                        if (sw_trava)
                            estado <= ST_TRAVADO_SW3;

                        else if (req_sw[andar_atual])
                            estado <= ST_ESPERANDO_SW;

                        else begin
                            if (direcao_atual == 1 && tem_req_acima)      
                                estado <= ST_MOVENDO;
                            else if (direcao_atual == 2 && tem_req_abaixo) 
                                estado <= ST_MOVENDO;
                            else if (tem_req_acima) begin 
                                direcao_atual <= 1; 
                                estado <= ST_MOVENDO; 
                            end
                            else if (tem_req_abaixo) begin 
                                direcao_atual <= 2; 
                                estado <= ST_MOVENDO; 
                            end
                            else begin 
                                if (andar_atual < 3) begin 
                                    direcao_atual <= 1; 
                                    estado <= ST_MOVENDO; 
                                end
                                else if (andar_atual > 3) begin 
                                    direcao_atual <= 2; 
                                    estado <= ST_MOVENDO; 
                                end
                                else direcao_atual <= 0;
                            end
                        end
                    end

                    ST_MOVENDO: begin
                        if (sw_trava) begin
                            estado <= ST_TRAVADO_SW3;
                        end else begin
                            if (direcao_atual == 1 && andar_atual < 5)
                                andar_atual <= andar_atual + 1;
                            else if (direcao_atual == 2 && andar_atual > 1)
                                andar_atual <= andar_atual - 1;

                            estado <= ST_PARADO;
                        end
                    end

                    ST_ESPERANDO_SW: begin
                        if (sw_trava)
                             estado <= ST_TRAVADO_SW3;
                        else begin
                            direcao_atual <= 0;
                            if (!req_sw[andar_atual])
                                estado <= ST_PARADO;
                        end
                    end
                    
                    default: estado <= ST_PARADO;
                endcase
            end
        end
    end

    // =====================================================
    //  SAÍDAS VISUAIS — AJUSTADAS PARA IMITAR O SEGUNDO CÓDIGO
    // =====================================================

    // Determina o andar solicitado mais alto
    reg [2:0] req_andar;
    always @(*) begin
        if (req_sw[5])      req_andar = 3'd5;
        else if (req_sw[4]) req_andar = 3'd4;
        else if (req_sw[3]) req_andar = 3'd3;
        else if (req_sw[2]) req_andar = 3'd2;
        else if (req_sw[1]) req_andar = 3'd1;
        else                req_andar = andar_atual;
    end

    always @(*) begin
        
        // HEX0 — andar atual
        case(andar_atual)
            3'd1: HEX0 = 7'b1111001; 
            3'd2: HEX0 = 7'b0100100; 
            3'd3: HEX0 = 7'b0110000; 
            3'd4: HEX0 = 7'b0011001; 
            3'd5: HEX0 = 7'b0010010; 
            default: HEX0 = 7'b1111111;
        endcase

        // HEX1 — andar solicitado mais alto
        case(req_andar)
            3'd1: HEX1 = 7'b1111001;
            3'd2: HEX1 = 7'b0100100;
            3'd3: HEX1 = 7'b0110000;
            3'd4: HEX1 = 7'b0011001;
            3'd5: HEX1 = 7'b0010010;
            default: HEX1 = 7'b1111111;
        endcase

        // HEX2 — direção
        case(direcao_atual)
            2'd1: HEX2 = 7'b1111001; // sobe
            2'd2: HEX2 = 7'b0111111; // "-"
            default: HEX2 = 7'b1000000; // parado
        endcase

        // HEX3 — emergência
        if (btn_emergencia)
            HEX3 = 7'b1111001; // "1"
        else
            HEX3 = 7'b1000000; // "0"

        // LEDS — comportamento igual ao segundo código
        LEDR = 10'b0;

        // Pessoas
        LEDR[0] = SW[0];
        LEDR[1] = SW[1];
        LEDR[2] = SW[2];

        // Emergência
        LEDR[3] = btn_emergencia;

        // LEDR[5..9] = andar atual
        if (andar_atual >= 1 && andar_atual <= 5)
            LEDR[andar_atual + 4] = 1'b1;
    end

endmodule
