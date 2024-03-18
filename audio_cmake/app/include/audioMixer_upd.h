#ifndef _AUDIOMIXER_UDP_H_
#define _AUDIOMIXER_UDP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "./audioMixer_control.h"
#include "./shutdown_control.h"
// #include "../include/joystick_control.h"
// #include "../include/processTime_control.h"
// #include "../include/i2cbus1_control.h"

// This module manages UDP server (application level)
// - Initiate the thread to run server - allow user to remotely control the program
// - Join & clean up the program after complete - free all resources after use

// Thread
void UDP_initServer();

//Clean up
void UDP_cleanup(void);

//Join
void UDP_join(void);

//Send UDP message
void UDP_sendToTarget(char *message);


#endif