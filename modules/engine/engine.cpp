//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "engine.h"
#include "user_interface.h"

//=====[Declaration of private defines]========================================

#define DEBOUNCE_BUTTON_TIME_MS                 40

//=====[Declaration of private data types]=====================================

typedef enum {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_FALLING,
    BUTTON_RISING
} buttonState_t;

//=====[Declaration and initialization of public global objects]===============

DigitalIn driverSeat(D2);
DigitalIn ignitionSwitch(D3);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static int accumulatedDebounceButtonTime     = 0;
static int numberOfEnterButtonReleasedEvents = 0;
buttonState_t ignitionSwitchState;

//=====[Declarations (prototypes) of private functions]========================

static void ignitionSwitchInit();
static bool ignitionSwitchUpdate();

//=====[Implementations of public functions]===================================

void engineInit() 
{
    driverSeat.mode(PullDown);
    ignitionSwitch.mode(PullUp);
}

void engineUpdate() 
{
    bool ignitionSwitch = ignitionSwitchUpdate();
    if( ignitionSwitch ) {
        if( !(engineStateRead()) ){
            if( driverSeat ){
                engineStateWrite(ON);
            }
        }
        else{
            engineStateWrite(OFF);
        }
    }

}

//=====[Implementations of private functions]==================================

void ignitionSwitchInit()
{
    if( ignitionSwitch == 1) {
        ignitionSwitchState = BUTTON_UP;
    } else {
        ignitionSwitchState = BUTTON_DOWN;
    }
}


bool ignitionSwitchUpdate()
{
    bool ignitionSwitchReleasedEvent = false;
    switch( ignitionSwitchState ) {


    case BUTTON_UP:
        if( ignitionSwitch == 0 ) {
            ignitionSwitchState = BUTTON_FALLING;
            accumulatedDebounceButtonTime = 0;
        }
        break;


    case BUTTON_FALLING:
        if( accumulatedDebounceButtonTime >= DEBOUNCE_BUTTON_TIME_MS ) {
            if( ignitionSwitch == 0 ) {
                ignitionSwitchState = BUTTON_DOWN;
            } else {
                ignitionSwitchState = BUTTON_UP;
            }
        }
        accumulatedDebounceButtonTime = accumulatedDebounceButtonTime + TIME_INCREMENT_MS;
        break;


    case BUTTON_DOWN:
        if( ignitionSwitch == 1 ) {
            ignitionSwitchState = BUTTON_RISING;
            accumulatedDebounceButtonTime = 0;
        }
        break;


    case BUTTON_RISING:
        if( accumulatedDebounceButtonTime >= DEBOUNCE_BUTTON_TIME_MS ) {
            if( ignitionSwitch == 1 ) {
                ignitionSwitchState = BUTTON_UP;
                ignitionSwitchReleasedEvent = true;
            } else {
                ignitionSwitchState = BUTTON_DOWN;
            }
        }
        accumulatedDebounceButtonTime = accumulatedDebounceButtonTime +
                                        TIME_INCREMENT_MS;
        break;


    default:
        ignitionSwitchInit();
        break;
    }
    return ignitionSwitchReleasedEvent;
}



