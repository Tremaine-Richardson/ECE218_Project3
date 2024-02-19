//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "engine.h"
#include "user_interface.h"
#include "windshield_wiper_subsystem.h"

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

DigitalIn driverSeat(D11);
DigitalIn ignitionSwitch(D10);

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

//engineUpdate() uses a FSM for debouncing the ignition switch button and for determining
//when it is released. With that, we determine if the driver seat button is held and when
//the ignition switch button gets released, we can write engine state as ON and the LED 
//will update in the user_interface module
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

static void ignitionSwitchInit()
{
    if( ignitionSwitch == 1) {
        ignitionSwitchState = BUTTON_UP;
    } else {
        ignitionSwitchState = BUTTON_DOWN;
    }
}


static bool ignitionSwitchUpdate()
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
        accumulatedDebounceButtonTime = accumulatedDebounceButtonTime + SYSTEM_TIME_INCREMENT_MS;
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
                                        SYSTEM_TIME_INCREMENT_MS;
        break;


    default:
        ignitionSwitchInit();
        break;
    }
    return ignitionSwitchReleasedEvent;
}



