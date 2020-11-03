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
  double Kp = .1, Kd = .1, Ki = .026;
  double error = 0, error2 = 0, lastError = 0, lastError2 = 0, totalError = 0, totalError2 = 0;
  double Derivative, Derivative2, P, P2, I, I2, D, D2;
  double motorSpeed, motorSpeed2;
  double target = desiredD;
  double currentDL, currentDR;
  double range = 20;
  int track = 1;

  driveLB.setPosition(0, deg);
  driveRB.setPosition(0, deg);
  driveLF.setPosition(0, deg);
  driveLB.setPosition(0, deg);
//if there is a 2 after it is for right
  while (track == 1)
  {
    currentDL = (driveLB.position(deg) + driveLF.position(deg)) / 2;
    currentDR = (driveRB.position(deg) + driveRF.position(deg)) / 2;
    error = target - currentDL;
    error2 = target - currentDR;
    totalError = totalError + error;
    totalError2 = totalError2 + error2;
    Derivative = (error - lastError);
    Derivative2 = (error2 - lastError2);
    P = error * Kp;
    P2 = error2 * Kp;
    I = totalError * Ki;
    I2 = totalError2 * Ki;
    D = Derivative * Kd;
    D2 = Derivative * Kd;

    motorSpeed = P + I + D;
    motorSpeed2 = P2 + I2 + D2;

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
    
    if (target < 0)
    {
      motorSpeed = motorSpeed * -1;
      motorSpeed2 = motorSpeed2 * -1;
    }
    else 
    {

    }

    driveLB.spin(fwd, motorSpeed, pct);
    driveLF.spin(fwd, motorSpeed, pct);
    driveRB.spin(fwd, motorSpeed2, pct);
    driveRF.spin(fwd, motorSpeed2, pct);

    if ((target - range) >= (currentDL && currentDR) && motorSpeed == 0 && motorSpeed2 == 0) // breaks the loop if desired distance is reached
    {
      track = -1;
    }
//    Controller1.Screen.print(int(currentDL) + "  " + int(currentDR));
  }
  driveLB.setPosition(0, deg);
  driveRB.setPosition(0, deg);
  driveLF.setPosition(0, deg);
  driveLB.setPosition(0, deg);
}

int main() 
{
  // Initializing Robot Configuration. DO NOT REMOVE!Controller1.Axis3.position(percent) + Controller1.Axis1.position(percent)
  vexcodeInit();
  driveLB.setVelocity(100,pct);
  driveRB.setVelocity(100,pct);
  driveLF.setVelocity(100,pct);
  driveRF.setVelocity(100,pct);
  driveLB.spinFor(fwd,1000,deg,false);
  driveRB.spinFor(fwd,1000,deg,false);
  driveLF.spinFor(fwd,1000,deg,false);
  driveRF.spinFor(fwd,1000,deg);
  Brain.Screen.print("Done");
  //forwardPID(50);
}