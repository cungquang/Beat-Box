#include "../include/audioMixer_control.h"
#include "../include/audioMixer_template.h"
#include "../include/audioMixer_helper.h"

#define MAX_STD_BEAT 3

#define STDBEAT_DRUM "/mnt/remote/myApps/beatbox-wave-files/100051__menegass__gui-drum-bd-hard.wav"
#define STDBEAT_HIT_HAT "/mnt/remote/myApps/beatbox-wave-files/100053__menegass__gui-drum-cc.wav"
#define STDBEAT_SNARE "/mnt/remote/myApps/beatbox-wave-files/100059__menegass__gui-drum-snare-soft.wav"

#define ACCBEAT_CYN "/mnt/remote/myApps/beatbox-wave-files/100056__menegass__gui-drum-cyn-hard.wav"
#define ACCBEAT_SNARE "/mnt/remote/myApps/beatbox-wave-files/100060__menegass__gui-drum-splash-hard.wav"
#define ACCBEAT_HIT_HAT "/mnt/remote/myApps/beatbox-wave-files/100062__menegass__gui-drum-tom-hi-hard.wav"

//Manage operation
static int isTerminate;

//Wave file
static wavedata_t stdBeat[MAX_STD_BEAT];
static wavedata_t accBeat[MAX_STD_BEAT];

static int selectedBeat = 0;

static pthread_t audioThreadId;
static pthread_mutex_t audioMutex = PTHREAD_MUTEX_INITIALIZER;

//Initiate private function
void* addThemeToQueue_thread();
static int convertTempoIntoTime(int tempo);
static void loadBeatIntoMemory();
static void cleanUpBeatInMemory();

//Music theme
static void playback_stdRockBeat();
static void playback_customBeats();


/*
#########################
#        PUBLIC         #
#########################
*/

void AudioMixerControl_init()
{
    //Init trigger flag
    isTerminate = 0;

    //Init Audio Mixer
    AudioMixer_init();

    //Load beats into memory
    loadBeatIntoMemory();

    // Launch playback thread:
	if(pthread_create(&audioThreadId, NULL, addThemeToQueue_thread, NULL) != 0)
    {
        exit(EXIT_FAILURE);
    }
}

void AudioMixerControl_join(void)
{
    //Join the thread
    pthread_join(audioThreadId, NULL);
}

void AudioMixerControl_cleanup(void)
{
    //Free other service
    AudioMixer_cleanup();

    //Clean up beat
    cleanUpBeatInMemory();
}

void AudioMixerControl_addDrum(int soundIndex)
{
    AudioMixer_queueSound(&accBeat[soundIndex]);
}

void AudioMixerControl_terminate() 
{
    isTerminate = 1;
}

/////////////////// Sound Control ///////////////////

void AudioMixerControl_setVolume(int newVolume)
{
    AudioMixer_setVolume(newVolume);
}

int AudioMixerControl_getVolume()
{
    return AudioMixer_getVolume();
}

/////////////////// Volume Control ///////////////////

void AudioMixerControl_setTempo(int newTempo)
{
    AudioMixer_setTempo(newTempo);
}

int AudioMixerControl_getTempo()
{
    return AudioMixer_getTempo();
}

/////////////////// User Selection ///////////////////


void AudioMixerControl_controlBeat(int beatIndex)
{
    if(beatIndex < 0 || beatIndex > 2)
    {
        printf("ERROR: Unsupported beats.\n");
    }

    pthread_mutex_lock(&audioMutex);
    selectedBeat = beatIndex;
    pthread_mutex_unlock(&audioMutex);
}


/*
#########################
#       PRIVATE         #
#########################
*/

void* addThemeToQueue_thread()
{
    int getSelectedBeat;
    while(!isTerminate)
    {
        pthread_mutex_lock(&audioMutex);
        getSelectedBeat = selectedBeat;
        pthread_mutex_unlock(&audioMutex);

        //Play standard rock beat
        if(getSelectedBeat == 1)
        {
            playback_stdRockBeat();
        }
        //Play custom beat
        else if(getSelectedBeat == 2)
        {
            playback_customBeats();
        }
        //Play None
        else
        {
            AudioMixer_CleanUpQueue();
            AudioMixer_CleanUpBuffer();
        }
        
    }

    return NULL;
}

static int convertTempoIntoTime(int tempo) 
{
    float timeToSleep = ((60.0f / tempo) / 2.0f) * 1000.0f;
    return (int)timeToSleep;
}

static void loadBeatIntoMemory()
{
    //Load 3 standard beat mode
	AudioMixer_readWaveFileIntoMemory(STDBEAT_DRUM, &stdBeat[0]);
    AudioMixer_readWaveFileIntoMemory(STDBEAT_HIT_HAT, &stdBeat[1]);
    AudioMixer_readWaveFileIntoMemory(STDBEAT_SNARE, &stdBeat[2]);

    //Load 3 beat for accelerometer
    AudioMixer_readWaveFileIntoMemory(ACCBEAT_CYN, &accBeat[0]);                //0 - Z axis - Drum
    AudioMixer_readWaveFileIntoMemory(ACCBEAT_SNARE, &accBeat[1]);              //1 - X axis - Snare
    AudioMixer_readWaveFileIntoMemory(ACCBEAT_HIT_HAT, &accBeat[2]);            //2 - Y axis - Hit-hat 
}

static void cleanUpBeatInMemory()
{
    //Free standard beat
    AudioMixer_freeWaveFileData(&stdBeat[0]);
    AudioMixer_freeWaveFileData(&stdBeat[1]);
    AudioMixer_freeWaveFileData(&stdBeat[2]);

    //Free beat for accelerometer
    AudioMixer_freeWaveFileData(&accBeat[0]);
    AudioMixer_freeWaveFileData(&accBeat[1]);
    AudioMixer_freeWaveFileData(&accBeat[2]);
}


/////////////////////////// MUSIC THEME /////////////////////////// 

//Standard Rock Beat
static void playback_stdRockBeat()
{
    //beat 1 - Hit-hat, base
    AudioMixer_queueSound(&stdBeat[1]);
    AudioMixer_queueSound(&stdBeat[0]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));

    //beat 1 - Hit-hat
    AudioMixer_queueSound(&stdBeat[1]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));

    //beat 2 - Hit-hat, snare
    AudioMixer_queueSound(&stdBeat[1]);
    AudioMixer_queueSound(&stdBeat[2]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));

    //beat 2.5 - Hit-hat
    AudioMixer_queueSound(&stdBeat[1]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));

    //beat 3 - Hit-hat, base
    AudioMixer_queueSound(&stdBeat[1]);
    AudioMixer_queueSound(&stdBeat[0]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));

    //beat 3.5 - Hit-hat
    AudioMixer_queueSound(&stdBeat[1]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));

    //beat 4 - Hit-hat, snare
    AudioMixer_queueSound(&stdBeat[1]);
    AudioMixer_queueSound(&stdBeat[0]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));

    //beat 4.5 - Hit hat
    AudioMixer_queueSound(&stdBeat[1]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));
}

//Standard Rock Beat
static void playback_customBeats()
{
    //beat 1 - base
    AudioMixer_queueSound(&stdBeat[1]);
    AudioMixer_queueSound(&stdBeat[0]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));

    //beat 1.5 - Hit-hat
    AudioMixer_queueSound(&stdBeat[0]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));

    //beat 2 - Hit-hat, snare
    AudioMixer_queueSound(&stdBeat[1]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));

    //beat 2.5 - Hit-hat
    AudioMixer_queueSound(&stdBeat[0]);
    AudioMixer_queueSound(&stdBeat[2]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));

    //beat 3
    AudioMixer_queueSound(&stdBeat[1]);
    AudioMixer_queueSound(&stdBeat[0]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));

    //beat 3.5
    AudioMixer_queueSound(&stdBeat[0]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));

    //beat 4
    AudioMixer_queueSound(&stdBeat[1]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));

    //beat 4.5
    AudioMixer_queueSound(&stdBeat[0]);
    AudioMixer_queueSound(&stdBeat[2]);
    sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));
}