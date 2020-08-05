/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\cjgeh                                            */
/*    Created:      Mon Jun 29 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// driveLB              motor         1               
// leftIntake           motor         2               
// rightIntake          motor         3               
// indexerMotor         motor         4               
// flywheel             motor         8               
// driveRB              motor         10              
// driveRF              motor         9               
// driveLF              motor         5               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int main() 
{
  // Initializing Robot Configuration. DO NOT REMOVE!Controller1.Axis3.position(percent) + Controller1.Axis1.position(percent)
  vexcodeInit();
  
  while(true)
  {
    //This is for the driving motors
    driveLB.spin(forward,Controller1.Axis3.value(),percent);
    driveLF.spin(forward,Controller1.Axis3.value(),percent);
    driveRB.spin(forward,Controller1.Axis2.value(),percent);
    driveRF.spin(forward,Controller1.Axis2.value(),percent);
    
    //When L1 is pressed the intake moves forward
    if(Controller1.ButtonL1.pressing())
    {
      rightIntake.setVelocity(50, percent);
      leftIntake.setVelocity(50, percent);
      rightIntake.spin(forward);
      leftIntake.spin(forward);
    }
    else
    {
      rightIntake.stop(coast);
      leftIntake.stop(coast);
    }
    //When L2 is pressed the intake moves backwards
    if(Controller1.ButtonL2.pressing())
    {
      rightIntake.setVelocity(50, percent);
      leftIntake.setVelocity(50, percent);
      rightIntake.spin(reverse);
      leftIntake.spin(reverse);
    }
    else
    {
      rightIntake.stop(coast);
      leftIntake.stop(coast);
    }
    //When R1 is pressed indexerMotor moves forward
    if(Controller1.ButtonR1.pressing())
    {
      indexerMotor.setVelocity(50, percent);
      indexerMotor.spin(forward);
    }
    else
    {
      indexerMotor.stop(hold);
    }
    //When R2 is pressed indexerMotor moves backwards
    if(Controller1.ButtonR2.pressing())
    {
      indexerMotor.setVelocity(50, percent);
      indexerMotor.spin(reverse);
    }
    else
    {
      indexerMotor.stop(hold);
    }
    //When A is pressed the flywheel moves forward
    if(Controller1.ButtonA.pressing())
    {
      flywheel.setVelocity(75, percent);
      flywheel.spin(forward);
    }
    else
    {
      flywheel.stop(hold);
    }
    //When B is pressed the flywheel moves backwards
    if(Controller1.ButtonB.pressing())
    {
      flywheel.setVelocity(75, percent);
      flywheel.spin(reverse);
    }
    else
    {
      flywheel.stop(hold);
    }
    //Has all motors stop
  }
}
