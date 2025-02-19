#ifndef BOTAO_H
#define BOTAO_H

#include "pico/stdlib.h"
#include "leds.h"
#include "ssd1306.h"

// Definições dos pinos de botões
#define BOTAO_A 5       // Pino para BOTAO_A
#define JOYSTICK_PB 22  // Pino para o botão do joystick 

extern volatile bool state_led;     // Variável para controlar o estado do LED
extern volatile bool state_border;  // Variável para controlar o estado da borda
extern ssd1306_t display;

void botao_init(void);
void button_irq_handler(uint gpio, uint32_t events);

#endif // BOTAO_H