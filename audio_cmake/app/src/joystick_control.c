#include "../include/joystick_control.h"

#define MAX_BOUNCING 20
#define MAX_PRESS_BOUNCING 100
#define MAX_BUFFER_SIZE 250

static int *isTerminate;
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
static pthread_mutex_t volumeMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t tempoMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t pressMutex = PTHREAD_MUTEX_INITIALIZER;

//Semaphore
sem_t press_full;
sem_t press_empty;
sem_t volume_full;
sem_t volume_empty;
sem_t tempo_full;
sem_t tempo_empty;


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

void JoystickControl_init(int* terminate_flag)
{
    //Trigger flag
    isTerminate = terminate_flag;

    //Init sempahore
    sem_init(&volume_empty, 0, 1);
    sem_init(&volume_full, 0, 0);
    sem_init(&tempo_empty, 0, 1);
    sem_init(&tempo_full, 0, 0);
    sem_init(&press_empty, 0, 1);
    sem_init(&press_full, 0, 0);

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
    AudioMixer_stop();
    pthread_join(pressTrigger_id, NULL);
    pthread_join(pressExecute_id, NULL);
    pthread_join(volumeTrigger_id, NULL);
    pthread_join(volumeExecute_id, NULL);
    pthread_join(tempoTrigger_id, NULL);
    pthread_join(tempoExecute_id, NULL);
}

void JoystickControl_cleanup()
{
    //End AudioMixer
    AudioMixer_stop();

    //Free semaphore
    sem_destroy(&press_full);
    sem_destroy(&press_empty);
    sem_destroy(&volume_empty);
	sem_destroy(&volume_full);
    sem_destroy(&tempo_empty);
	sem_destroy(&tempo_full);
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

    while(!*isTerminate)
    {
        currPressDir = readValue_gpioPress();

        //Critical section
        sem_wait(&press_empty);
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
        sem_post(&press_full);
    }

    return NULL;
}

//Thread execute command -> "None" -> "Beat1" -> "None" -> "Beat2"
void* press_execute_thread()
{
    while(!*isTerminate)
    {
        sem_wait(&press_full);
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
        sem_post(&press_empty);
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

    while(!*isTerminate)
    {
        gpioUp = readValue_gpioUp();
        gpioDown = readValue_gpioDown();

        //Critical section
        sem_wait(&volume_empty);
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
        sem_post(&volume_full);
    }

    return NULL;
}

//Thread execute command -> increase/decrease volume + send to server
void* volume_execute_thread()
{
    while(!*isTerminate)
    {
        sem_wait(&volume_full);
        pthread_mutex_lock(&volumeMutex);

        //Up => increase the volume
        if(volumeContinue >= MAX_BOUNCING && prevVolumeDir == 2)
        {
            //Increase volume
            AudioMixerControl_setVolume(AudioMixerControl_getVolume() + 5);

            //Send data -> update frontend
            snprintf(volumeBuffer, MAX_BUFFER_SIZE, "volume,increase,%d", AudioMixerControl_getVolume());
            printf("change Volume ---> %s\n", volumeBuffer);
            //UDP_sendToTarget(volumeBuffer);
        }
        //Down => decrease the volume
        else if(volumeContinue >= MAX_BOUNCING && prevVolumeDir == 3)
        {
            //Decrease volume
            AudioMixerControl_setVolume(AudioMixerControl_getVolume() - 5);

            //Send to Server -> update on frontend
            snprintf(volumeBuffer, MAX_BUFFER_SIZE, "volume,decrease,%d", AudioMixerControl_getVolume());
            printf("change Volume ---> %s\n", volumeBuffer);
            //UDP_sendToTarget(volumeBuffer);
        }

        pthread_mutex_unlock(&volumeMutex);
        sem_post(&volume_empty);
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

    while(!*isTerminate)
    {
        gpioLeft = readValue_gpioLeft();
        gpioRight = readValue_gpioRight();

        sem_wait(&tempo_empty);
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
        sem_post(&tempo_full);
    }

    return NULL;
}

//Thread execute command -> increase/decrease tempo + send to server
void* tempo_execute_thread()
{
    while(!*isTerminate)
    {
        sem_wait(&tempo_full);
        pthread_mutex_lock(&tempoMutex);

        //Left => decrease the tempo
        if(tempoContinue >= MAX_BOUNCING && prevTempoDir == 2)
        {
            //Decrease the tempo
            AudioMixerControl_setTempo(AudioMixerControl_getTempo() - 5);

            //Send to Server -> update on frontend
            snprintf(tempoBuffer, MAX_BUFFER_SIZE, "tempo,decrease,%d", AudioMixerControl_getTempo());
            printf("change Tempo ---> %s\n", tempoBuffer);
            //UDP_sendToTarget(tempoBuffer);
        }
        //Right => increase the tempo
        else if(tempoContinue >= MAX_BOUNCING && prevTempoDir == 3)
        {
            //Increase the tempo
            AudioMixerControl_setTempo(AudioMixerControl_getTempo() + 5);

            //Send to Server -> update on frontend
            snprintf(tempoBuffer, MAX_BUFFER_SIZE, "tempo,increase,%d", AudioMixerControl_getTempo());
            printf("change Tempo ---> %s\n", tempoBuffer);
            //UDP_sendToTarget(tempoBuffer);
        }

        pthread_mutex_unlock(&tempoMutex);
        sem_post(&tempo_empty);
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