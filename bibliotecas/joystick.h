#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "hardware/adc.h"

void setup_joystick();
void joystick_read_axis(uint16_t *vrx, uint16_t *vry);

#endif