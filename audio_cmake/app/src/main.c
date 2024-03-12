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
    AudioMixerControl_init(&terminate_flag);
    AudioMixerControl_setUserSelection(2);

    AudioMixerControl_join();
    AudioMixerControl_cleanup();
}


int main() 
{
    
    test();
    return 0;
}
