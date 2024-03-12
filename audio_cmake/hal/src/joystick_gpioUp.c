#include "../include/joystick_gpioUp.h"

#define GPIO26 "/sys/class/gpio/gpio26"
#define GPIO26_VALUE "/sys/class/gpio/gpio26/value"
#define GPIO26_DIRECTION "/sys/class/gpio/gpio26/direction"
#define GPIO26_ACTIVELOW "/sys/class/gpio/gpio26/active_low"

#define GPIO26_EXPORT_COMMAND "echo 26 > /sys/class/gpio/export"

#define MAX_BUFFER_SIZE 200

int isExported_gpioUp()
{
    return folderExists(GPIO26);
}

void configure_gpioUp()
{
    runCommand(GPIO26_EXPORT_COMMAND);
}

int readValue_gpioUp()
{
    char buffer[MAX_BUFFER_SIZE];
    readFromFileToBuffer(GPIO26_VALUE, buffer, MAX_BUFFER_SIZE);

    return atoi(buffer);
}