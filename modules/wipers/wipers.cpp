//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "wipers.h"
#include "servo_motor.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

typedef enum {
    WIPE_COUNTER_CLOCKWISE,
    WIPE_CLOCKWISE,
    HOMED
} servoState_t;

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

servoState_t wipeState;

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

void wipersInit()
{
    
    wipeState = HOMED
}

void wipersUpdate()
{

}

//=====[Implementations of private functions]==================================

void wipeCycle(float increment)
{
    switch( wipeState ) {
        case WIPE_COUNTER_CLOCKWISE:
            
            break;
        
        case WIPE_CLOCKWISE:
            break;
        
        case HOMED:
            wipeState = WIPE_COUNTER_CLOCKWISE
            break;
    }  
}
