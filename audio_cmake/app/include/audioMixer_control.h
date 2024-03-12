#ifndef _AUDIOMIXER_CONTROL_H_
#define _AUDIOMIXER_CONTROL_H_

#include <unistd.h> 
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <alsa/asoundlib.h>
#include "../include/audioMixer_template.h"

//Setup
void AudioMixerControl_init(int *terminateFlag);
void AudioMixerControl_join(void);
void AudioMixerControl_cleanup(void);

//Beat control - Select or unselect beats
void AudioMixerControl_modifyBeat(int beatIndex, int value);

//Volume control
void AudioMixerControl_setVolume(int newVolume);
int AudioMixerControl_getVolume();

//Tempo control
void AudioMixerControl_setTempo(int newTempo);
int AudioMixerControl_getTempo();

//Drum control - add drum to queue
void AudioMixerControl_AddDrum(int drumIndex);

//User selection control
void AudioMixerControl_setUserSelection(int newSelect);
int AudioMixerControl_getUserSelection();

#endif