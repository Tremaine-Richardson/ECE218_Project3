//=====[Libraries]=============================================================

#include "windshield_wiper_subsystem.h"

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    windshieldWiperSubsystemInit();
    while (true) {
        windshieldWiperSubsystemUpdate();
    }
}