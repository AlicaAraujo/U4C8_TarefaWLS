#include "joystick.h"

void setup_joystick() {
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
}
void joystick_read_axis(uint16_t *vrx, uint16_t *vry) {
    adc_select_input(0);
    *vry = adc_read();
    adc_select_input(1);
    *vrx = adc_read();
}