#include <stdio.h>
#include "../include/audioMixer_upd.h"
#include "../include/joystick_control.h"


int terminate_flag = 0;

void Operation() 
{
    UDP_initServer(&terminate_flag);
    JoystickControl_init(&terminate_flag);

    UDP_join();
    JoystickControl_join();

    //Cleanup -> later move to shutdown thread
    UDP_cleanup();
    JoystickControl_cleanup();
}

void test()
{

    int gpio = readValue_gpioPress();
    printf("value from gpio press: %d\n", gpio);

    gpio = readValue_gpioUp();
    printf("value from gpio up: %d\n", gpio);

    gpio = readValue_gpioDown();
    printf("value from gpio down: %d\n", gpio);

    gpio = readValue_gpioLeft();
    printf("value from gpio left: %d\n", gpio);

    gpio = readValue_gpioRight();
    printf("value from gpio right: %d\n", gpio);
}


int main() 
{
    
    Operation();
    return 0;
}
