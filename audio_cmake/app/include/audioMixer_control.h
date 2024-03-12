#ifndef _AUDIOMIXER_CONTROL_H_
#define _AUDIOMIXER_CONTROL_H_

#include <unistd.h> 
#include <stdbool.h>
#include <pthread.h>
#include <alsa/asoundlib.h>
#include "../include/audioMixer_template.h"

//Setup
void AudioMixerControl_init(int *terminateFlag);
void AudioMixerControl_join(void);
void AudioMixerControl_cleanup(void);

//Drum control - add random drum to queue
void AudioMixerControl_AddSound(int soundIndex);

//Beat control - Select or unselect beats
void AudioMixerControl_modifyBeat(int beatIndex, int value);

//Sound control
void AudioMixerControl_setVolume(int newVolume);
int AudioMixerControl_getVolume();

//Tempo control
void AudioMixerControl_setTempo(int newTempo);
int AudioMixerControl_getTempo();


#endif