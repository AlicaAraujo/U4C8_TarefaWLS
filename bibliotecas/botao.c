#include "botao.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "leds.h"
#include "ssd1306.h"

// Variáveis globais de estado
volatile bool state_led_g = false;
extern absolute_time_t debounce;

void botao_init(void) {
    // Inicializa os pinos dos botões
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);  // Pull-up ativado

    gpio_init(JOYSTICK_PB);
    gpio_set_dir(JOYSTICK_PB, GPIO_IN);
    gpio_pull_up(JOYSTICK_PB);
}

// Função de callback para tratar interrupções dos botão A e do botão do joystick
void button_irq_handler(uint gpio, uint32_t events) {
    if (time_reached(debounce)) { // Verifica se o tempo de debounce já passou
        if (gpio == JOYSTICK_PB) {
            state_border = !state_border;
            gpio_put(LED_G, !gpio_get(LED_G)); // Alterna o LED verde
        }
        else if (gpio == BOTAO_A) {
            state_led = !state_led;
        }
        debounce = delayed_by_ms(get_absolute_time(), 200); // Atualiza o debounce
    }
}
