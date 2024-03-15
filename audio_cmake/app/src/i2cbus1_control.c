#include "../include/i2cbus1_control.h"

#define TRIGGER_BIT 0x27

static int isTerminate = 0;

static int xenH_value;
static int yenH_value;
static int zenH_value;

static pthread_t i2cbus1XenH_id;
static pthread_t i2cbus1YenH_id;
static pthread_t i2cbus1ZenH_id;

static pthread_mutex_t xenH_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t yenH_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t zenH_mutex = PTHREAD_MUTEX_INITIALIZER;


//Initiate private function
void* I2cbus1readXenH_thread();
void* I2cbus1readYenH_thread();
void* I2cbus1readZenH_thread();



/*
#########################
#        PUBLIC         #
#########################
*/

void I2cbus1Control_init()
{
    I2cbus1Control_init();
    I2cbus1Write_Reg1(TRIGGER_BIT);
    if(I2cbus1Read_Reg1() != TRIGGER_BIT) 
    {
        printf("ERROR: fail to switch power mode and enable sensor.\n");
        exit(EXIT_FAILURE);
    }

    //Initiate pthread
    pthread_create(&i2cbus1XenH_id, NULL, I2cbus1readXenH_thread, NULL);

    pthread_create(&i2cbus1YenH_id, NULL, I2cbus1readYenH_thread, NULL);

    pthread_create(&i2cbus1ZenH_id, NULL, I2cbus1readZenH_thread, NULL);

}

void I2cbus1Control_join()
{
    pthread_join(&i2cbus1XenH_id, NULL);
    pthread_join(&i2cbus1YenH_id, NULL);
    pthread_join(&i2cbus1ZenH_id, NULL);
}


void I2cbus1Control_cleanup()
{
    xenH_value = 0;
    yenH_value = 0;
    zenH_value = 0;
}

void I2cbusControl_setTerminate()
{
    isTerminate = 1;
}



/*
#########################
#       PRIVATE         #
#########################
*/


void* I2cbus1readXenH_thread()
{
    while(!isTerminate)
    {

    }

    return NULL;
}


void* I2cbus1readYenH_thread()
{
    while(!isTerminate)
    {
        
    }

    return NULL;
}

void* I2cbus1readZenH_thread()
{
    while(!isTerminate)
    {
        
    }

    return NULL;
}