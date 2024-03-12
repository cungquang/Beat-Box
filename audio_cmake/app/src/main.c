#include <stdio.h>
#include "../include/audioMixer_upd.h"
#include "../include/joystick_control.h"


int terminate_flag = 0;

void Operation() 
{
    UDP_initServer(&terminate_flag);
    AudioMixerControl_init(&terminate_flag);
    JoystickControl_init(&terminate_flag);

    AudioMixerControl_setUserSelection(2);

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
    
    test();
    return 0;
}
