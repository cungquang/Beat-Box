#include <stdio.h>
#include "../include/audioMixer_upd.h"
#include "../include/joystick_control.h"


int terminate_flag = 0;

void Operation() 
{
    //AudioMixerControl_init(&terminate_flag);
    JoystickControl_init(&terminate_flag);
    UDP_initServer(&terminate_flag);

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
