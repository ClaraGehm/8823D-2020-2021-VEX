// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// driveLB              motor         15              
// leftIntake           motor         12              
// rightIntake          motor         19              
// indexerMotor         motor         11              
// flywheel             motor         20              
// driveRB              motor         1               
// driveRF              motor         9               
// driveLF              motor         2               
// eyes                 vision        16              
// BumperG              bumper        G               
// BumperH              bumper        H               
// rightE               encoder       A, B            
// leftE                encoder       E, F            
// Inertial3            inertial      3               
// BallDetect           limit         C               
// ---- END VEXCODE CONFIGURED DEVICES ----
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

int x1;
int y4; 
int kill = 1;
int r = 1;
int g = 1;

void ballKickout()
{
  if(kill == 1)
  {
    if (eyes.takeSnapshot(eyes__BLUEB))
    {
      flywheel.spin(forward, 12.0, voltageUnits::volt);
      Brain.Screen.printAt(150,105, "i did it");
      wait(0.15,seconds);
    }
    else if (eyes.takeSnapshot(eyes__REDB))
    {
      flywheel.spin(reverse, 12.0, voltageUnits::volt);
      Brain.Screen.printAt(150,105, "i did it");
      wait(0.15,seconds);
    }
    else
    { 
      flywheel.stop(coast);
      wait(0.15,seconds);
    }
  }
  else //if(kill == -1)
  {

  }
  wait(20, msec);
}

void changeX()
{
  y4 = -1;
  x1 = x1*-1;
}

void changeY()
{
  x1 = -1;
  y4 = y4 * -1;
}

int main() 
{
  // Initializing Robot Configuration. DO NOT REMOVE!Controller1.Axis3.position(percent) + Controller1.Axis1.position(percent)
  vexcodeInit();

  //sets up the variables for the flywheel function
  x1 = -1;
  y4 = -1;

  //sets up the intakes so that their starting position is at 0 degrees 
  rightIntake.setPosition(0, deg); 
  leftIntake.setPosition(0, deg); 


  while(true)
  {
    //This is for the driving motors
    driveLB.spin(reverse,Controller1.Axis3.value(), percent);
    driveLF.spin(reverse,Controller1.Axis3.value(),percent);
    driveRB.spin(reverse,Controller1.Axis2.value(),percent);
    driveRF.spin(reverse,Controller1.Axis2.value(),percent);

    //kicks out red balls
    /*if (Controller1.ButtonRight.pressing())
    {
      ballKickout();
    }
    else
    {

    }*/
    
    //When L1 is pressed the intake moves forward
    if (Controller1.ButtonL2.pressing())
    {
      if (BumperG.pressing() && BumperH.pressing())
        {
          r = -1;
        }
      else
      {
        r = 1;
        rightIntake.spin(forward, 12.0, voltageUnits::volt);
        leftIntake.spin(forward, 12.0, voltageUnits::volt);
        rightIntake.setStopping(coast);
        leftIntake.setStopping(coast);
      }
    }
    else if(Controller1.ButtonL1.pressing()&& r == 1)//|| (Controller1.ButtonL1.pressing() && r == -1)) //movs out 20 degrees 
    {
      r = 1;
      rightIntake.spin(reverse, 12.0, voltageUnits::volt);
      leftIntake.spin(reverse, 12.0, voltageUnits::volt);
      rightIntake.setStopping(hold);
      leftIntake.setStopping(hold);
    }
    
    else
    {
      rightIntake.stop();
      leftIntake.stop();
    }

    //if (Controller1.ButtonUp.pressing())
    //{
      //kill = kill*-1;
    //}

    //When R1 is pressed indexerMotor moves forward
    if(Controller1.ButtonR1.pressing())
    {
      indexerMotor.spin(forward, 12.0, voltageUnits::volt);
      ballKickout();
    }
    //When R2 is pressed indexerMotor moves backwards
    else if(Controller1.ButtonR2.pressing())
    {
      indexerMotor.spin(reverse, 12.0, voltageUnits::volt);
      ballKickout();
    }
    else
    {
      indexerMotor.stop(coast);
    }
/*
    if(Controller1.ButtonDown.pressing())
    {
      ballKickout();
    }
*/
/*
    //When A is pressed the flywheel moves forward
    //Directional logic------------------------------------------------------------------------------------------------------------------------------------------
    if(x1 == -1 && y4 == -1)
    {
      flywheel.stop(coast);
    }
    else if (x1==1)
    {
      flywheel.spin(forward,12.0,voltageUnits::volt);
    }
    else if (x1==-1 && y4==1)
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
    */
    if(Controller1.ButtonDown.pressing())
    {
      flywheel.spin(forward,-100, pct);
    }
    else
    {
      flywheel.stop();
    }
  }
}