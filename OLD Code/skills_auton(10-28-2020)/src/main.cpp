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

void drive_fwd(int speed, int dist)
{
  driveLB.setVelocity(speed,pct);
  driveRB.setVelocity(speed,pct);
  driveLF.setVelocity(speed,pct);
  driveRF.setVelocity(speed,pct);
  driveLB.spinFor(fwd,dist,deg,false);
  driveRB.spinFor(fwd,dist,deg,false);
  driveLF.spinFor(fwd,dist,deg,false);
  driveRF.spinFor(fwd,dist,deg);
  driveLB.stop(brake);
  driveRB.stop(brake);
  driveLF.stop(brake);
  driveRF.stop(brake);
}

void drive_bwd(int speed, int dist)
{
  driveLB.setVelocity(-speed,pct);
  driveRB.setVelocity(-speed,pct);
  driveLF.setVelocity(-speed,pct);
  driveRF.setVelocity(-speed,pct);
  driveLB.spinFor(fwd,-dist,deg,false);
  driveRB.spinFor(fwd,-dist,deg,false);
  driveLF.spinFor(fwd,-dist,deg,false);
  driveRF.spinFor(fwd,-dist,deg);
  driveLB.stop(brake);
  driveRB.stop(brake);
  driveLF.stop(brake);
  driveRF.stop(brake);
}

void drive_tr(/*int speed,*/ int target)
{
  InertialSensor.setHeading(0,degrees);
  wait(100,msec);
  int speed = (target-InertialSensor.rotation(degrees))/3;
  while(InertialSensor.rotation(degrees) < target)
  {
    driveLB.spin(reverse,speed,pct);
    driveLF.spin(reverse,speed,pct);
    driveRF.spin(forward,speed,pct);
    driveRB.spin(forward,speed,pct);
  }
  driveLB.stop(brake);
  driveLF.stop(brake);
  driveRB.stop(brake);
  driveRF.stop(brake);
}

void drive_tl(int target)
{
  InertialSensor.setHeading(0,degrees);
  wait(100,msec);
  int speed = (target-InertialSensor.rotation(degrees))/3;
  while(InertialSensor.rotation(degrees) < target)
  {
    driveLB.spin(reverse,speed,pct);
    driveLF.spin(reverse,speed,pct);
    driveRF.spin(forward,speed,pct);
    driveRB.spin(forward,speed,pct);
    //printf(degrees);
  }
  driveLB.stop(brake);
  driveLF.stop(brake);
  driveRB.stop(brake);
  driveRF.stop(brake);
}

void intake_open()
{
  while(!BumperG.pressing())
  {
  leftIntake.setVelocity(-100,pct);
  rightIntake.setVelocity(-100,pct);
  leftIntake.spin(fwd);
  rightIntake.spin(fwd);
  }
  leftIntake.stop(hold);
  rightIntake.stop(hold);
}


int main() 
{
  // Initializing Robot Configuration. DO NOT REMOVE!Controller1.Axis3.position(percent) + Controller1.Axis1.position(percent)
  vexcodeInit();
  InertialSensor.calibrate();
  wait(3, sec);
  flywheel.spin(fwd, -100, pct);
  intake_open();
  drive_fwd(80,1000);
  flywheel.stop(coast);
  drive_tr(120);
}