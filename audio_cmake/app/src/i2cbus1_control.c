#include "../include/i2cbus1_control.h"

#define TRIGGER_BIT 0x27

static int xenH_value;
static int yenH_value;
static int zenH_value;

static pthread_t i2cbus1XenH_id;
static pthread_t i2cbus1YenH_id;
static pthread_t i2cbus1ZenH_id;

static pthread_mutex_t xenH_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t yenH_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t zenH_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
#########################
#        PUBLIC         #
#########################
*/

void I2cbus1Control_init()
{
    I2cbus1_init();
    I2cbus1Write_Reg1(TRIGGER_BIT);
    if(I2cbus1Read_Reg1() != TRIGGER_BIT) 
    {
        printf("ERROR: fail to switch power mode and enable sensor.\n");
        exit(EXIT_FAILURE);
    }

    //Initiate pthread

}





/*
#########################
#       PRIVATE         #
#########################
*/

