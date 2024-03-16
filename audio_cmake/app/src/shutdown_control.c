#include "../include/shutdown_control.h"

static int isTerminate = 0;
static int isTimeToCleanup = 0;

static pthread_t shutdown_id;

/*
#############################
#           PUBLIC          #
#############################
*/

//Start the entire application
void OperationControl_start() 
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
void OperationControl_terminate()
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
void Shutdown_init()
{
    //Run server thread
    pthread_create(&shutdown_id, NULL, shutdown_program, NULL);
}

//Join shutdown thread
void Shutdown_join()
{
    pthread_join(shutdown_id, NULL);
}

//Shutdown thread 
void *shutdown_program()
{
    //Sleep until other process finish 
    while(!isTimeToCleanup)
    {
        sleepForMs(150);
    }

    //Clean up start
    Shutdown_cleanup();
}

//Cleanup other process
void Shutdown_cleanup()
{
    //Cleanup all other threads
    UDP_cleanup();
    AudioMixerControl_cleanup();
    JoystickControl_cleanup();
    ProcessTime_cleanup();
    I2cbus1Control_cleanup();
}