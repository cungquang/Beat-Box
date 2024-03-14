#include "../include/register_configure.h"

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

#define I2C_DEVICE_ADDRESS 0x20

#define REG_CTRL_REG1 0x20
#define REG_OUT_X_L 0x28
#define REG_OUT_X_H 0x29
#define REG_OUT_Y_L 0x2A
#define REG_OUT_Y_H 0x2B
#define REG_OUT_Z_L 0x2C
#define REG_OUT_Z_H 0x2D

static int i2c1_FileDesc;


////////////////////////// INIT OPERATION //////////////////////////

void I2cbus1_init()
{
    //Initiate the bus 1 at address 0x20
    i2c1_FileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

    //Configure pin
    initI2cpin_17();
    initI2cpin_18();
}


///////////////////////// WRITE OPERATION /////////////////////////

void I2cbus1Write_Reg1(unsigned char value)
{
    if(i2c1_FileDesc)
    {
        printf("ERROR: The bus was not iniated\n");
        exit(EXIT_FAILURE);
    }

    //Write to the CTRL_REG1
    writeI2cReg(i2c1_FileDesc, REG_CTRL_REG1, value);
}

////////////////////////// READ OPERATION //////////////////////////

unsigned char I2cbus1Read_Reg1()
{
    if(i2c1_FileDesc)
    {
        printf("ERROR: The bus was not iniated\n");
        exit(EXIT_FAILURE);
    }

    //Read from reg1
    return readI2cReg(i2c1_FileDesc, REG_CTRL_REG1);
}

unsigned char I2cbus1Read_OutXL()
{
    if(i2c1_FileDesc)
    {
        printf("ERROR: The bus was not iniated\n");
        exit(EXIT_FAILURE);
    }

    //Read from reg1
    return readI2cReg(i2c1_FileDesc, REG_OUT_X_L);
}

unsigned char I2cbus1Read_OutXH()
{
    if(i2c1_FileDesc)
    {
        printf("ERROR: The bus was not iniated\n");
        exit(EXIT_FAILURE);
    }

    //Read from reg1
    return readI2cReg(i2c1_FileDesc, REG_OUT_X_H);
}

unsigned char I2cbus1Read_OutYL()
{
    if(i2c1_FileDesc)
    {
        printf("ERROR: The bus was not iniated\n");
        exit(EXIT_FAILURE);
    }

    //Read from reg1
    return readI2cReg(i2c1_FileDesc, REG_OUT_Y_L);
}

unsigned char I2cbus1Read_OutYH()
{
    if(i2c1_FileDesc)
    {
        printf("ERROR: The bus was not iniated\n");
        exit(EXIT_FAILURE);
    }

    //Read from reg1
    return readI2cReg(i2c1_FileDesc, REG_OUT_Y_H);
}

unsigned char I2cbus1Read_OutZL()
{
    if(i2c1_FileDesc)
    {
        printf("ERROR: The bus was not iniated\n");
        exit(EXIT_FAILURE);
    }

    //Read from reg1
    return readI2cReg(i2c1_FileDesc, REG_OUT_Z_L);
}

unsigned char I2cbus1Read_OutZH()
{
    if(i2c1_FileDesc)
    {
        printf("ERROR: The bus was not iniated\n");
        exit(EXIT_FAILURE);
    }

    //Read from reg1
    return readI2cReg(i2c1_FileDesc, REG_OUT_Z_H);
}