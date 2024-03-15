#include "../include/i2c_configure.h"

#define MAX_BUFFER_SIZE 500

#define P9_17_STATE_PATH "/sys/devices/platform/ocp/ocp:P9_17_pinmux/state"
#define P9_18_STATE_PATH "/sys/devices/platform/ocp/ocp:P9_18_pinmux/state"

#define CONFIGURE_PIN_18 "config-pin p9_18 i2c > /dev/null"
#define CONFIGURE_PIN_17 "config-pin p9_17 i2c > /dev/null"

static char buffer[MAX_BUFFER_SIZE];


void initI2c_p917(void)
{
    runCommand(CONFIGURE_PIN_17);
}

void initI2c_p918(void)
{
    runCommand(CONFIGURE_PIN_18);
}

int isI2cConfigure_p917()
{
    memset(buffer, 0, sizeof(buffer));
    readFromFileToBuffer(P9_17_STATE_PATH, buffer, sizeof(buffer));
    trimString(buffer);
    if(strcmp(buffer, "i2c") == 0)
    {
        return 1;
    }
    return 0;
}

int isI2cConfigure_p918()
{
    memset(buffer, 0, sizeof(buffer));
    readFromFileToBuffer(P9_18_STATE_PATH, buffer, sizeof(buffer));
    trimString(buffer);
    if(strcmp(buffer, "i2c") == 0)
    {
        return 1;
    }
    return 0;
}

int initI2cBus(char* bus, int address)
{
    int i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);

    if (result < 0) {
        perror("I2C: Unable to set I2C device to slave address.");
        exit(1);
    }
    return i2cFileDesc;
}

void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);
    if (res != 2) {
        perror("I2C: Unable to write i2c register.");
        exit(1);
    }
}

unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr)
{
    // To read a register, must first write the address
    int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
    if (res != sizeof(regAddr)) {
        perror("I2C: Unable to write to i2c register.");
        exit(1);
    }
    // Now read the value and return it
    char value = 0;
    res = read(i2cFileDesc, &value, sizeof(value));
    if (res != sizeof(value)) {
        perror("I2C: Unable to read from i2c register");
        exit(1);
    }
    return value;
}
