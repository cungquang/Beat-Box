#include "../include/application_control.h"

#define MAX_BUFFER_SIZE 300
#define MAX_SENDMSG_SIZE 800
#define MAX_TIME_PARTS 2

#define PROC_UPTIME_PATH "/proc/uptime"

static int isTerminate;
static char buffer[MAX_BUFFER_SIZE];
static char* timeToParts[MAX_TIME_PARTS];
static char messageToSend[500];

static pthread_t processUpTime_id;

//Initate Private function
static void splitTimeToParts(char *input, char *intoParts[]);
void* processUpTime_thread();
void readFromProcessUptime();


/*
#############################
#           PUBLIC          #
#############################
*/


void Appliction_init() 
{
    isTerminate = 0;

    //Create thread
    pthread_create(&processUpTime_id, NULL, processUpTime_thread, NULL);
}

void Application_join()
{
    pthread_join(processUpTime_id, NULL);
}

void Application_terminate()
{
    isTerminate = 1;
}

void Application_cleanup()
{
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


void* processUpTime_thread()
{
    while(!isTerminate) 
    {
        readFromProcessUptime();
        UDP_sendToTarget(messageToSend);

        //Sleep for 1 second
        sleepForMs(1000);
    }

    return NULL;
}


void readFromProcessUptime()
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
    printf("%s\n", timeToParts[0]);
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

