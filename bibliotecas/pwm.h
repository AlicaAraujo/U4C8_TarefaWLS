#ifndef PWM_H
#define PWM_H

#include "hardware/pwm.h"
#include "hardware/gpio.h"

// Função para inicializar o PWM em um pino específico
void pwm_led_setup(uint gpio, uint16_t *level);

// Função para alterar o nível do PWM
void altera_pwm(uint gpio, uint16_t duty);

#endif
