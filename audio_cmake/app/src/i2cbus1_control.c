#include "../include/i2cbus1_control.h"

#define TRIGGER_BIT 0x27
#define BUFFER_SIZE 2
#define RESOLUTION_12_BITS 4096
#define SELECT_SCALE 2

static int isTerminate = 0;

uint8_t buff_x[BUFFER_SIZE];
uint8_t buff_y[BUFFER_SIZE];
uint8_t buff_z[BUFFER_SIZE];

static int16_t xenH_value;
static int16_t yenH_value;
static int16_t zenH_value;

static pthread_t i2cbus1XenH_id;
static pthread_t i2cbus1YenH_id;
static pthread_t i2cbus1ZenH_id;

// static pthread_mutex_t xenH_mutex = PTHREAD_MUTEX_INITIALIZER;
// static pthread_mutex_t yenH_mutex = PTHREAD_MUTEX_INITIALIZER;
// static pthread_mutex_t zenH_mutex = PTHREAD_MUTEX_INITIALIZER;


//Initiate private function
void* I2cbus1readXenH_thread();
void* I2cbus1readYenH_thread();
void* I2cbus1readZenH_thread();
int16_t I2cbus1_getRawData(int8_t rawL, int8_t rawH);
float I2cbus1_calculateGForce(int16_t rawData);



/*
#########################
#        PUBLIC         #
#########################
*/

void I2cbus1Control_init(void)
{
    I2cbus1_init();
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

void I2cbus1Control_join(void)
{
    pthread_join(i2cbus1XenH_id, NULL);
    pthread_join(i2cbus1YenH_id, NULL);
    pthread_join(i2cbus1ZenH_id, NULL);
}


void I2cbus1Control_cleanup(void)
{
    xenH_value = 0;
    yenH_value = 0;
    zenH_value = 0;
}

void I2cbusControl_terminate(void)
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
        buff_x[0] = I2cbus1Read_OutXL();
        buff_x[1] = I2cbus1Read_OutXH();
        xenH_value = I2cbus1_calculateGForce(I2cbus1_getRawData(buff_x[0], buff_x[1]));
        printf("OUT_X_H ---> %d\n", xenH_value);
    }

    return NULL;
}


void* I2cbus1readYenH_thread()
{
    while(!isTerminate)
    {
        buff_y[0] = I2cbus1Read_OutYL();
        buff_y[1] = I2cbus1Read_OutYH();
        yenH_value = I2cbus1_calculateGForce(I2cbus1_getRawData(buff_y[0], buff_y[1]));
        printf("OUT_Y_H---> %d\n", yenH_value);
    }

    return NULL;
}

void* I2cbus1readZenH_thread()
{
    while(!isTerminate)
    {
        buff_z[0] = I2cbus1Read_OutZL();
        buff_z[1] = I2cbus1Read_OutZH();
        zenH_value = I2cbus1_calculateGForce(I2cbus1_getRawData(buff_z[0], buff_z[1]));
        printf("OUT_Y_H---> %d\n", zenH_value);
    }

    return NULL;
}

int16_t I2cbus1_getRawData(int8_t rawL, int8_t rawH)
{
    return (rawH << 8) | (rawL);
}

float I2cbus1_calculateGForce(int16_t rawData)
{
    return (float)rawData * SELECT_SCALE/RESOLUTION_12_BITS;
}
