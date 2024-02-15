//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "wipers.h"
#include "servo_motor.h"
#include "windshield_wiper_subsystem.h"
#include "potentiometer.h"

//=====[Declaration of private defines]========================================

#define DUTY_SIXTY_SEVEN .051

#define LOW_SPEED_INCREMENT .0005
#define HIGH_SPEED_INCREMENT .0008

#define HIGH_MODE_THRESHOLD .75
#define LOW_MODE_THRESHOLD .5
#define INT_MODE_THRESHOLD .25

#define LONG_DELAY_THRESHOLD .6
#define MEDIUM_DELAY_THRESHOLD .3

#define LONG_DELAY 8000
#define MEDIUM_DELAY 6000
#define SHORT_DELAY 3000


//=====[Declaration of private data types]=====================================

typedef enum {
    WIPE_COUNTER_CLOCKWISE,
    WIPE_CLOCKWISE,
    HOMED
} servoState_t;

typedef enum {
    MODE_HIGH,
    MODE_LOW,
    MODE_OFF,
    MODE_INT,
} modeState_t;

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

int accumulatedTimeDelay = 0;
float servoPosition = 0.0;
float modeSelect = 0.0;
float delaySelect = 0.0;

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

servoState_t wipeState;
modeState_t mode;

//=====[Declarations (prototypes) of private functions]========================

void wipersHighMode();
void wipeCycle(float increment);
void wipersLowMode();
void wipersIntMode(int waitTime);
void wipersOffMode();

//=====[Implementations of public functions]===================================

void wipersInit()
{
    servoInit();
    wipeState = HOMED;
    servoPosition = servoPositionRead();
}

void wipersUpdate()
{
    modeSelect = wiperMode();
    delaySelect = delayTime();
    if ( modeSelect < HIGH_MODE_THRESHOLD ) {
        if ( modeSelect < LOW_MODE_THRESHOLD ) {
            if ( modeSelect < INT_MODE_THRESHOLD ) {
                wipersOffMode();
            }
            else {
                if( delaySelect < LONG_DELAY_THRESHOLD ) {
                    if( delaySelect < MEDIUM_DELAY_THRESHOLD ) {
                        wipersIntMode(SHORT_DELAY);
                    }
                    else {
                        wipersIntMode(MEDIUM_DELAY);
                    }
                }
                else {
                    wipersIntMode(LONG_DELAY);
                }
            }
        }
        else {
            wipersLowMode();
        }
    }
    else {
        wipersHighMode();
    }
    

}

//=====[Implementations of private functions]==================================

void wipersHighMode()
{
    wipeCycle( HIGH_SPEED_INCREMENT );
}

void wipersLowMode()
{
    wipeCycle( LOW_SPEED_INCREMENT );
}

void wipersIntMode( int waitTime )
{
    if( wipeState != HOMED ) {
        wipeCycle( LOW_SPEED_INCREMENT );
    }
    else {
        accumulatedTimeDelay = accumulatedTimeDelay + SYSTEM_TIME_INCREMENT_MS;
        if( accumulatedTimeDelay <= waitTime ) {
            wipeState = HOMED;
        }
        else {
            wipeCycle( LOW_SPEED_INCREMENT );
            accumulatedTimeDelay = 0;
        }
    }
}

void wipersOffMode() 
{
    if( wipeState != HOMED) {
        if (mode == MODE_HIGH ) {
            wipeCycle( HIGH_SPEED_INCREMENT );
        }
        else {
            wipeCycle( LOW_SPEED_INCREMENT );
        }
    }
    else {
        mode = MODE_OFF;
    }
}

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
