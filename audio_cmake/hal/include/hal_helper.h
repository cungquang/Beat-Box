#ifndef _HAL_HELPER_H_
#define _HAL_HELPER_H_

#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

void runCommand(char* command);
static void sleepForMs(long long delayInMs);
static long long getTimeInMs(void);
void readFromFileToScreen(char *fileName, char *buff, int maxLength);
void writeToFile(char * fileName);
int folderExists(const char *folderPath);

#endif