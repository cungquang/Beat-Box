#include "../include/application_control.h"

#define MAX_BUFFER_SIZE 300
#define PROC_UPTIME_PATH "../../../proc/uptime"

static int isTerminate;
static char buffer[MAX_BUFFER_SIZE];
static char messageToSend[500];

static pthread_t processTime_id;

/*
#############################
#           PUBLIC          #
#############################
*/


void Appliction_control() 
{
    isTerminate = 0;

}

void* processTimeRead_thread()
{
    while(!isTerminate) 
    {
        //UDP_sendToTarget();
        sleepForMs(1000);
    }
}

void readFromProcessUptime()
{
    readFromFileToBuffer(PROC_UPTIME_PATH, buffer, MAX_BUFFER_SIZE);
    printf("%s\n", buffer);
    //snprintf(messageToSend, 800, "uptime,%s");

}
