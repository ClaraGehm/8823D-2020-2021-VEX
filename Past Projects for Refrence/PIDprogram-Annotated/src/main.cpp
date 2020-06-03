/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Sam B. and Clara G.                                       */
/*    Created:      Sun Jan 26 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:s
// [Name]               [Type]        [Port(s)]
// leftDrive            motor         1               
// Controller1          controller                    
// rightDrive           motor         10              
// claw                 motor         3               
// arm                  motor         8               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

double dabs (double num)
{
  if (num < 0)
  {
    num *= -1;
  }
  return num;
}

/*double rampUp = 0;
double accelerate (double error, double rampBy)
{
  if(rampUp < 1)
  {
    rampUp += rampBy;
  }
  error *= rampUp;
  return error;
}*/

void printRotations(motor l, motor r)
{
  Brain.Screen.setCursor(1,1);
  Brain.Screen.print("Left rotations: ");
  Brain.Screen.print(l.rotation(degrees));
  Brain.Screen.setCursor(2,1);
  Brain.Screen.print("Right rotations: ");
  Brain.Screen.print(r.rotation(degrees));
}

void printSpeed(double speed, double speed2)
{
  Brain.Screen.setCursor(3,1);
  Brain.Screen.print("Left speed: ");
  Brain.Screen.print(speed);
  Brain.Screen.setCursor(4,1);
  Brain.Screen.print("Right speed: ");
  Brain.Screen.print(speed2);

}

void consoleTrack (double spd, double spd2, double err, double err2, double drv, double drv2, double integral, double integral2)
{
  printf("time:%f Left motor: speed %f, error %f, derivative %f, integral %f\n", Brain.Timer.value(), spd, err, drv, integral);
  printf("Right motor: speed %f, error %f, derivative %f, integral %f\n", spd2, err2, drv2, integral2);
}

//make into class (driving.go)
void go (double target, motor sally, motor bob, bool there)  
{
  double kP = 1;
  double kD = 0.2;
  double kI = 0.005;
  double deltaTime = 0.004; //this is one of the sides of the rectangles that represents the integral
  double error = target; 
  double error2 = target;
  double integral = 0;
  double integral2 = 0;
  double derivative = 0;
  double derivative2 = 0;
  double prevErr = 0;
  double prevErr2 = 0;
  double range = 5;
  double currentDistance = sally.rotation(deg);
  double currentDistance2 = bob.rotation(deg);
  double speed;
  double speed2;
  double overflow = 250;
  double cap = 5000000;
  //double rampBy = 0.05;
  int counter = 0;
  //clear encoders

  while((dabs(error) > range) || (dabs(error2) > range))
  {
    currentDistance = sally.rotation(deg);
    error = target - currentDistance;
    currentDistance2 = bob.rotation(deg);
    error2 = target - currentDistance2;
    
    integral += error * deltaTime;
    derivative = error - prevErr;
    integral2 += error2 * deltaTime;
    derivative2 = error2 - prevErr2;
    
    if (dabs(error) > overflow)
    {
      integral = 0;
    }
    else if(dabs(error) < range)
    {
      integral = 0;
    }

    if (dabs(error2) > overflow)
    {
      integral2 = 0;
    }
    else if (dabs(error2) < range)
    {
      integral2 = 0;
    }
    //target: 50 speed influence points max.
    if (integral > cap) 
    {
      integral = cap;
    }
    else if (integral < -cap)
    {
      integral = -cap;
    }

    if (integral2 > cap) 
    {
      integral2 = cap;
    }
    else if (integral2 < -cap)
    {
      integral2 = -cap;
    }


    
    speed = (error * kP) + (integral * kI) + (derivative * kD);
    speed2 = (error2 * kP) + (integral2 * kI) + (derivative2 * kD);

    sally.spin(forward, speed, dps); //dps deals in far larger numbers than percent
    bob.spin(forward, speed2, dps);

    prevErr = error;
    prevErr2 = error2;

    counter++;
    if(counter % 1500 == 0)
    {
      consoleTrack(speed, speed2, error, error2, derivative, derivative2, integral, integral2);
    }
    task::sleep(25);
  }

  
  sally.stop();
  bob.stop();
  there = true;
  Brain.Screen.newLine();
  Brain.Screen.print("Done");
}

int main() 
{
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  bool done = true;

  go(1000, leftDrive, rightDrive, done);
}
