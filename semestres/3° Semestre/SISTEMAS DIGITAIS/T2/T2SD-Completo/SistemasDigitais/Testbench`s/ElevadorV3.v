// =====================================================
//  MÓDULO DIVISOR DE CLOCK (MODO SIMULAÇÃO)
// =====================================================
module clock_divider(
    input clk_in,
    input reset_n,
    output reg clk_out
);
    reg [24:0] counter;

    always @(posedge clk_in or negedge reset_n) begin
        if (~reset_n) begin
            counter <= 0;
            clk_out <= 0;
        end else begin
            if (counter == 25'd5) begin 
                counter <= 0;
                clk_out <= ~clk_out;
            end else begin
                counter <= counter + 1;
            end
        end
    end
endmodule

// =====================================================
//  MÓDULO PRINCIPAL DO ELEVADOR (V3 - Início no 3)
// =====================================================
module Elevador_v3(
    input CLOCK_50,
    input  [9:0] SW,
    input  [3:0] KEY,
    output reg [6:0] HEX0,
    output reg [6:0] HEX1,
    output reg [6:0] HEX2,
    output reg [6:0] HEX3,
    output reg [9:0] LEDR
);

    wire slow_clk;
    wire reset_n = KEY[1];
    
    clock_divider clkdiv (
        .clk_in(CLOCK_50),
        .reset_n(reset_n),
        .clk_out(slow_clk)
    );

    // Registradores
    reg [3:0] estado_atual, next_state;
    reg porta;
    reg [2:0] andar;       
    reg [1:0] direcao;     

    // Parâmetros
    parameter A1 = 4'd1,
              A2 = 4'd2,
              A3 = 4'd3,
              A4 = 4'd4,
              A5 = 4'd5,
              Subindo   = 4'd6,
              Descendo  = 4'd7;

    // Entradas
    wire andar1_req = SW[5];
    wire andar2_req = SW[6];
    wire andar3_req = SW[7];
    wire andar4_req = SW[8];
    wire andar5_req = SW[9];

    wire pessoa0 = SW[0];
    wire pessoa1 = SW[1];
    wire pessoa2 = SW[2];
    wire emergencia_sw = SW[3];
    wire emergencia = emergencia_sw | ~KEY[1];

    // Lógica de Prioridade
    reg [2:0] req_andar;
    always @(*) begin
        if (andar5_req)      req_andar = 3'd5;
        else if (andar4_req) req_andar = 3'd4;
        else if (andar3_req) req_andar = 3'd3;
        else if (andar2_req) req_andar = 3'd2;
        else if (andar1_req) req_andar = 3'd1;
        else                 req_andar = andar;
    end

    initial begin
        estado_atual = A3; // Começa no estado do andar 3
        next_state   = A3;
        porta        = 0;
        andar        = 3;  // Posição física 3
        direcao      = 0;
    end

    // Máquina de Estados
    always @(*) begin
        next_state = estado_atual;

        if (emergencia) begin
            next_state = estado_atual;
        end else begin
            case (estado_atual)
                A1, A2, A3, A4, A5: begin
                    if (req_andar > andar)
                        next_state = Subindo;
                    else if (req_andar < andar)
                        next_state = Descendo;
                    else begin
                        porta = 1'b1;
                        next_state = estado_atual;
                    end
                end

                Subindo: begin
                    if (andar < req_andar)
                        next_state = Subindo;
                    else
                        next_state = req_andar;
                end

                Descendo: begin
                    if (andar > req_andar)
                        next_state = Descendo;
                    else
                        next_state = req_andar;
                end

                default: next_state = A3; // Default seguro
            endcase
        end
    end

    always @(posedge slow_clk or negedge reset_n) begin
        if (~reset_n) begin
            estado_atual <= A3; // Reseta para o andar 3
            andar <= 3;         // Reseta posição para 3
            direcao <= 0;
        end 
        else begin
            if (emergencia) begin
                direcao <= 0;
            end 
            else begin
                estado_atual <= next_state;

                case (next_state)
                    Subindo: begin
                        if (andar < 5)
                            andar <= andar + 1;
                        direcao <= 1;
                    end
                    Descendo: begin
                        if (andar > 1)
                            andar <= andar - 1;
                        direcao <= 2;
                    end
                    A1, A2, A3, A4, A5: direcao <= 0;
                    default: direcao <= 0;
                endcase
            end
        end
    end

    // Saídas Visuais
    always @(*) begin
        LEDR = 10'b0;
        LEDR[0] = pessoa0;
        LEDR[1] = pessoa1;
        LEDR[2] = pessoa2;
        LEDR[3] = emergencia;
        LEDR[andar + 4] = 1'b1;

        case (andar)
            3'd1: HEX0 = 7'b1111001;
            3'd2: HEX0 = 7'b0100100;
            3'd3: HEX0 = 7'b0110000;
            3'd4: HEX0 = 7'b0011001;
            3'd5: HEX0 = 7'b0010010;
            default: HEX0 = 7'b1111111;
        endcase

        case (req_andar)
            3'd1: HEX1 = 7'b1111001;
            3'd2: HEX1 = 7'b0100100;
            3'd3: HEX1 = 7'b0110000;
            3'd4: HEX1 = 7'b0011001;
            3'd5: HEX1 = 7'b0010010;
            default: HEX1 = 7'b1111111;
        endcase

        case (direcao)
            0: HEX2 = 7'b1000000;
            1: HEX2 = 7'b1111001;
            2: HEX2 = 7'b0111111;
            default: HEX2 = 7'b1111111;
        endcase

        if (emergencia) HEX3 = 7'b1111001;
        else HEX3 = 7'b1000000;
    end
endmodule