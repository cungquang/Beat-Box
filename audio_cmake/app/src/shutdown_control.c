#include "../include/shutdown_control.h"

static int isTimeToCleanup = 0;

//thread
static pthread_t shutdown_id;

//Initiate private function
static void Shutdown_init(void);
static void Shutdown_join(void);
void *shutdown_thread();
static void Shutdown_cleanup(void);


/*
#############################
#           PUBLIC          #
#############################
*/

//Start the entire application
void OperationControl_start(void) 
{
    Shutdown_init();
    UDP_initServer();
    ProcessTime_init();
    AudioMixerControl_init();
    JoystickControl_init();
    I2cbus1Control_init();

    ProcessTime_join();
    I2cbus1Control_join();
    JoystickControl_join();
    AudioMixerControl_join();
    UDP_join();

    //Trigger -> cleanup process
    isTimeToCleanup = 1;
    Shutdown_join();
}

//Terminate all other processes
void OperationControl_terminate(void)
{
    AudioMixer_stop();
    JoystickControl_terminate();
    AudioMixerControl_terminate();
    ProcessTime_terminate();
    I2cbusControl_terminate();
}



/*
#############################
#          PRIVATE          #
#############################
*/


//Init shutdown
static void Shutdown_init(void)
{
    //Run server thread
    pthread_create(&shutdown_id, NULL, shutdown_thread, NULL);
}

//Join shutdown thread
static void Shutdown_join(void)
{
    pthread_join(shutdown_id, NULL);
}

//Shutdown thread 
void *shutdown_thread()
{
    //Sleep until other process finish 
    while(!isTimeToCleanup)
    {
        sleepForMs(150);
    }

    //Clean up start
    Shutdown_cleanup();

    return NULL;
}

//Cleanup other process
static void Shutdown_cleanup(void)
{
    //Cleanup all other threads
    UDP_cleanup();
    AudioMixerControl_cleanup();
    JoystickControl_cleanup();
    ProcessTime_cleanup();
    I2cbus1Control_cleanup();
}