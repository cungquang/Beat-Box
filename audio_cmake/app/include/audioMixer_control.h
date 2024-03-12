#ifndef _AUDIOMIXER_CONTROL_H_
#define _AUDIOMIXER_CONTROL_H_

#include <unistd.h> 
#include <stdbool.h>
#include <pthread.h>
#include <alsa/asoundlib.h>
#include "../include/audioMixer_template.h"

void AudioMixerControl_init(int *terminateFlag);
void AudioMixerControl_join(void);
void AudioMixerControl_cleanup(void);
void AudioMixerControl_AddSound(int indexSound);

#endif