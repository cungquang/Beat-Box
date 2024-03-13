#include <stdio.h>
#include "../include/audioMixer_upd.h"
#include "../include/joystick_control.h"


void Operation() 
{
    AudioMixerControl_init();
    JoystickControl_init();
    UDP_initServer();

    UDP_join();
    AudioMixerControl_join();
    JoystickControl_join();

    //Cleanup -> later move to shutdown thread
    UDP_cleanup();
    AudioMixerControl_cleanup();
    JoystickControl_cleanup();
}

int main() 
{
    Operation();
    return 0;
}
