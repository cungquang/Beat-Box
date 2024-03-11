#include <stdio.h>

#include "../include/audioMixer_upd.h"

int terminate_flag = 0;

int main() {
 
    UDP_initServer(&terminate_flag);
    UDP_join();
    UDP_cleanup();
    return 0;
}
