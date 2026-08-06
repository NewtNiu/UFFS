// =====================================================
//  MÓDULO DIVISOR DE CLOCK
// =====================================================
module clock_divider(
    input clk_in,   // Clock de 50 MHz da FPGA
    input reset_n,  // Reset ativo baixo
    output reg clk_out // Clock de saída (gerado pela divisão)
);

    reg [24:0] counter; // Contador para dividir o clock

    // Divide o clock de 50 MHz para um clock de 1 Hz
    always @(posedge clk_in or negedge reset_n) begin
        if (~reset_n) begin
            counter <= 0;
            clk_out <= 0;
        end else begin
            if (counter == 25'd24999999) begin // 50MHz / (2*25e6) = 1Hz
                counter <= 0;
                clk_out <= ~clk_out; // Alterna o clock de saída
            end else begin
                counter <= counter + 1;
            end
        end
    end
endmodule


// =====================================================
//  MÓDULO PRINCIPAL DO ELEVADOR
// =====================================================
module Elevador(
    input CLOCK_50,      // Clock de 50 MHz da FPGA
    input  [9:0] SW,     // Interruptores (andares, pessoas, emergência)
    input  [3:0] KEY,    // Botões (KEY1 = emergência/reset)
    output reg [6:0] HEX0, // Andar atual
    output reg [6:0] HEX1, // Andar solicitado
    output reg [6:0] HEX2, // Direção
    output reg [6:0] HEX3, // Emergência
    output reg [9:0] LEDR  // LEDs
);

    // =====================================================
    //  Instancia o divisor de clock
    // =====================================================
    wire slow_clk;
    wire reset_n = KEY[1]; // Reset ativo alto (pressionar KEY1 = 0 → reset)
    
    clock_divider clkdiv (
        .clk_in(CLOCK_50),
        .reset_n(reset_n),
        .clk_out(slow_clk)
    );

    // =====================================================
    //  Registradores principais
    // =====================================================
    reg [3:0] estado_atual, next_state;
    reg porta;
    reg [2:0] andar;               // posição atual
    reg [1:0] direcao;             // 0=parado, 1=subindo, 2=descendo

    // =====================================================
    //  Parâmetros de estado
    // =====================================================
    parameter A1 = 4'd1,
              A2 = 4'd2,
              A3 = 4'd3,
              A4 = 4'd4,
              A5 = 4'd5,
              Subindo   = 4'd6,
              Descendo  = 4'd7;

    // =====================================================
    //  Mapeamento dos andares
    // =====================================================
    wire andar1_req = SW[5];
    wire andar2_req = SW[6];
    wire andar3_req = SW[7];
    wire andar4_req = SW[8];
    wire andar5_req = SW[9];

    // =====================================================
    //  Mapeamento das pessoas e emergência
    // =====================================================
    wire pessoa0 = SW[0];
    wire pessoa1 = SW[1];
    wire pessoa2 = SW[2];
    wire emergencia_sw = SW[3]; // emergência via chave
    wire emergencia = emergencia_sw | ~KEY[1]; // emergência combinada

    // =====================================================
    //  Andar solicitado (prioridade: mais alto)
    // =====================================================
    reg [2:0] req_andar;
    always @(*) begin
        if (andar5_req)      req_andar = 3'd5;
        else if (andar4_req) req_andar = 3'd4;
        else if (andar3_req) req_andar = 3'd3;
        else if (andar2_req) req_andar = 3'd2;
        else if (andar1_req) req_andar = 3'd1;
        else                 req_andar = andar;
    end

    // =====================================================
    //  Inicialização
    // =====================================================
    initial begin
        estado_atual = A1;
        next_state   = A1;
        porta        = 0;
        andar        = 1;
        direcao      = 0;
    end

    // =====================================================
    //  Máquina de estados combinacional
    // =====================================================
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

                default: next_state = A1;
            endcase
        end
    end

    // =====================================================
    //  Máquina sequencial (agora usando slow_clk)
    // =====================================================
    always @(posedge slow_clk or negedge reset_n) begin
        if (~reset_n) begin
            estado_atual <= A1;
            andar <= 1;
            direcao <= 0;
        end 
        else begin
            if (emergencia) begin
                direcao <= 0; // parado
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

    // =====================================================
    //  Saídas (LEDs e Displays)
    // =====================================================
    always @(*) begin
        LEDR = 10'b0;

        // LEDs de pessoas e emergência
        LEDR[0] = pessoa0;
        LEDR[1] = pessoa1;
        LEDR[2] = pessoa2;
        LEDR[3] = emergencia;

        // LEDs do andar atual (LEDR[5–9])
        LEDR[andar + 4] = 1'b1;

        // HEX0 = andar atual
        case (andar)
            3'd1: HEX0 = 7'b1111001;
            3'd2: HEX0 = 7'b0100100;
            3'd3: HEX0 = 7'b0110000;
            3'd4: HEX0 = 7'b0011001;
            3'd5: HEX0 = 7'b0010010;
            default: HEX0 = 7'b1111111;
        endcase

        // HEX1 = andar solicitado
        case (req_andar)
            3'd1: HEX1 = 7'b1111001;
            3'd2: HEX1 = 7'b0100100;
            3'd3: HEX1 = 7'b0110000;
            3'd4: HEX1 = 7'b0011001;
            3'd5: HEX1 = 7'b0010010;
            default: HEX1 = 7'b1111111;
        endcase

        // HEX2 = direção
        case (direcao)
            0: HEX2 = 7'b1000000; // 0 = parado
            1: HEX2 = 7'b1111001; // 1 = subindo
            2: HEX2 = 7'b0111111; // - = descendo
            default: HEX2 = 7'b1111111;
        endcase

        // HEX3 = emergência
        if (emergencia)
            HEX3 = 7'b1111001; // "1" = ativa
        else
            HEX3 = 7'b1000000; // "0" = inativa
    end
endmodule
