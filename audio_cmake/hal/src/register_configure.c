#include "../include/register_configure.h"

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

#define I2C_DEVICE_ADDRESS 0x18

#define REG_CTRL_REG1 0x20
#define REG_OUT_X_L 0x28
#define REG_OUT_X_H 0x29
#define REG_OUT_Y_L 0x2A
#define REG_OUT_Y_H 0x2B
#define REG_OUT_Z_L 0x2C
#define REG_OUT_Z_H 0x2D

static int i2c1_FileDesc;


/*
#########################
#        PUBLIC         #
#########################
*/


////////////////////////// INIT OPERATION //////////////////////////

void I2cbus1_init()
{
    //Enable the bus1 - 0x18
    i2c1_FileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

    //Pin9_17 not yet configure -> configure
    if(!isI2cConfigure_p917())
    {
        initI2c_p917();
    }

    //Pin9_18 not yet configure -> configure
    if(!isI2cConfigure_p918())
    {
        initI2c_p918();
    }
}

int I2c1FileDesc_get()
{
    return i2c1_FileDesc;
}

void I2c1FileDesc_set(int newI2c1FileDesc)
{
    i2c1_FileDesc = newI2c1FileDesc;
}

///////////////////////// WRITE OPERATION /////////////////////////

void I2cbus1Write_Reg1(unsigned char value)
{
    if(!i2c1_FileDesc)
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
    if(!i2c1_FileDesc)
    {
        printf("ERROR: The bus was not iniated\n");
        exit(EXIT_FAILURE);
    }

    //Read from reg1
    return readI2cReg(i2c1_FileDesc, REG_CTRL_REG1);
}

unsigned char I2cbus1Read_OutXL()
{
    if(!i2c1_FileDesc)
    {
        printf("ERROR: The bus was not iniated\n");
        exit(EXIT_FAILURE);
    }

    //Read from reg1
    return readI2cReg(i2c1_FileDesc, REG_OUT_X_L);
}

unsigned char I2cbus1Read_OutXH()
{
    if(!i2c1_FileDesc)
    {
        printf("ERROR: The bus was not iniated\n");
        exit(EXIT_FAILURE);
    }

    //Read from reg1
    return readI2cReg(i2c1_FileDesc, REG_OUT_X_H);
}

unsigned char I2cbus1Read_OutYL()
{
    if(!i2c1_FileDesc)
    {
        printf("ERROR: The bus was not iniated\n");
        exit(EXIT_FAILURE);
    }

    //Read from reg1
    return readI2cReg(i2c1_FileDesc, REG_OUT_Y_L);
}

unsigned char I2cbus1Read_OutYH()
{
    if(!i2c1_FileDesc)
    {
        printf("ERROR: The bus was not iniated\n");
        exit(EXIT_FAILURE);
    }

    //Read from reg1
    return readI2cReg(i2c1_FileDesc, REG_OUT_Y_H);
}

unsigned char I2cbus1Read_OutZL()
{
    if(!i2c1_FileDesc)
    {
        printf("ERROR: The bus was not iniated\n");
        exit(EXIT_FAILURE);
    }

    //Read from reg1
    return readI2cReg(i2c1_FileDesc, REG_OUT_Z_L);
}

unsigned char I2cbus1Read_OutZH()
{
    if(!i2c1_FileDesc)
    {
        printf("ERROR: The bus was not iniated\n");
        exit(EXIT_FAILURE);
    }

    //Read from reg1
    return readI2cReg(i2c1_FileDesc, REG_OUT_Z_H);
}


/*
#########################
#       PRIVATE         #
#########################
*/
