#include "../include/audioMixer_control.h"
#include "../include/audioMixer_template.h"
#include "../include/audioMixer_helper.h"

#define MAX_STD_BEAT 3

#define STDBEAT_DRUM "100051__menegass__gui-drum-bd-hard.wav"
#define STDBEAT_HIT_HAT "100053__menegass__gui-drum-cc.wav"
#define STDBEAT_SNARE "100059__menegass__gui-drum-snare-soft.wav"

#define ACCBEAT_CYN "100056__menegass__gui-drum-cyn-hard.wav"
#define ACCBEAT_SNARE "100060__menegass__gui-drum-splash-hard.wav"
#define ACCBEAT_HIT_HAT "100062__menegass__gui-drum-tom-hi-hard.wav"

//Manage operation
static int* isTerminate;

//Wave file
static wavedata_t stdBeat[MAX_STD_BEAT];
static wavedata_t accBeat[MAX_STD_BEAT];

static int selectedBeats[MAX_STD_BEAT];

static pthread_t audioThreadId;
static pthread_mutex_t audioMutex = PTHREAD_MUTEX_INITIALIZER;

//Initiate private function
void* addThemeToQueue_thread();
static void addThemeBeatToQueue();
static int convertTempoIntoTime(int tempo);
static void loadBeatIntoMemory();
static void cleanUpBeatInMemory();


/*
#########################
#        PUBLIC         #
#########################
*/

void AudioMixerControl_init(int *terminateFlag)
{
    //Init trigger flag
    isTerminate = terminateFlag;

    //Load beats into memory
    loadBeatIntoMemory();

    //Init Audio Mixer
    AudioMixer_init();

    // Launch playback thread:
	pthread_create(&audioThreadId, NULL, addThemeToQueue_thread, NULL);
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

void AudioMixerControl_AddSound(int soundIndex)
{
    AudioMixer_queueSound(&accBeat[soundIndex]);
}

void AudioMixerControl_modifyBeat(int beatIndex, int value)
{
    if(beatIndex < 0 || beatIndex > 2)
    {
        printf("ERROR: Unsupported beats.\n");
    }

    pthread_mutex_lock(&audioMutex);
    selectedBeats[beatIndex] = value;
    pthread_mutex_unlock(&audioMutex);
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

/*
#########################
#       PRIVATE         #
#########################
*/

void* addThemeToQueue_thread()
{
    while(!*isTerminate)
    {
        //add to queue
        addThemeBeatToQueue();

        //Sleep based on tempo value
        sleepForMs(convertTempoIntoTime(AudioMixer_getTempo()));
    }

    return NULL;
}

static void addThemeBeatToQueue()
{
    for(int i = 0; i < MAX_STD_BEAT; i++)
    {
        //Critical section
        pthread_mutex_lock(&audioMutex);    
        if(selectedBeats[i] == 1)
        {
            AudioMixer_queueSound(&stdBeat[i]);
        }
        pthread_mutex_unlock(&audioMutex);
        sleepForMs(700);
    }
}

static int convertTempoIntoTime(int tempo) 
{
    float timeToSleep = (60/tempo/2)*1000;
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