#ifndef _HAL_HELPER_H_
#define _HAL_HELPER_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/wait.h>

void runCommand(char* command);
void sleepForMs(long long delayInMs);
long long getTimeInMs(void);
void readFromFileToBuffer(char *fileName, char *buff, int maxLength);
void writeToFile(char * fileName);
int folderExists(const char *folderPath);

#endif