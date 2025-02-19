#ifndef LEDS_H
#define LEDS_H

#include "hardware/gpio.h"
#include "pwm.h"

#define LED_R 13
#define LED_G 11
#define LED_B 12

extern volatile bool state_led; 

void led_init(uint gpio);
void ligaLed(uint gpio);
void desligaLed(uint gpio);
void altera_led(uint gpio, bool status);  // Declaração da função altera_led
void inicializaLEDs();

#endif
