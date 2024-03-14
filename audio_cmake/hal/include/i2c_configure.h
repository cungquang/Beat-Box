#ifndef _I2C_CONFIGURE_H_
#define _I2C_CONFIGURE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "./hal_helper.h"

//Configure pin 17 & 18
void initI2c_p917(void);
void initI2c_p918(void);

//Read from pin9_17 & p9_18
int isI2cConfigure_p917();
int isI2cConfigure_p918();

//Initiate I2C bus
int initI2cBus(char* bus, int address);

//Write to I2C register
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);

//Read from I2C register
unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr);

#endif