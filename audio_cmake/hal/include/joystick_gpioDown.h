#ifndef _JOYSTICK_GPIODOWN_H_
#define _JOYSTICK_GPIODOWN_H_

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../app/include/audioMixer_helper.h"

int isExported_gpioDown();
void configure_gpioDown();
int readValue_gpioDown();

#endif