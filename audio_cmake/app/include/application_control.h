#ifndef _APPLICATION_CONTROL_H
#define _APPLICATION_CONTROL_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "../include/audioMixer_helper.h"
#include "../include/audioMixer_upd.h"

void Appliction_control();
void Application_join();
void Application_terminate();
void Application_cleanup();

void readFromProcessUptime();

#endif