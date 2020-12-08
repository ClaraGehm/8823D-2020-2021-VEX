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

void ballKickout()
{
  if(1 == 1)
  {
    if (eyes.takeSnapshot(eyes__BLUEB))
    {
      flywheel.spin(forward, 12.0, voltageUnits::volt);
      Brain.Screen.printAt( 150,105, "i did it");
      wait(0.1,seconds);
    }
    else if (eyes.takeSnapshot(eyes__REDB))
    {
      flywheel.spin(reverse, 12.0, voltageUnits::volt);
      Brain.Screen.printAt( 150,105, "i did it");
      wait(0.1,seconds);
    }
    else
    { 
      flywheel.stop(coast);
      wait(0.1,seconds);
    }
  }
  else //if(kill == -1)
  {

  }
  wait(20, msec);
}

void forwardPID(double desiredD)
{
  double Kp = .1, Kd = .1;
  double error = 0, error2 = 0, lastError = 0, lastError2 = 0;
  double Derivative, Derivative2, P, P2, D, D2;
  double motorSpeed, motorSpeed2;
  double target = desiredD;
  double currentDL, currentDR;
  double range = 250;
  driveLB.setPosition(0,deg);
  driveRB.setPosition(0,deg);
//if there is a 2 after it is for right
  while (1 == 1)
  {
    currentDL = (driveLB.position(deg) + driveLF.position(deg)) / 2;
    currentDR = (driveRB.position(deg) + driveRF.position(deg)) / 2;
    error = target - currentDL;
    error2 = target - currentDR;
    Derivative = (error - lastError);
    Derivative2 = (error2 - lastError2);
    P = error * Kp;
    P2 = error2 * Kp;
    D = Derivative * Kd;
    D2 = Derivative * Kd;

    motorSpeed = P + D;
    motorSpeed2 = P2 + D2;

    //Makes sure the motos speed does not go over 95 percent
    if (motorSpeed > 95)
    {
      motorSpeed = 95;
    }
    else
    {

    }
    if (motorSpeed2 > 95)
    {
      motorSpeed2 = 95;
    }
    else
    {

    }

    //Creates a range the robot can stop in so that it is not constantly running
    if (currentDL >= (desiredD - range))
    {
      motorSpeed = 0;
    }
    else
    {

    }
    if (currentDR >= (desiredD - range))
    {
      motorSpeed2 = 0;
    }
    else
    {

    }

    driveLB.spin(fwd, motorSpeed, pct);
    driveLF.spin(fwd, motorSpeed, pct);
    driveRB.spin(fwd, motorSpeed2, pct);
    driveRF.spin(fwd, motorSpeed2, pct);
  }
}

int main() 
{
  // Initializing Robot Configuration. DO NOT REMOVE!Controller1.Axis3.position(percent) + Controller1.Axis1.position(percent)
  vexcodeInit();
  forwardPID(500);
  //Brain.Screen.print("Done");
  forwardPID(300);
}