#include "../include/joystick_control.h"

#define MAX_BOUNCING 5

static int *isTerminate;

//Volume - UP or Down
static int prevVolumeDir;
static int volumeBounce;                
static int volumeContinue;

//Tempo - LEFT or RIGHT
static int prevTempoDir;
static int tempoBounce;                 
static int tempoContinue;

static pthread_t bounceRightLeft_id;
static pthread_t bounceUpDown_id;

//Mutex
static pthread_mutex_t volumeMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t tempoMutex = PTHREAD_MUTEX_INITIALIZER;

//Semaphore
sem_t volume_full;
sem_t volume_empty;
sem_t tempo_full;
sem_t tempo_empty;

//Initiate private function

void* volume_up_down_thread();
void* volume_execute_thread();
int isLeftOrRight(int left, int right);

void* tempo_left_right_thread();
void* tempo_execute_thread();
int isUpOrDown(int up, int down);


void JoystickControl_init()
{

}

void JoystickControl_cleanup()
{

}

//////////////////// Volume ////////////////////

//Thread is polling to check Up or Down button trigger
void* volume_up_down_thread()
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

        prevVolumeDir = currVolumeDir;

        pthread_mutex_unlock(&volumeMutex);
        sem_post(&volume_full);
    }
}

//Thread execute command -> increase/decrease volume + send to server
void* volume_execute_thread()
{
    while(!*isTerminate)
    {
        sem_wait(&volume_full);
        pthread_mutex_lock(&volumeMutex);

        //Up => increase the volume
        if(volumeContinue >= 3 && prevVolumeDir == 2)
        {
            //Increase volume
            AudioMixerControl_setVolume(AudioMixerControl_getVolume()+1);

            //Send data -> update frontend
        }
        //Down => decrease the volume
        else if(volumeContinue >= 3 && prevVolumeDir == 3)
        {
            //Decrease volume
            AudioMixerControl_setVolume(AudioMixerControl_getVolume()-1);

            //Send data -> update frontend
        }

        pthread_mutex_unlock(&volumeMutex);
        sem_post(&volume_empty);
    }
}

//////////////////// Tempo ////////////////////

//Thread is polling to check left & right button trigger
void* tempo_left_right_thread()
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
        prevTempoDir = currTempoDir;
        
        pthread_mutex_unlock(&tempoMutex);
        sem_post(&tempo_full);
    }
}

//Thread execute command -> increase/decrease tempo + send to server
void* tempo_execute_thread()
{
    while(!*isTerminate)
    {
        sem_wait(&tempo_full);
        pthread_mutex_lock(&tempoMutex);

        //Left => decrease the tempo
        if(tempoContinue >= 3 && prevTempoDir == 2)
        {
            //Decrease the tempo
            AudioMixerControl_setTempo(AudioMixerControl_getTempo() - 1);

            //Send to Server -> update on frontend
        }
        //Right => increase the tempo
        else if(tempoContinue >= 3 && prevTempoDir == 3)
        {
            //Increase the tempo
            AudioMixerControl_setTempo(AudioMixerControl_getTempo() + 1);

            //Send to Server -> update on frontend

        }

        pthread_mutex_unlock(&tempoMutex);
        sem_post(&tempo_empty);
    }
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