#include <unistd.h> 
#include <alsa/asoundlib.h>
#include "../include/audioMixer_template.h"

#define SOURCE_FILE_1 "wave-files/100060__menegass__gui-drum-splash-hard.wav"
#define SOURCE_FILE_2 "wave-files/100066__menegass__gui-drum-tom-mid-hard.wav"
#define SOURCE_FILE_3 "wave-files/100062__menegass__gui-drum-tom-hi-hard.wav"
#define SOURCE_FILE_4 "wave-files/100055__menegass__gui-drum-co.wav"

void testOperation()
{
       // Load wave file we want to play:
	wavedata_t sampleFile_1;
	wavedata_t sampleFile_2;
    wavedata_t sampleFile_3;
    wavedata_t sampleFile_4;

	AudioMixer_readWaveFileIntoMemory(SOURCE_FILE_1, &sampleFile_1);
	AudioMixer_readWaveFileIntoMemory(SOURCE_FILE_2, &sampleFile_2);
    AudioMixer_readWaveFileIntoMemory(SOURCE_FILE_3, &sampleFile_3);
    AudioMixer_readWaveFileIntoMemory(SOURCE_FILE_4, &sampleFile_4);

    AudioMixer_init();

    //add to queue

    while(1) {
        AudioMixer_queueSound(&sampleFile_1);
        sleep(0.2);
        AudioMixer_queueSound(&sampleFile_2);
        sleep(0.5);
        AudioMixer_queueSound(&sampleFile_3);
        //AudioMixer_queueSound(&sampleFile_4);
        sleep(1);
    }

    //Free other service
    AudioMixer_cleanup();
    
    //Free file after complete
    AudioMixer_freeWaveFileData(&sampleFile_1);
    AudioMixer_freeWaveFileData(&sampleFile_2);
    AudioMixer_freeWaveFileData(&sampleFile_3);
    AudioMixer_freeWaveFileData(&sampleFile_4);
}