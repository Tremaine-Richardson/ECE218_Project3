//=====[Libraries]=============================================================

#include "arm_book_lib.h"

#include "windshield_wiper_subsystem.h"

#include "user_interface.h"
#include "engine.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

void windshieldWiperSubsystemInit()
{
    userInterfaceInit();
    engineInit();
}

void windshieldWiperSubsystemUpdate()
{
    userInterfaceUpdate();
    engineUpdate();
    delay(SYSTEM_TIME_INCREMENT_MS);
}

//=====[Implementations of private functions]==================================
