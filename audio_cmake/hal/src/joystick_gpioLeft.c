#include "../include/joystick_gpioLeft.h"

#define GPIO65 "/sys/class/gpio/gpio65"
#define GPIO65_VALUE "/sys/class/gpio/gpio65/value"
#define GPIO65_DIRECTION "/sys/class/gpio/gpio65/direction"
#define GPIO65_ACTIVELOW "/sys/class/gpio/gpio65/active_low"

#define GPIO65_EXPORT_COMMAND "echo 65 > /sys/class/gpio/export"

#define MAX_BUFFER_SIZE 200

int isExported_gpioLeft()
{
    return folderExists(GPIO65);
}

void configure_gpioLeft()
{
    runCommand(GPIO65_EXPORT_COMMAND);
}

int readValue_gpioLeft()
{
    char buffer[MAX_BUFFER_SIZE];
    readFromFileToBuffer(GPIO65_VALUE, buffer, MAX_BUFFER_SIZE);

    return atoi(buffer);
}