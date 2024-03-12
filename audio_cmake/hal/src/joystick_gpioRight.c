#include "../include/joystick_gpioRight.h"

#define GPIO47 "/sys/class/gpio/gpio47"
#define GPIO47_VALUE "/sys/class/gpio/gpio47/value"
#define GPIO47_DIRECTION "/sys/class/gpio/gpio47/direction"
#define GPIO47_ACTIVELOW "/sys/class/gpio/gpio47/active_low"

#define GPIO47_EXPORT_COMMAND "echo 47 > /sys/class/gpio/export"

#define MAX_BUFFER_SIZE 200

int isExported_gpioRight()
{
    return folderExists(GPIO47);
}

void configure_gpioRight()
{
    runCommand(GPIO47_EXPORT_COMMAND);
}

int readValue_gpioRight()
{
    char buffer[MAX_BUFFER_SIZE];
    readFromFileToBuffer(GPIO47_VALUE, buffer, MAX_BUFFER_SIZE);

    return atoi(buffer);
}