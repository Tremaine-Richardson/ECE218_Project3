//=====[Libraries]=============================================================

#include "potentiometer.h"
#include "mbed.h"
#include "arm_book_lib.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

AnalogIn potentiometer1(A0);
AnalogIn potentiometer2(A1);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

float wiperMode() 
{
    return potentiometer1.read();
}

float delayTime()
{
    return potentiometer2.read();
}

//=====[Implementations of private functions]==================================
