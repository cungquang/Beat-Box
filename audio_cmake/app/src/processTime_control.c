#include "../include/processTime_control.h"

#define MAX_BUFFER_SIZE 300
#define MAX_SENDMSG_SIZE 800
#define MAX_TIME_PARTS 2

#define PROC_UPTIME_PATH "/proc/uptime"

//Statistic
static Period_statistics_t stats_refill;
static Period_statistics_t stats_accelerometer;

static double stats_refillBuffer[3];
static long count_refillBuffer;
static double stats_accelerBuffer[3];
static long count_accelerometer;
static int curr_mode;
static int curr_tempo;
static int curr_volume;

static int isTerminate;
static char buffer[MAX_BUFFER_SIZE];
static char* timeToParts[MAX_TIME_PARTS];
static char messageToSend[500];

static pthread_t processUpTime_id;

//Initate Private function
static void splitTimeToParts(char *input, char *intoParts[]);
static void* processUpTime_thread();
static void isAliveMessage(void);
static void readFromProcessUptime(void);
static void set_currentMode(void);
static void set_currentVolume(void);
static void set_currentTempo(void);
static void setStats_refillBuffer(void);
static void setStats_accelerometer(void);
static void serverTextDisplay();


/*
#############################
#           PUBLIC          #
#############################
*/


void ProcessTime_init(void) 
{
    isTerminate = 0;

    //Initiate Period Timer
    Period_init();

    //Create thread
    pthread_create(&processUpTime_id, NULL, processUpTime_thread, NULL);
}

void ProcessTime_join(void)
{
    pthread_join(processUpTime_id, NULL);
}

void ProcessTime_terminate(void)
{
    isTerminate = 1;
}

void ProcessTime_cleanup(void)
{
    //Period cleanup
    Period_cleanup();

    //Clean data before use
    memset(buffer, 0, sizeof(buffer));
    memset(messageToSend, 0, sizeof(messageToSend));
    for (int i = 0; i < MAX_TIME_PARTS; i++) {
        timeToParts[i] = NULL;
    }
}


/*
#############################
#          PRIVATE          #
#############################
*/


/////////////////////////////// GETTER /////////////////////////////// 


static void set_currentMode(void) 
{
    curr_mode = AudioMixerControl_getMode();   
}

static void set_currentVolume(void)
{
    curr_volume = AudioMixerControl_getVolume();
}

static void set_currentTempo(void)
{
    curr_tempo = AudioMixerControl_getTempo();
}

static void setStats_refillBuffer(void)
{
    //Reset & get statistic
    Period_getStatisticsAndClear(PERIOD_EVENT_REFILL_BUFFER, &stats_refill);

    stats_refillBuffer[0] = stats_refill.minPeriodInMs;
    stats_refillBuffer[1] = stats_refill.maxPeriodInMs;
    stats_refillBuffer[2] = stats_refill.avgPeriodInMs;
    count_refillBuffer = stats_refill.numSamples;
}

static void setStats_accelerometer(void)
{
     //Reset & get statistic
    Period_getStatisticsAndClear(PERIOD_EVENT_SAMPLE_LIGHT, &stats_accelerometer);

    stats_accelerBuffer[0] = stats_accelerometer.minPeriodInMs;
    stats_accelerBuffer[1] = stats_accelerometer.maxPeriodInMs;
    stats_accelerBuffer[2] = stats_accelerometer.avgPeriodInMs;
    count_accelerometer = stats_accelerometer.numSamples;
}


/////////////////////////////// MAJOR OPERATION ///////////////////////////////


static void* processUpTime_thread(void)
{
    while(!isTerminate) 
    {
        readFromProcessUptime();
        UDP_sendToTarget(messageToSend);

        isAliveMessage();
        UDP_sendToTarget(messageToSend);

        serverTextDisplay();
        
        //Sleep for 1 second
        sleepForMs(1000);
    }

    return NULL;
}


/////////////////////////////// SUPPORT ///////////////////////////////


static void isAliveMessage(void)
{
    memset(messageToSend, 0, sizeof(messageToSend));
    snprintf(messageToSend, MAX_SENDMSG_SIZE, "show_error,hide");
}

static void readFromProcessUptime(void)
{
    //Clean data before use
    memset(buffer, 0, sizeof(buffer));
    memset(messageToSend, 0, sizeof(messageToSend));
    for (int i = 0; i < MAX_TIME_PARTS; i++) {
        timeToParts[i] = NULL;
    }

    //Get time from /proc/uptime
    readFromFileToBuffer(PROC_UPTIME_PATH, buffer, MAX_BUFFER_SIZE);
    splitTimeToParts(buffer, timeToParts);
    snprintf(messageToSend, MAX_SENDMSG_SIZE, "timer,%s", timeToParts[0]);
}


static void splitTimeToParts(char *input, char *intoParts[]) 
{
    char *token;
    int partNum = 0;

    // Get the first token
    token = strtok(input, " ");

    // Continue getting tokens until NULL or 3 parts are found
    while (token != NULL && partNum < MAX_TIME_PARTS) {
        intoParts[partNum] = token;
        token = strtok(NULL, " ");
        partNum++;
    }
}

static void serverTextDisplay() {
    set_currentMode();
    set_currentTempo();
    set_currentVolume();
    setStats_accelerometer();
    setStats_refillBuffer();

    //need sample - count
    printf("M%d %dBpm vol%d   Audio[%.3f, %.3f] avg %.3f/%ld   Accel[%.3f, %.3f] avg %.3f/%ld\n", curr_mode, curr_tempo, curr_volume,
        stats_refillBuffer[0], stats_refillBuffer[1], stats_refillBuffer[2], count_refillBuffer,
        stats_accelerBuffer[0], stats_accelerBuffer[1], stats_accelerBuffer[2], count_accelerometer);
}

