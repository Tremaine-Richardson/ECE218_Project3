//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "wipers.h"
#include "servo_motor.h"
#include "windshield_wiper_subsystem.h"
#include "potentiometer.h"
#include "user_interface.h"

//=====[Declaration of private defines]========================================

#define DUTY_SIXTY_SEVEN .051

#define LOW_SPEED_INCREMENT .0007
#define HIGH_SPEED_INCREMENT .001

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

//=====[Declaration and initialization of public global variables]=============

static int accumulatedTimeDelay = 0;
static float servoPosition = 0.0;
static float modeSelect = 0.0;
static float delaySelect = 0.0;

//=====[Declaration and initialization of private global variables]============

servoState_t wipeState;
modeState_t mode;

//=====[Declarations (prototypes) of private functions]========================

static void wipersHighMode();
static void wipeCycle(float increment);
static void wipersLowMode();
static void wipersIntMode(int waitTime);
static void wipersOffMode();

//=====[Implementations of public functions]===================================

void wipersInit()
{
    servoInit();
    wipeState = HOMED;
    servoPosition = servoPositionRead();
}

//wipersUpdate() reads the position of the wiper mode selector potentiometer and the delay
//selector potentiometer and uses thresholds to determine where the potentiometer is and what
//mode or delay time corresponds with that position. For the mode selector, each mode has its
//own function that runs whenever the potentiometer is within their range. For the INT mode range
//the delay selector potentiometer uses its position to pick which wait time parameter will be 
//used in the INT mode function. The modes can be selected if the engineStateRead() reads that
//the engine is on, otherwise the function will default to off mode

void wipersUpdate()
{
    modeSelect = wiperMode();
    delaySelect = delayTime();
    if( engineStateRead() ) {
        if( modeSelect >= HIGH_MODE_THRESHOLD ) {
            wipersHighMode();
        }
        if( modeSelect >= LOW_MODE_THRESHOLD && modeSelect < HIGH_MODE_THRESHOLD ) {
            wipersLowMode();
        }
        if(modeSelect < INT_MODE_THRESHOLD ) {
            wipersOffMode(); 
        }
        if(modeSelect >= INT_MODE_THRESHOLD && modeSelect < LOW_MODE_THRESHOLD){
            if(delaySelect >= LONG_DELAY_THRESHOLD){
                wipersIntMode(LONG_DELAY);
            }
            if(delaySelect < LONG_DELAY_THRESHOLD && delaySelect >= MEDIUM_DELAY_THRESHOLD){
                wipersIntMode(MEDIUM_DELAY);
            }
            if(delaySelect < MEDIUM_DELAY_THRESHOLD){
                wipersIntMode(SHORT_DELAY);
            }

        }
    }
    else {
        wipersOffMode();
    }
}


//uses the mode variable to send numbers corresponding to what mode the wipers are currently in,
//this function is used in the user_interface module for the display aspect
int wipersModeRead()
{
    switch( mode ){
        case MODE_HIGH:
            return 1;
            break;
        
        case MODE_LOW:
            return 2;
            break;
        
        case MODE_OFF:
            return 3;
            break;
        
        case MODE_INT:
            return 4;
            break;
        
        default:
            return 0;
            break;
    }
}

//=====[Implementations of private functions]==================================

//Each mode function runs wipeCycle with a given increment. For HIGH mode, it uses the high speed increment.
//For LOW and INT mode, it uses the low speed increment. And for OFF mode, it depends on what the previous
//mode was in, in order to finish its current wipe cycle (if it is in one) at the appropriate speed

static void wipersHighMode()
{
    wipeCycle( HIGH_SPEED_INCREMENT );
    mode = MODE_HIGH;
}

static void wipersLowMode()
{
    wipeCycle( LOW_SPEED_INCREMENT );
    mode = MODE_LOW;
}

static void wipersIntMode( int waitTime )
{
    if( wipeState != HOMED ) { //if the wipe is currently in a cycle, it completes its cycle
        wipeCycle( LOW_SPEED_INCREMENT );
    }
    else { //if a cycle has been complete and the wipeState is at home, it will wait for a given waitTime
           //remained homed while it does so
        accumulatedTimeDelay = accumulatedTimeDelay + SYSTEM_TIME_INCREMENT_MS;
        if( accumulatedTimeDelay <= waitTime ) {
            wipeState = HOMED;
        }
        else {
            wipeCycle( LOW_SPEED_INCREMENT );
            accumulatedTimeDelay = 0;
        }
    }
    mode = MODE_INT;
}

static void wipersOffMode() 
{
    if( wipeState != HOMED) {
        if (mode == MODE_HIGH ) {
            wipeCycle( HIGH_SPEED_INCREMENT );
        }
        else {
            wipeCycle( LOW_SPEED_INCREMENT );
        }
    }
    mode = MODE_OFF;
}

//wipeCycle() takes an increment as a parameter and is the function that moves the servo. It works
//by using a wipeState variable to determine if the servo should be moving clockwise, counter clockwise
//or if it is currently at DUTY_ZERO, homed. The servo is initially homed, and once this function is
//called it sets it to the counter clockwise state. From there, while the servo position is less than
//the duty cycle value found to be 67 degrees, each time the wipeCycle function is called it will move
//the servo counter clockwise by a given increment. This increment is what determines the speed where 
//the higher the increment, the faster the servo will move. Once the servo position reaches the 67
//degree mark, the wipe state will be set to clockwise and will now move clockwise by the given 
//increment until it reaches DUTY_ZERO again. Once there, the wipe state gets set to home and the wipe
//is complete. Then to start the next wipe, the wipeCycle() function will just need to be called once more

static void wipeCycle(float increment)
{
    servoPosition = servoPositionRead();
    switch( wipeState ) {
        case WIPE_COUNTER_CLOCKWISE:
            if( servoPosition <= DUTY_SIXTY_SEVEN ) {
                servoDutyCycleWrite( servoPosition + increment );
            }
            else {
                wipeState = WIPE_CLOCKWISE;
            }
            break;
        
        case WIPE_CLOCKWISE:
            if ( servoPosition >= DUTY_ZERO ) {
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
