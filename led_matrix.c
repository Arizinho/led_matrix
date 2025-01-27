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

//número de linhas e colunas teclado
#define ROWS 4
#define COLUMNS 4

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
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        if (i%2==0)
        {
            valor_led = matrix_rgb(desenho[24-i], r=0.0, g=0.0);
            pio_sm_put_blocking(pio, sm, valor_led);

        }else{
            valor_led = matrix_rgb(b=0.0, desenho[24-i], g=0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
    }
}



int main()
{   
    char tecla;
    PIO pio = pio0; 
    teclado_init();
    bool ok;
    uint16_t i;
    uint32_t valor_led;
    double r = 0.0, b = 0.0 , g = 0.0;

    //coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);

    // Inicializa todos os códigos stdio padrão que estão ligados ao binário.
    stdio_init_all();

    printf("iniciando a transmissão PIO");
    if (ok) printf("clock set to %ld\n", clock_get_hz(clk_sys));

    //configurações da PIO
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    while (true) {
        //lê teclado
        tecla = teclado_get();
        switch (tecla)
        {
        case /* constant-expression */:
            /* code */
            break;
        
        default:
            break;
        }
    }
}
