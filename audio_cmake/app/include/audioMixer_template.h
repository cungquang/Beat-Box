// Playback sounds in real time, allowing multiple simultaneous wave files
// to be mixed together and played without jitter.
#ifndef _AUDIO_MIXER_H_
#define _AUDIO_MIXER_H_

typedef struct {
	int numSamples;
	short *pData;
} wavedata_t;

#define AUDIOMIXER_MAX_VOLUME 100

// init() must be called before any other functions,
// cleanup() must be called last to stop playback threads and free memory.s
// join() must be called after intiate -> join thread
void AudioMixer_init(void);
void AudioMixer_cleanup(void);
void AudioMixer_join();

// stop() called to stop the current play list
// isSoundBites() check if any sound in the current list
void AudioMixer_stop(void);
int AudioMixer_isSoundBites(void);

// Read the contents of a wave file into the pSound structure. Note that
// the pData pointer in this structure will be dynamically allocated in
// readWaveFileIntoMemory(), and is freed by calling freeWaveFileData().
void AudioMixer_readWaveFileIntoMemory(char *fileName, wavedata_t *pSound);
void AudioMixer_freeWaveFileData(wavedata_t *pSound);

// Queue up another sound bite to play as soon as possible.
void AudioMixer_queueSound(wavedata_t *pSound);

// Get/set the volume.
// setVolume() function posted by StackOverflow user "trenki" at:
// http://stackoverflow.com/questions/6787318/set-alsa-master-volume-from-c-code
int  AudioMixer_getVolume(void);
void AudioMixer_setVolume(int newVolume);

// Get/set the tempo
int AudioMixer_getTempo();
void AudioMixer_setTempo(int newTempo);

//Manually clean up - Queue & playbackBuffer
void AudioMixer_CleanUpQueue();
void AudioMixer_CleanUpBuffer();

//Get statistic
void AudioMixer_getStats(double *minPeriod, double *maxPeriod, double *avgPeriod, long *countPeriod);

#endif