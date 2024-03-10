/*
 *  Small program to read a 16-bit, signed, 44.1kHz wave file and play it.
 *  Written by Brian Fraser, heavily based on code found at:
 *  http://www.alsa-project.org/alsa-doc/alsa-lib/_2test_2pcm_min_8c-example.html
 */

#include <alsa/asoundlib.h>

// File used for play-back:
// If cross-compiling, must have this file available, via this relative path,
// on the target when the application is run. This example's Makefile copies the wave-files/
// folder along with the executable to ensure both are present.
#define SOURCE_FILE "wave-files/100060__menegass__gui-drum-splash-hard.wav"
//#define SOURCE_FILE "wave-files/100053__menegass__gui-drum-cc.wav"

#define SAMPLE_RATE   44100
#define NUM_CHANNELS  1
#define SAMPLE_SIZE   (sizeof(short)) 	// bytes per sample

// Store data of a single wave file read into memory.
// Space is dynamically allocated; must be freed correctly!
typedef struct {
	int numSamples;
	short *pData;
} wavedata_t;

// Prototypes:
snd_pcm_t *Audio_openDevice();
void Audio_readWaveFileIntoMemory(char *fileName, wavedata_t *pWaveStruct);
void Audio_playFile(snd_pcm_t *handle, wavedata_t *pWaveData);



int main(void)
{
	printf("Beginning play-back of %s\n", SOURCE_FILE);

	// Configure Output Device
	snd_pcm_t *handle = Audio_openDevice();

	// Load wave file we want to play:
	wavedata_t sampleFile;
	Audio_readWaveFileIntoMemory(SOURCE_FILE, &sampleFile);

	// Play Audio
	Audio_playFile(handle, &sampleFile);
//	Audio_playFile(handle, &sampleFile);
//	Audio_playFile(handle, &sampleFile);

	// Cleanup, letting the music in buffer play out (drain), then close and free.
	snd_pcm_drain(handle);
	snd_pcm_hw_free(handle);
	snd_pcm_close(handle);
	free(sampleFile.pData);

	printf("Done!\n");
	return 0;
}


