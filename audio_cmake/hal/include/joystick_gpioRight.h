#ifndef _JOYSTICK_GPIORIGHT_H_
#define _JOYSTICK_GPIORIGHT_H_

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/hal_helper.h"

int isExported_gpioRight();
void configure_gpioRight();
int readValue_gpioRight();

#endif