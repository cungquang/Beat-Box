#ifndef _HAL_BUTTONUP_H_
#define _HAL_BUTTONUP_H_

#include <stdio.h>
#include <sys/stat.h>

void runCommand(char* command);
static void sleepForMs(long long delayInMs);
static long long getTimeInMs(void);
void readFromFileToScreen(char *fileName);
void writeToFile(char * fileName);

#endif