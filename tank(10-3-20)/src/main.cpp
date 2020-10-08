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

int x;
int y; 

void l2()
{
  rightIntake.spin(forward, 12.0, voltageUnits::volt);
  leftIntake.spin(forward, 12.0, voltageUnits::volt);
  wait(1, sec);
  rightIntake.setPosition(0, deg);
  leftIntake.setPosition(0, deg);
}

void changeX()
{
  y = -1;
  x = x*-1;
}

void changeY()
{
  x = -1;
  y = y*-1;
}

int main() 
{
  // Initializing Robot Configuration. DO NOT REMOVE!Controller1.Axis3.position(percent) + Controller1.Axis1.position(percent)
  vexcodeInit();

  //sets up the variables for the flywheel function
  x = -1;
  y = -1;

  //sets up the intakes so that their starting position is at 0 degrees 
  rightIntake.setPosition(0, deg); 
  leftIntake.setPosition(0, deg); 


  while(true)
  {
    //This is for the driving motors
    driveLB.spin(forward,Controller1.Axis3.value(), percent);
    driveLF.spin(forward,Controller1.Axis3.value(),percent);
    driveRB.spin(forward,Controller1.Axis2.value(),percent);
    driveRF.spin(forward,Controller1.Axis2.value(),percent);
    
    //When L1 is pressed the intake moves forward
    if(Controller1.ButtonL1.pressing()) //movs out 20 degrees 
    {
      rightIntake.spin(forward, 12.0, voltageUnits::volt);
      leftIntake.spin(forward, 12.0, voltageUnits::volt);
      rightIntake.spinToPosition(-250, deg, false);
      leftIntake.spinToPosition(-250, deg, true);
    }
    else if (Controller1.ButtonL2.pressing())
    {
      l2();
    }
    else
    {
      rightIntake.stop(coast);
      leftIntake.stop(coast);
    }

    //When R1 is pressed indexerMotor moves forward
    if(Controller1.ButtonR1.pressing())
    {
      indexerMotor.spin(forward, 12.0, voltageUnits::volt);
    }
    //When R2 is pressed indexerMotor moves backwards
    else if(Controller1.ButtonR2.pressing())
    {
      indexerMotor.spin(reverse, 12.0, voltageUnits::volt);
    }
    else
    {
      indexerMotor.stop(coast);
    }

    //When A is pressed the flywheel moves forward
    //Directional logic------------------------------------------------------------------------------------------------------------------------------------------
    if(x==-1 && y==-1)
    {
      flywheel.stop(coast);
    }
    else if (x==1)
    {
      flywheel.spin(forward,12.0,voltageUnits::volt);
    }
    else if (x==-1 && y==1)
    {
      flywheel.spin(reverse,12.0,voltageUnits::volt);
    }
    else
    {
    }

    //Button input-----------------------------------------------------------------------------------------------------------------------------------------------
    if (Controller1.ButtonA.pressing()) //when button A is pressed the first time, the flywheel 
    {
      changeX();
      wait(250,msec);
    }
    else if (Controller1.ButtonB.pressing())
    {
      changeY();
      wait(250,msec);
    }
    else
    {
    }
    //Has all motors stop
  }
}
