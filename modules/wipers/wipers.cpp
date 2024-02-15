//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "wipers.h"
#include "servo_motor.h"

//=====[Declaration of private defines]========================================

#define DUTY_ZERO .019
#define DUTY_SIXTY_SEVEN .051

#define LOW_SPEED_INCREMENT .0005
#define HIGH_SPEED_INCREMENT .005

//=====[Declaration of private data types]=====================================

typedef enum {
    WIPE_COUNTER_CLOCKWISE,
    WIPE_CLOCKWISE,
    HOMED
} servoState_t;

//=====[Declaration and initialization of public global objects]===============

float servoPosition = 0.0;

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

servoState_t wipeState;

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

void wipersInit()
{
    servoInit();
    wipeState = HOMED;
    servoPosition = servoPositionRead();
}

void wipersUpdate()
{

}

//=====[Implementations of private functions]==================================

void wipeCycle(float increment)
{
    servoPosition = servoPositionRead();
    switch( wipeState ) {
        case WIPE_COUNTER_CLOCKWISE:
            if( servoPosition < DUTY_SIXTY_SEVEN ) {
                servoDutyCycleWrite( servoPosition + increment );
            }
            else {
                wipeState = WIPE_CLOCKWISE;
            }
            break;
        
        case WIPE_CLOCKWISE:
            if ( servoPosition > DUTY_ZERO ) {
                servoDutyCycleWrite( servoPosition - increment);
            }
            else {
                wipeState = HOMED;
            }
            break;
        
        case HOMED:
            wipeState = WIPE_COUNTER_CLOCKWISE;
            break;
    }  
}
