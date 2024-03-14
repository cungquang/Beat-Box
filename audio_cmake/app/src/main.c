#include <stdio.h>
#include "../include/audioMixer_upd.h"
#include "../include/joystick_control.h"
#include "../include/processTime_control.h"

void Operation() 
{
    UDP_initServer();
    AudioMixerControl_init();
    JoystickControl_init();
    ProcessTime_init();

    AudioMixerControl_join();
    JoystickControl_join();
    Application_join();
    UDP_join();

    //Cleanup -> later move to shutdown thread
    UDP_cleanup();
    AudioMixerControl_cleanup();
    JoystickControl_cleanup();
}

int main() 
{
    Operation();
    //readFromProcessUptime();

    return 0;
}
