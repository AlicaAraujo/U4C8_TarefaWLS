#include "pwm.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

// Função para configurar o PWM no pino especificado
void pwm_led_setup(uint gpio, uint16_t *level) {
    // Configura o pino como função PWM
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    // Obtém o slice de PWM correspondente ao pino
    uint slice = pwm_gpio_to_slice_num(gpio);

    // Configura o valor máximo de contagem (4095 para PWM de 12 bits)
    pwm_set_wrap(slice, 4095);

    // Inicializa o PWM com o valor de duty cycle fornecido
    pwm_set_gpio_level(gpio, *level);

    // Habilita o PWM
    pwm_set_enabled(slice, true);
}

// Função para alterar o nível de PWM de acordo com o duty cycle desejado
void altera_pwm(uint gpio, uint16_t duty) {
    // Altera o nível de PWM diretamente para o pino especificado
    pwm_set_gpio_level(gpio, duty);
}
