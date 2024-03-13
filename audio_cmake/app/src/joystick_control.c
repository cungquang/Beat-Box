#include "../include/joystick_control.h"

#define MAX_BOUNCING 20
#define MAX_PRESS_BOUNCING 70
#define MAX_BUFFER_SIZE 250

static int isTerminate;
static char volumeBuffer[MAX_BUFFER_SIZE];
static char tempoBuffer[MAX_BUFFER_SIZE];

//Press
static int prevPressDir;
static int pressContinue;

//Volume - UP or Down
static int prevVolumeDir;  
static int volumeContinue;

//Tempo - LEFT or RIGHT
static int prevTempoDir;      
static int tempoContinue;

//Mode 
static int mode = 0;

//Thread
static pthread_t pressTrigger_id;
static pthread_t pressExecute_id;
static pthread_t volumeTrigger_id;
static pthread_t volumeExecute_id;
static pthread_t tempoTrigger_id;
static pthread_t tempoExecute_id;

//Mutex
pthread_mutex_t volumeMutex;
pthread_mutex_t tempoMutex;
pthread_mutex_t pressMutex;


//Initiate private function
void* press_trigger_thread();
void* press_execute_thread();

void* volume_up_down_trigger_thread();
void* volume_execute_thread();
int isLeftOrRight(int left, int right);

void* tempo_left_right_trigger_thread();
void* tempo_execute_thread();
int isUpOrDown(int up, int down);



/*
#########################
#       PUBLIC          #
#########################
*/

void JoystickControl_init()
{
    //Trigger flag
    isTerminate = 0;

    //	pthread_mutex_init(&sampler_mutex, NULL);
    pthread_mutex_init(&volumeMutex, NULL);
    pthread_mutex_init(&tempoMutex, NULL);
    pthread_mutex_init(&pressMutex, NULL);


    //Create and start pressTrigger_id
    if(pthread_create(&pressTrigger_id, NULL, press_trigger_thread, NULL) != 0) {
        exit(EXIT_FAILURE);
    }

    //Create and start pressExecute_id;
    if(pthread_create(&pressExecute_id, NULL, press_execute_thread, NULL) != 0) {
        exit(EXIT_FAILURE);
    }

    //Create & start volumeTrigger thread
    if(pthread_create(&volumeTrigger_id, NULL, volume_up_down_trigger_thread, NULL) != 0) {
        exit(EXIT_FAILURE);
    }

    //Create & start volumeExecute thread
    if(pthread_create(&volumeExecute_id, NULL, volume_execute_thread, NULL) != 0) {
        exit(EXIT_FAILURE);
    }

    //Create & start tempoTrigger_id thread
    if(pthread_create(&tempoTrigger_id, NULL, tempo_left_right_trigger_thread, NULL) != 0) {
        exit(EXIT_FAILURE);
    }

    //Create & start tempoExecute_id thread
    if(pthread_create(&tempoExecute_id, NULL, tempo_execute_thread, NULL) != 0) {
        exit(EXIT_FAILURE);
    }

}

void JoystickControl_join()
{
    printf("Start to join\n");
    pthread_join(volumeTrigger_id, NULL);
    pthread_join(tempoTrigger_id, NULL);
    pthread_join(pressTrigger_id, NULL);

    pthread_join(tempoExecute_id, NULL);
    pthread_join(volumeExecute_id, NULL);
    pthread_join(pressExecute_id, NULL);
}

void JoystickControl_cleanup()
{
    AudioMixer_stop();

    pthread_mutex_destroy(&volumeMutex);
    pthread_mutex_destroy(&tempoMutex);
    pthread_mutex_destroy(&pressMutex);

}

void JoystickControl_terminate()
{
    isTerminate = 1;
}

/*
#########################
#       PRIVATE         #
#########################
*/


//////////////////// Press ////////////////////

//Thread is polling to check press trigger
void *press_trigger_thread()
{
    int currPressDir;

    while(isTerminate == 0)
    {
        currPressDir = readValue_gpioPress();

        //Critical section
        pthread_mutex_lock(&pressMutex);

        //Match with previous
        if(prevPressDir == currPressDir)
        {
            pressContinue++;
            pressContinue = pressContinue > MAX_BOUNCING ? MAX_BOUNCING + 1 : pressContinue;
        }
        //Does not match with previous
        else{
            pressContinue = 1;
        }

        //Update press
        prevPressDir = currPressDir;
        pthread_mutex_unlock(&pressMutex);
        sleepForMs(5);
    }

    return NULL;
}

//Thread execute command -> "None" -> "Beat1" -> "None" -> "Beat2"
void* press_execute_thread()
{
    while(isTerminate == 0)
    {
        pthread_mutex_lock(&pressMutex);

        //User press button - continously
        if(pressContinue >= MAX_BOUNCING && prevPressDir == 0)
        {
            //mode == 0
            if(mode == 0) 
            {
                mode = 1;
            } 
            else if (mode == 1)
            {
                mode = 2;
            }
        } 
        //User did not press button continously or does not meet bouncing condition
        else
        {
            mode = 0;
        }

        //Take action based on mode value
        if(mode == 1)
        {
            //Do something
            printf("pressContinue: %d - pressValue: %d - mode: %d\n", pressContinue, prevPressDir, mode);
        }
        else if(mode == 2)
        {
            //Do something
            printf("pressContinue: %d - pressValue: %d - mode: %d]\n", pressContinue, prevPressDir, mode);
        }
        pthread_mutex_unlock(&pressMutex);
        sleepForMs(5);
    }

    return NULL;
}

//////////////////// Volume ////////////////////

//Thread is polling to check Up or Down button trigger
void* volume_up_down_trigger_thread()
{
    int gpioUp;
    int gpioDown;
    int currVolumeDir;

    while(isTerminate == 0)
    {
        gpioUp = readValue_gpioUp();
        gpioDown = readValue_gpioDown();

        //Critical section
        pthread_mutex_lock(&volumeMutex);

        currVolumeDir = isUpOrDown(gpioUp, gpioDown);
        
        //If previous match signal - 2:Up || 3:Down || 5: None trigger
        if(currVolumeDir == prevVolumeDir)
        {
            volumeContinue++;
            //Set max value to avoid overflow
            volumeContinue = volumeContinue >= MAX_BOUNCING? MAX_BOUNCING : volumeContinue;
        }
        //If previous not match 
        else{
            volumeContinue = 1;    
        }

        //Update previous 
        prevVolumeDir = currVolumeDir;
        pthread_mutex_unlock(&volumeMutex);
        sleepForMs(5);
    }

    return NULL;
}

//Thread execute command -> increase/decrease volume + send to server
void* volume_execute_thread()
{
    while(isTerminate == 0)
    {
        pthread_mutex_lock(&volumeMutex);

        //Up => increase the volume
        if(volumeContinue >= MAX_BOUNCING && prevVolumeDir == 2)
        {
            //Increase volume
            AudioMixerControl_setVolume(AudioMixerControl_getVolume() + 5);

            //Send data -> update frontend
            //snprintf(volumeBuffer, MAX_BUFFER_SIZE, "volume,increase,%d", AudioMixerControl_getVolume());
            //printf("change Volume ---> %s\n", volumeBuffer);
            UDP_sendToTarget(volumeBuffer);
        }
        //Down => decrease the volume
        else if(volumeContinue >= MAX_BOUNCING && prevVolumeDir == 3)
        {
            //Decrease volume
            AudioMixerControl_setVolume(AudioMixerControl_getVolume() - 5);

            //Send to Server -> update on frontend
            //snprintf(volumeBuffer, MAX_BUFFER_SIZE, "volume,decrease,%d", AudioMixerControl_getVolume());
            //printf("change Volume ---> %s\n", volumeBuffer);
            UDP_sendToTarget(volumeBuffer);
        }

        pthread_mutex_unlock(&volumeMutex);
        sleepForMs(5);
    }

    return NULL;
}

//////////////////// Tempo ////////////////////

//Thread is polling to check left & right button trigger
void* tempo_left_right_trigger_thread()
{
    int gpioLeft;
    int gpioRight;
    int currTempoDir;

    while(isTerminate == 0)
    {
        gpioLeft = readValue_gpioLeft();
        gpioRight = readValue_gpioRight();

        pthread_mutex_lock(&tempoMutex);

        currTempoDir = isLeftOrRight(gpioLeft, gpioRight);

        //if previous is matched - 2:Left || 3:Right || 5:None
        if(prevTempoDir == currTempoDir)
        {
            tempoContinue++;
            //Avoid overflow
            tempoContinue = tempoContinue >= MAX_BOUNCING ? MAX_BOUNCING : tempoContinue;
        }
        //if previous not match
        else 
        {
            tempoContinue = 1;
        }

        //Update previous
        prevTempoDir = currTempoDir;
        pthread_mutex_unlock(&tempoMutex);

        sleepForMs(5);
    }

    return NULL;
}

//Thread execute command -> increase/decrease tempo + send to server
void* tempo_execute_thread()
{
    while(isTerminate == 0)
    {
        pthread_mutex_lock(&tempoMutex);

        //Left => decrease the tempo
        if(tempoContinue >= MAX_BOUNCING && prevTempoDir == 2)
        {
            //Decrease the tempo
            AudioMixerControl_setTempo(AudioMixerControl_getTempo() - 5);

            //Send to Server -> update on frontend
            //snprintf(tempoBuffer, MAX_BUFFER_SIZE, "tempo,decrease,%d", AudioMixerControl_getTempo());
            //printf("change Tempo ---> %s\n", tempoBuffer);
            UDP_sendToTarget(tempoBuffer);
        }
        //Right => increase the tempo
        else if(tempoContinue >= MAX_BOUNCING && prevTempoDir == 3)
        {
            //Increase the tempo
            AudioMixerControl_setTempo(AudioMixerControl_getTempo() + 5);

            //Send to Server -> update on frontend
            //snprintf(tempoBuffer, MAX_BUFFER_SIZE, "tempo,increase,%d", AudioMixerControl_getTempo());
            //printf("change Tempo ---> %s\n", tempoBuffer);
            UDP_sendToTarget(tempoBuffer);
        }

        pthread_mutex_unlock(&tempoMutex);

        sleepForMs(5);
    }

    return NULL;
}

//////////////////// Support ////////////////////

//Return 2:left or 3:right
int isLeftOrRight(int left, int right)
{
    return left*2 + right*3;    
}

//Return 2:up or 3:down
int isUpOrDown(int up, int down)
{
    return up*2 + down*3;
}