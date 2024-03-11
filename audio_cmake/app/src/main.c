#include <stdio.h>
#include <unistd.h> // For sleep function
#include <alsa/asoundlib.h>
#include "audioMixer_template.h"

#define SOURCE_FILE_1 "wave-files/100060__menegass__gui-drum-splash-hard.wav"
#define SOURCE_FILE_2 "wave-files/100053__menegass__gui-drum-cc.wav"
#define SOURCE_FILE_3 "wave-files/100054__menegass__gui-drum-ch.wav"
#define SOURCE_FILE_4 "wave-files/100055__menegass__gui-drum-co.wav"

int main() {
    AudioMixer_init();

    // Load wave file we want to play:
	wavedata_t sampleFile_1;
	wavedata_t sampleFile_2;

	AudioMixer_readWaveFileIntoMemory(SOURCE_FILE_1, &sampleFile_1);
	AudioMixer_readWaveFileIntoMemory(SOURCE_FILE_2, &sampleFile_2);

    AudioMixer_queueSound(&sampleFile_1);
    AudioMixer_queueSound(&sampleFile_2);

    //Before write
    printSoundBites();

    //Write to buffer
    testPlaybackBuffer();
    //printPlaybackBuffer();

    //Free other service
    AudioMixer_cleanup();
    
    //Free file after complete
    AudioMixer_freeWaveFileData(&sampleFile_1);
    AudioMixer_freeWaveFileData(&sampleFile_2);

    return 0;
}
