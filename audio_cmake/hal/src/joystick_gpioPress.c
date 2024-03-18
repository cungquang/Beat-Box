#include "../include/joystick_gpioPress.h"

#define GPIO27 "/sys/class/gpio/gpio27"
#define GPIO27_VALUE "/sys/class/gpio/gpio27/value"
#define GPIO27_DIRECTION "/sys/class/gpio/gpio27/direction"
#define GPIO27_ACTIVELOW "/sys/class/gpio/gpio27/active_low"

#define GPIO27_EXPORT_COMMAND "echo 27 > /sys/class/gpio/export"

#define MAX_BUFFER_SIZE 200

int isExported_gpioPress()
{
    return folderExists(GPIO27);
}

void configure_gpioPress()
{
    runCommand(GPIO27_EXPORT_COMMAND);
}

int readValue_gpioPress()
{
    char buffer[MAX_BUFFER_SIZE];
    readFromFileToBuffer(GPIO27_VALUE, buffer, MAX_BUFFER_SIZE);

    return atoi(buffer);
}