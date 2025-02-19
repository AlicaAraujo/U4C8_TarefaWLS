#include "leds.h"
#include "pwm.h"
#include "hardware/gpio.h"

volatile bool state_led;

void led_init(uint gpio) {
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_OUT);
}

void ligaLed(uint gpio) { 
    gpio_put(gpio, 1); 
}

void desligaLed(uint gpio) {
    gpio_put(gpio, 0); 
}

void altera_led(uint gpio, bool status) {  // Função que manipula o LED de acordo com o status
    if (status) {
        gpio_put(gpio, 1);  // Liga o LED
    } else {
        gpio_put(gpio, 0);  // Desliga o LED
    }
}

void inicializaLEDs() {
    // Inicializando os pinos dos LEDs
    led_init(LED_R);
    led_init(LED_G);
    led_init(LED_B);
}
