/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\User                                             */
/*    Created:      Tue Sep 29 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;


//Diameter of the wheel is 2.75 in
void PID(int setpoint)
{
  int sensorValue = 0;
  int error;
  int lastError = 0;
  int integral = 0;
  int derivative;
  float kP = 0.25;
  float kI = 0.25;
  float kD = 0.25;
  int motor_power = 0;
  while (true)
  {
    error = setpoint - sensorValue;
    integral = integral + lastError;
    derivative = error - lastError;
    lastError = error;
    motor_power = (error*kP) + (integral*kI) + (derivative*kD);
    if (motor_power > 100)
    {
      motor_power = 100;
    }
    else
    {
    }
    vex::task::sleep(10);
    //while ( leftEncoderClicks < targetClicks)
    // then put stuff here
  }
}





int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  while (1==1)
  {
    PID(120);
  }
}
