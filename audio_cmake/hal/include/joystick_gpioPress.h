#ifndef _JOYSTICK_GPIOPRESS_H_
#define _JOYSTICK_GPIOPRESS_H_

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../app/include/audioMixer_helper.h"

int isExported_gpioPress();
void configure_gpioPress();
int readValue_gpioPress();

#endif