#ifndef _PROCESSTIME_CONTROL_H_
#define _PROCESSTIME_CONTROL_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "../include/audioMixer_helper.h"
#include "../include/audioMixer_upd.h"

void Application_init(void);
void Application_join(void);
void Application_terminate(void);
void Application_cleanup(void);
void AudioMixerControl_hasSound(void);

#endif