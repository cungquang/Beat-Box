#include "../include/joystick_gpioDown.h"

#define GPIO46 "/sys/class/gpio/gpio46"
#define GPIO46_VALUE "/sys/class/gpio/gpio46/value"
#define GPIO46_DIRECTION "/sys/class/gpio/gpio46/direction"
#define GPIO46_ACTIVELOW "/sys/class/gpio/gpio46/active_low"

#define GPIO46_EXPORT_COMMAND "echo 46 > /sys/class/gpio/export"

#define MAX_BUFFER_SIZE 200

int isExported_gpioDown()
{
    return folderExists(GPIO46);
}

void configure_gpioDown()
{
    runCommand(GPIO46_EXPORT_COMMAND);
}

int readValue_gpioDown()
{
    char buffer[MAX_BUFFER_SIZE];
    readFromFileToBuffer(GPIO46_VALUE, buffer, MAX_BUFFER_SIZE);

    return atoi(buffer);
}