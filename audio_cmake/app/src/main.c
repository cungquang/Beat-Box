#include <stdio.h>
#include "../include/audioMixer_upd.h"
#include "../include/joystick_control.h"
#include "../include/processTime_control.h"
#include "../../hal/include/register_configure.h"
#include "../include/i2cbus1_control.h"

void Operation() 
{
    UDP_initServer();
    AudioMixerControl_init();
    JoystickControl_init();
    ProcessTime_init();
    I2cbus1Control_init();

    I2cbus1Control_join();
    ProcessTime_join();
    JoystickControl_join();
    AudioMixerControl_join();
    UDP_join();

    //Cleanup -> later move to shutdown thread
    UDP_cleanup();
    AudioMixerControl_cleanup();
    JoystickControl_cleanup();
    ProcessTime_cleanup();
    I2cbus1Control_cleanup();
}

int main() 
{
    Operation();
    return 0;
}
