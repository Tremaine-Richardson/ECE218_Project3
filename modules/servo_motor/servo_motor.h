//=====[#include guards - begin]===============================================

#ifndef _SERVO_MOTOR_H_
#define _SERVO_MOTOR_H_

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================

//=====[Declarations (prototypes) of public functions]=========================

void servoInit();
void servoDutyCycleWrite( float cycle );
float servoPositionRead();

//=====[#include guards - end]=================================================

#endif // _SERVO_MOTOR_H_