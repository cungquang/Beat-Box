#ifndef _JOYSTICK_CONTROL_H_
#define _JOYSTICK_CONTROL_H_

#include <unistd.h> 
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include "../../hal/include/joystick_gpioPress.h"
#include "../../hal/include/joystick_gpioUp.h"
#include "../../hal/include/joystick_gpioDown.h"
#include "../../hal/include/joystick_gpioLeft.h"
#include "../../hal/include/joystick_gpioRight.h"
#include "../include/audioMixer_control.h"
#include "../include/audioMixer_upd.h"

void JoystickControl_init();
void JoystickControl_join();

void JoystickControl_cleanup();
void JoystickControl_setTerminateFlag();

#endif