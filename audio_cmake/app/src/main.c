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

    AudioMixerControl_join();
    JoystickControl_join();
    ProcessTime_join();
    UDP_join();

    //Cleanup -> later move to shutdown thread
    UDP_cleanup();
    AudioMixerControl_cleanup();
    JoystickControl_cleanup();
    ProcessTime_cleanup();
}

void test()
{
    unsigned char test;
    //readFromProcessUptime();
    I2cbus1_init();
    I2cbus1Write_Reg1(0x27);

    test = I2cbus1Read_OutXH();
    printf("OUT_X_H: %d ---> %hhu\n", I2c1FileDesc_get(), test);
    test = I2cbus1Read_OutYH();
    printf("OUT_Y_H: %d ---> %hhu\n", I2c1FileDesc_get(), test);
    test = I2cbus1Read_OutZH();
    printf("OUT_Z_H: %d ---> %hhu\n", I2c1FileDesc_get(), test);

    printf("-------------------------------------\n");

    test = I2cbus1Read_OutXL();
    printf("OUT_X_L: %d ---> %hhu\n", I2c1FileDesc_get(), test);
    test = I2cbus1Read_OutYL();
    printf("OUT_Y_L: %d ---> %hhu\n", I2c1FileDesc_get(), test);
    test = I2cbus1Read_OutZL();
    printf("OUT_Z_L: %d ---> %hhu\n", I2c1FileDesc_get(), test);

}

int main() 
{
    UDP_initServer();
    I2cbus1Control_init();

    I2cbus1Control_join();
    UDP_join();
    UDP_cleanup();
    return 0;
}
