//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "user_interface.h"

#include "windshield_wiper_subsystem.h"
#include "display.h"
#include "wipers.h"

//=====[Declaration of private defines]========================================

#define DISPLAY_REFRESH_TIME_MS 1000

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

DigitalOut engineLed(LED2);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static bool engineState = OFF;
static bool highMode = OFF;
static bool lowMode = OFF;
static bool intMode = OFF;
static bool offMode = OFF;

//=====[Declarations (prototypes) of private functions]========================

static void engineIndicatorUpdate();

static void userInterfaceDisplayInit();
static void userInterfaceDisplayUpdate();

//=====[Implementations of public functions]===================================

void userInterfaceInit()
{
    engineLed = OFF;
    userInterfaceDisplayInit();
}

void userInterfaceUpdate()
{
    engineIndicatorUpdate();
    userInterfaceDisplayUpdate();
}

bool engineStateRead()
{
    return engineState;
}

void engineStateWrite( bool state )
{
    engineState = state;
}

//=====[Implementations of private functions]==================================

static void userInterfaceDisplayInit()
{
    displayInit();
    displayCharPositionWrite( 0,0 );
}

static void userInterfaceDisplayUpdate()
{   
    static int accumulatedDisplayTime = 0;

    if( accumulatedDisplayTime >= DISPLAY_REFRESH_TIME_MS ){
        accumulatedDisplayTime = 0;

        if( engineStateRead() ) {
            displayCharPositionWrite( 0,0 );
            switch( wipersModeRead() ){
                case 1:
                    if(!highMode) {
                        displayStringWrite("Mode is HIGH");
                    }
                    highMode = ON;
                    lowMode = OFF;
                    intMode = OFF;
                    offMode = OFF;
                    break;
                
                case 2:
                    if(!lowMode) {
                        displayStringWrite("Mode is LOW ");
                    }
                    highMode = OFF;
                    lowMode = ON;
                    intMode = OFF;
                    offMode = OFF;
                    break;

                case 3:
                    if(!offMode) {
                        displayStringWrite("Mode is OFF ");
                    }
                    highMode = OFF;
                    lowMode = OFF;
                    intMode = OFF;
                    offMode = ON;
                    break;

                case 4:
                    if(!intMode) {
                        displayStringWrite("Mode is INT ");
                    }
                    highMode = OFF;
                    lowMode = OFF;
                    intMode = ON;
                    offMode = OFF;
                    break;
                 }
            }
        else {
            displayClear();
            highMode = OFF;
            lowMode = OFF;
            intMode = OFF;
            offMode = OFF;
        }
    }
    else {
        accumulatedDisplayTime = accumulatedDisplayTime + 10;
    }

}

static void engineIndicatorUpdate()
{
    engineLed = engineState;
}