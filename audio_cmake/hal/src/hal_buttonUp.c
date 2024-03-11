#include "../include/hal_buttonUp.h"

#define SERVER_IP "192.168.7.2"
#define GPIO27_CONFIGURE "echo 27 > export"
#define GPIO27 "/sys/class/gpio/gpio27"
#define GPIO27_VALUE "/sys/class/gpio/gpio27/value"
#define GPIO27_DIRECTION "/sys/class/gpio/gpio27/direction"
#define GPIO27_ACTIVELOW "/sys/class/gpio/gpio27/active_low"


int verifyConfigurePin()
{
    return folderExists(GPIO27);
}

void configureGpionPin()
{
    runCommand(GPIO27_CONFIGURE);
}

