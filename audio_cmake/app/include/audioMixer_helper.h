#ifndef _AUDIOMIXER_HELPER_H_
#define _AUDIOMIXER_HELPER_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

void runCommand(char* command);
void sleepForMs(long long delayInMs);
long long getTimeInMs(void);
void readFromFileToBuffer(char *fileName, char *buff, int maxLength);
void writeToFile(char * fileName);
int folderExists(const char *folderPath);
int convertTempoIntoTime(int tempo);
void trimString(char* input);
double exponentialAvg(double current_avg, double previous_avg);
double regularAvg(long current_size, double current_sum);

#endif