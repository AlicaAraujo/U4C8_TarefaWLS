#include <stdio.h>
#include <stdlib.h>
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "bibliotecas/botao.h"
#include "bibliotecas/leds.h"
#include "bibliotecas/pwm.h"
#include "bibliotecas/joystick.h"
#include "bibliotecas/ssd1306.h"

// Defines
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define DISPLAY_ADDR 0x3C
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define SQUARE_SIZE 8

// Variáveis globais
ssd1306_t display;
uint16_t vrx_value, vry_value;
int square_x = 64, square_y = 32;
uint16_t led_r_level = 0, led_b_level = 0;
volatile bool state_border = true;
absolute_time_t debounce;

// Função para atualizar posição do quadrado com base no joystick
void update_square_position(int *x, int *y, uint16_t vrx, uint16_t vry) {
    int delta_x = (4095 - vry) / 64;
    int delta_y = vrx / 32;
    *x = delta_x;
    *y = delta_y;
    
    // Limites do display
    if (*x < 8) *x = 8;
    if (*x > 56) *x = 56;
    if (*y < 12) *y = 12;
    if (*y > 124) *y = 124;
}

int main() {
    stdio_init_all();
    
    // Inicialização do hardware
    botao_init();
    inicializaLEDs();
    setup_joystick();
    pwm_led_setup(LED_R, &led_r_level);
    pwm_led_setup(LED_B, &led_b_level);
    led_init(LED_G);

    // Configuração do I2C e do display
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&display, DISPLAY_WIDTH, DISPLAY_HEIGHT, false, DISPLAY_ADDR, I2C_PORT);
    ssd1306_config(&display);
    ssd1306_fill(&display, false);
    ssd1306_send_data(&display);
    
    // Configuração das interrupções
    gpio_set_irq_enabled_with_callback(JOYSTICK_PB, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);
    debounce = delayed_by_ms(get_absolute_time(), 200);
    
    while (true) {
        joystick_read_axis(&vrx_value, &vry_value);

        // Controle do brilho dos LEDs RGB via PWM
        if ((vrx_value >= 2400 || vrx_value <= 2120) && state_led)
            led_r_level = abs(vrx_value - 2047) * 2;
        else
            led_r_level = 0;
        
        if ((vry_value >= 2100 || vry_value <= 1850) && state_led)
            led_b_level = abs(vry_value - 2047) * 2;
        else
            led_b_level = 0;
        
        pwm_set_gpio_level(LED_R, led_r_level);
        pwm_set_gpio_level(LED_B, led_b_level);

        // Atualiza o display
        ssd1306_fill(&display, false);
        update_square_position(&square_x, &square_y, vrx_value, vry_value);

        if (state_border)
            ssd1306_rect(&display, 3, 3, 122, 58, true, true);
        else
            ssd1306_rect_hearts(&display, 3, 3, 122, 58, true);

        ssd1306_rect(&display, square_x - 4, square_y - 8, SQUARE_SIZE, SQUARE_SIZE, true, true);
        ssd1306_send_data(&display);
        sleep_ms(40);
    }
}
