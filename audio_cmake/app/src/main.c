#include <stdio.h>
#include "../include/audioMixer_upd.h"
#include "../include/joystick_control.h"


int terminate_flag = 0;

void Operation() 
{
    UDP_initServer(&terminate_flag);
    AudioMixerControl_init(&terminate_flag);
    JoystickControl_init(&terminate_flag);

    AudioMixerControl_join();
    JoystickControl_join();
    UDP_join();

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
