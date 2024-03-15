#ifndef _PROCESSTIME_CONTROL_H_
#define _PROCESSTIME_CONTROL_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "./audioMixer_helper.h"
#include "./audioMixer_upd.h"
#include "./audioMixer_template.h"
#include "./i2cbus1_control.h"

void ProcessTime_init(void);
void ProcessTime_join(void);
void ProcessTime_terminate(void);
void ProcessTime_cleanup(void);

#endif