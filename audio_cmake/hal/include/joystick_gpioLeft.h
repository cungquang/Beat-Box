#ifndef _JOYSTICK_GPIOLEFT_H_
#define _JOYSTICK_GPIOLEFT_H_

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../app/include/audioMixer_helper.h"

int isExported_gpioLeft();
void configure_gpioLeft();
int readValue_gpioLeft();

#endif