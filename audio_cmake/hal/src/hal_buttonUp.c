#include "../include/hal_buttonUp.h"

#define GPIO27 "/sys/class/gpio/gpio27"
#define GPIO27_VALUE "/sys/class/gpio/gpio27/value"
#define GPIO27_DIRECTION "/sys/class/gpio/gpio27/direction"
#define GPIO27_ACTIVELOW "/sys/class/gpio/gpio27/active_low"

#define GPIO27_EXPORT_COMMAND "echo 27 > /sys/class/gpio/export"

#define MAX_BUFFER_SIZE 1024

int isExported_gpio27()
{
    return folderExists(GPIO27);
}

void configure_gpio27()
{
    runCommand(GPIO27_EXPORT_COMMAND);
}

int readValue_gpio27()
{
    char buffer[MAX_BUFFER_SIZE];
    readFromFileToBuffer(GPIO27_VALUE, buffer, MAX_BUFFER_SIZE);

    return atoi(buffer);
}