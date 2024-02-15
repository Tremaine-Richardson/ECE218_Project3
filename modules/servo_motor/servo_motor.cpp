//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "servo_motor.h"

//=====[Declaration of private defines]========================================

#define PERIOD          .02

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

PwmOut servo(PF_9);

//=====[Declaration of external public global variables]=======================

float servoPos = DUTY_ZERO;

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

void servoInit()
{
    servo.period(PERIOD);
}

void servoDutyCycleWrite( float cycle )
{
    servo.write( cycle );
    servoPos = cycle;
}

float servoPositionRead()
{
    return servoPos;
}

//=====[Implementations of private functions]==================================
