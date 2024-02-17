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
            displayStringWrite("Mode is");
            displayCharPositionWrite( 8,0 );
            switch( wipersModeRead() ){
                case 1:
                    displayStringWrite("HIGH");
                    break;
                
                case 2:
                    displayStringWrite("LOW ");
                    break;

                case 3:
                    displayStringWrite("OFF ");
                    break;

                case 4:
                    displayStringWrite("INT ");
                    break;
                 }
            }
        else {
            displayClear();
        }
    }
    else {
        accumulatedDisplayTime =
            accumulatedDisplayTime + SYSTEM_TIME_INCREMENT_MS;        
    } 
}

static void engineIndicatorUpdate()
{
    engineLed = engineState;
}