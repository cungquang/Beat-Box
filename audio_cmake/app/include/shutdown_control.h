#ifndef _SHUTDOWN_CONTROL_H_
#define _SHUTDOWN_CONTROL_H_

#include "../include/audioMixer_upd.h"
#include "../include/joystick_control.h"
#include "../include/processTime_control.h"
#include "../include/i2cbus1_control.h"
#include "../include/audioMixer_helper.h"

//Start all the operation
void OperationControl_start(); 

//Terminate all other process
void OperationControl_terminate();


#endif