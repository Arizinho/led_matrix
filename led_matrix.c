#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

//arquivo .pio
#include "led_matrix.pio.h"

//número de LEDs
#define NUM_PIXELS 25

//pino de saída
#define OUT_PIN 7

//número de sequências que aparecerão na animação
#define NUM_SEQUENCIAS 18

//número de linhas e colunas teclado
#define ROWS 4
#define COLUMNS 4

//matriz de quadros para animação do corpo do gato (12 quadros / 25 leds)
bool cat_body[12][25] = {{1, 0, 1, 0, 1,
                        1, 1, 1, 0, 1, 
                        1, 1, 1, 1, 1,
                        0, 1, 1, 1, 1,
                        1, 1, 0, 1, 0},

                        {0, 1, 1, 0, 1,
                        1, 1, 1, 0, 1, 
                        1, 1, 1, 1, 1,
                        0, 1, 1, 1, 1,
                        1, 1, 0, 1, 0},

                        {0, 1, 1, 0, 1,
                        1, 1, 1, 1, 0, 
                        1, 1, 1, 1, 1,
                        0, 1, 1, 1, 1,
                        1, 1, 0, 1, 0},

                        {1, 0, 1, 0, 1,
                        1, 1, 1, 1, 0, 
                        1, 1, 1, 1, 1,
                        0, 1, 1, 1, 1,
                        1, 1, 0, 1, 0},
                        
                        {1, 0, 1, 1, 0,
                        1, 1, 1, 0, 1, 
                        1, 1, 1, 1, 1,
                        0, 1, 1, 1, 1,
                        1, 1, 0, 1, 0},
                        
                        {1, 0, 1, 0, 0,
                        1, 1, 1, 0, 1, 
                        1, 1, 1, 1, 1,
                        0, 1, 1, 1, 1,
                        1, 1, 0, 1, 0},
                        
                        {1, 0, 0, 0, 0,
                        0, 0, 1, 0, 1,
                        1, 1, 1, 1, 1, 
                        1, 1, 1, 1, 1,
                        1, 1, 1, 1, 0},
                        
                        {0, 0, 0, 0, 0,
                        0, 0, 1, 0, 1,
                        1, 0, 1, 1, 1, 
                        1, 1, 1, 1, 1,
                        1, 1, 1, 1, 0},
                        
                        {0, 0, 0, 0, 0,
                        0, 0, 1, 1, 0,
                        1, 0, 1, 1, 1, 
                        1, 1, 1, 1, 1,
                        1, 1, 1, 1, 0},
                        
                        {0, 0, 0, 0, 0,
                        0, 0, 1, 0, 0,
                        1, 1, 1, 1, 1, 
                        1, 1, 1, 1, 1,
                        1, 1, 1, 1, 0},
                        
                        {0, 0, 0, 0, 0,
                        0, 0, 1, 0, 0,
                        1, 0, 1, 1, 1, 
                        1, 1, 1, 1, 1,
                        1, 1, 1, 1, 0},
                        
                        {0, 0, 0, 0, 0,
                        0, 0, 1, 0, 0,
                        0, 0, 1, 1, 1, 
                        1, 1, 1, 1, 1,
                        1, 1, 1, 1, 0}};

//matriz de quadros para animação da posição dos olhos do gato (12 quadros / 25 leds)
bool cat_eyes[12][25] = {{0, 0, 0, 0, 0,
                        1, 0, 1, 0, 0, 
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0},

                        {0, 0, 0, 0, 0,
                        1, 0, 1, 0, 0, 
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0},

                        {0, 0, 0, 0, 0,
                        1, 0, 1, 0, 0, 
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0},

                        {0, 0, 0, 0, 0,
                        1, 0, 1, 0, 0, 
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0},
                        
                        {0, 0, 0, 0, 0,
                        0, 1, 0, 0, 0, 
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0},
                        
                        {0, 0, 0, 0, 0,
                        1, 0, 0, 0, 0, 
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0},
                        
                        {0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 1, 
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0},
                        
                        {0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 1, 
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0},
                        
                        {0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 1, 
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0},
                        
                        {0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 1, 
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0},
                        
                        {0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 1, 
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0},
                        
                        {0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 1, 
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0}};

//array de sequência dos quadros na animação
//idenfica: {quadro a ser mostrado, quantidade de frames que o quadro será mostrado}
uint8_t sequencia_quadros[NUM_SEQUENCIAS][2] = {{0,1},{1,1},{2,1},{3,1},{0,20},{0,1},{1,1},{2,1},{3,1},{0,20},{4,4},{5,8},{6,1},{7,1},{8,1},{9,1},{10,1},{11,1}};

//pinos de linhas e colunas do teclado matricial
const uint8_t pin_rows[ROWS] = {2, 3, 4, 5};
const uint8_t pin_columns[COLUMNS] = {16, 17, 18, 19};

//matriz de mapeamento dos botões do teclado matricial
const char key_map[ROWS][COLUMNS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

//função para inicializar os pinos do teclado matricial
void teclado_init (void){
    int i;
    //inicializa os pinos de linha do teclado matricial como saída
    for(i = 0; i < ROWS; i++){
        gpio_init(pin_rows[i]);
        gpio_set_dir(pin_rows[i], GPIO_OUT);
    }

    //inicializa os pinos de coluna do teclado matricial como entrada
    for(i = 0; i < COLUMNS; i++){
        gpio_init(pin_columns[i]);
        gpio_set_dir(pin_columns[i], GPIO_IN);
        gpio_pull_down(pin_columns[i]);
    }

}

//função para ler tecla pressionada
char teclado_get(void){
    int r, c;
    for (r = 0; r < ROWS; r++){
        //coloca sinal de tensão alto nos pinos de linha do teclado
        gpio_put(pin_rows[r], true);
        //varre as colunas de 1 a 4 do teclado testando se algum botão na linha atual foi precionado
        for (c = 0; c < COLUMNS; c++){
            //se precionado retorna o caractere correspondente ao botão
            if(gpio_get(pin_columns[c])){
                gpio_put(pin_rows[r], false);
                return key_map[r][c];
            }
        }
        gpio_put(pin_rows[r], false);
    }
    //se nenhum botão for precionado retorna caractere nulo
    return '\0';
}

//rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

//rotina para acionar a matrix de leds - ws2812b
void desenha_cor(PIO pio, uint sm, double r, double g, double b){
    uint32_t valor_led;
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(b, r, g);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    sleep_ms(250);
}

//rotina para mostrar frames da animação
void animacao_pio(bool *animacao_body, bool *animacao_eyes, PIO pio, uint sm){
    uint32_t valor_led;
    for (uint8_t i = 0; i < NUM_PIXELS; i++) {
        //mostra cor verde no led para olhos do gato
        if (animacao_eyes[24-i]==1){
            valor_led = matrix_rgb(0.13, 0.7, 0.9);
            pio_sm_put_blocking(pio, sm, valor_led);
        }

        //mostra cor laranja no led para corpo do gato
        else if(animacao_body[24-i]==1){
            valor_led = matrix_rgb(0.15, 1.0, 0.5);
            pio_sm_put_blocking(pio, sm, valor_led);
        }

        //led apagado para outros casos
        else {
            valor_led = matrix_rgb(0, 0, 0);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
    }
}



int main()
{   
    char tecla; //variavel para armazenar tecla pressionada
    PIO pio = pio0; //seleciona a pio0
    teclado_init(); //inicializa teclado

    //coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    set_sys_clock_khz(128000, false);

    // Inicializa todos os códigos stdio padrão que estão ligados ao binário.
    stdio_init_all();

    //configurações da PIO
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    while (true) {
        //lê teclado
        tecla = teclado_get();
        switch (tecla)
        {
        case 'A':
            //Apaga matriz de LEDs
            desenha_cor(pio, sm, 0, 0, 0);
            break;
        
        case 'B':
            //Acende cor azul em 100% na matriz de LEDs
            desenha_cor(pio, sm, 0, 0, 1);
            break;

        case 'C':
            //Acende cor vermelha em 80% na matriz de LEDs
            desenha_cor(pio, sm, 0.8, 0, 0);
            break;

        case 'D':
            //Acende cor verde em 50% na matriz de LEDs
            desenha_cor(pio, sm, 0, 0.5, 0);
            break;

        case '#':
            //Acende cor branca em 20% na matriz de LEDs
            desenha_cor(pio, sm, 0.2, 0.2, 0.2);
            break;

        case '*':
            //Entra em modo de armazenamento
            reset_usb_boot(0,0);
            break;

        case '1':
            //Mostra animação de gato na matriz de LEDs
            for (uint8_t j = 0; j < NUM_SEQUENCIAS; j++){
                for (uint8_t i = 0; i < sequencia_quadros[j][1]; i++){
                    animacao_pio(cat_body[sequencia_quadros[j][0]],cat_eyes[sequencia_quadros[j][0]],pio,sm);
                    sleep_ms(125);
                }
            }
            //apaga LEDs ao final da animação
            desenha_cor(pio, sm, 0, 0, 0);
            break;

        //informa animação indisponível para teclas 2~9
        case '2':
            printf("Animação indisponível\n");
            sleep_ms(250);
            break;

        case '3':
            printf("Animação indisponível\n");
            sleep_ms(250);
            break;

        case '4':
            printf("Animação indisponível\n");
            sleep_ms(250);
            break;

        case '5':
            printf("Animação indisponível\n");
            sleep_ms(250);
            break;

        case '6':
            printf("Animação indisponível\n");
            sleep_ms(250);
            break;

        case '7':
            printf("Animação indisponível\n");
            sleep_ms(250);
            break;

        case '8':
            printf("Animação indisponível\n");
            sleep_ms(250);
            break;

        case '9':
            printf("Animação indisponível\n");
            sleep_ms(250);
            break;

        case '0':
            printf("Animação indisponível\n");
            sleep_ms(250);
            break;

        default:
            //Delay de 1ms para diminuir custo computacional da simulação no computador 
            sleep_ms(1);
            break;
        }
    }
}
