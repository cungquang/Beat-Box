#include "../include/i2cbus1_control.h"

#define TRIGGER_BIT 0x27
#define BUFFER_SIZE 2
#define RESOLUTION_8BITS_SHIFT 16000
#define SELECT_SCALE 2

//Operation
static int isTerminate = 0;

unsigned char xen_L_H[BUFFER_SIZE];
unsigned char yen_L_H[BUFFER_SIZE];
unsigned char zen_L_H[BUFFER_SIZE];

static int16_t xenH_curr;
static int16_t yenH_curr;
static int16_t zenH_curr;

//Threads
static pthread_t i2cbus1XenH_id;
static pthread_t i2cbus1YenH_id;
static pthread_t i2cbus1ZenH_id;

//Mutex
static pthread_mutex_t xenH_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t yenH_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t zenH_mutex = PTHREAD_MUTEX_INITIALIZER;


//Initiate private function
void* I2cbus1readXenH_thread();
void* I2cbus1readYenH_thread();
void* I2cbus1readZenH_thread();
int16_t I2cbus1_getRawData(int8_t rawL, int8_t rawH);
float I2cbus1_convertToGForce(int16_t rawData);



/*
#########################
#        PUBLIC         #
#########################
*/

void I2cbus1Control_init(void)
{
    //Configure bus & register
    I2cbus1_init();
    I2cbus1Write_Reg1(TRIGGER_BIT);

    if(I2cbus1Read_Reg1() != TRIGGER_BIT) 
    {
        printf("ERROR: fail to switch power mode and enable sensor.\n");
        exit(EXIT_FAILURE);
    }

    
    pthread_create(&i2cbus1YenH_id, NULL, I2cbus1readYenH_thread, NULL);
    pthread_create(&i2cbus1XenH_id, NULL, I2cbus1readXenH_thread, NULL);
}

void baseZ(void)
{
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
    xenH_curr = 0;
    yenH_curr = 0;
    zenH_curr = 0;

    memset(xen_L_H, 0, sizeof(xen_L_H));
    memset(yen_L_H, 0, sizeof(yen_L_H));
    memset(zen_L_H, 0, sizeof(zen_L_H));
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
        pthread_mutex_lock(&xenH_mutex);

        //Convert raw to G force value
        xen_L_H[0] = I2cbus1Read_OutXL();
        xen_L_H[1] = I2cbus1Read_OutXH();
        xenH_curr = I2cbus1_convertToGForce(I2cbus1_getRawData(xen_L_H[0], xen_L_H[1]));
        
        //Trigger the sound
        if(xenH_curr >= 2)
        {
            AudioMixerControl_addDrum(0);
            sleepForMs(700);
        } else if(xenH_curr <= -2)
        {
            AudioMixerControl_addDrum(0);
            sleepForMs(700);
        }

        pthread_mutex_unlock(&xenH_mutex);
    }

    return NULL;
}


void* I2cbus1readYenH_thread()
{
    while(!isTerminate)
    {
        pthread_mutex_lock(&yenH_mutex);

        //Convert raw to G force value
        yen_L_H[0] = I2cbus1Read_OutYL();
        yen_L_H[1] = I2cbus1Read_OutYH();
        yenH_curr = I2cbus1_convertToGForce(I2cbus1_getRawData(yen_L_H[0], yen_L_H[1]));

        //Trigger the sound
        if(yenH_curr >= 2)
        {
            AudioMixerControl_addDrum(1);
            sleepForMs(700);
        } else if(yenH_curr <= -2)
        {
            AudioMixerControl_addDrum(1);
            sleepForMs(700);
        }
        
        pthread_mutex_unlock(&yenH_mutex);
    }

    return NULL;
}

void* I2cbus1readZenH_thread()
{
    while(!isTerminate)
    {
        pthread_mutex_lock(&zenH_mutex);

         //Mark statistic event
        Period_markEvent(PERIOD_EVENT_ACCELEROMETER);

        //Convert raw data to G force
        zen_L_H[0] = I2cbus1Read_OutZL();
        zen_L_H[1] = I2cbus1Read_OutZH();
        zenH_curr = I2cbus1_convertToGForce(I2cbus1_getRawData(zen_L_H[0], zen_L_H[1]));

        //Trigger the sound
        if(zenH_curr >= 2)
        {
            AudioMixerControl_addDrum(2);
            sleepForMs(700);
        } else if(zenH_curr <= -2)
        {
            AudioMixerControl_addDrum(2);
            sleepForMs(700);
        }
        
        pthread_mutex_unlock(&zenH_mutex);
    }

    return NULL;
}

int16_t I2cbus1_getRawData(int8_t rawL, int8_t rawH)
{
    return (rawH << 8) | (rawL);
}

float I2cbus1_convertToGForce(int16_t rawData)
{
    return (float)rawData/RESOLUTION_8BITS_SHIFT;
}