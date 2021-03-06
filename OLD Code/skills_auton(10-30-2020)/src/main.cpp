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

int kill = 1;

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
  InertialSensor.setRotation(0,degrees);
  wait(100,msec);
  int speed = 50; // (target-InertialSensor.rotation(degrees))/3;
  while(InertialSensor.rotation(degrees) < target)
  {
    driveLB.spin(forward,speed,pct);
    driveLF.spin(forward,speed,pct);
    driveRF.spin(reverse,speed,pct);
    driveRB.spin(reverse,speed,pct);
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
  while(InertialSensor.rotation(degrees) > -target)
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
  while(!BumperG.pressing() && !BumperH.pressing())
  {
  leftIntake.setVelocity(-100,pct);
  rightIntake.setVelocity(-100,pct);
  leftIntake.spin(fwd);
  rightIntake.spin(fwd);
  Brain.Screen.print(BumperG.value());
  }
  leftIntake.stop(hold);
  rightIntake.stop(hold);
}

void intake_close(int stay)
{
  leftIntake.setVelocity(-100,pct);
  rightIntake.setVelocity(-100,pct);
  leftIntake.spin(reverse);
  rightIntake.spin(reverse);
  wait(stay, sec);
  leftIntake.stop(coast);
  rightIntake.stop(coast);
}

void ballKickout(int nap)
{
  if(kill == 1)
  {
    if (eyes.takeSnapshot(eyes__BLUEB))
    {
      flywheel.spin(forward, 12.0, voltageUnits::volt);
      Brain.Screen.printAt( 150,105, "i did it");
    }
    else if (eyes.takeSnapshot(eyes__REDB))
    {
      flywheel.spin(reverse, 12.0, voltageUnits::volt);
      Brain.Screen.printAt( 150,105, "i did it");
    }
    else
    { 
    }
  }
  else //if(kill == -1)
  {

  }
  wait(20, msec);
}

int main() 
{
  // Initializing Robot Configuration. DO NOT REMOVE!Controller1.Axis3.position(percent) + Controller1.Axis1.position(percent)
  vexcodeInit();
  //InertialSensor.calibrate();
  //scores the ball in the goal
  wait(1, sec);
  flywheel.spin(fwd, -100, pct);
  intake_open();

  //moves forward
  drive_fwd(60,1290);
  flywheel.stop(coast);

  //waits a second so the ball can go in
  wait(.25, sec);

  //intakes ball
  intake_close(1);

  //moves toward corner goal
  drive_fwd(60, 1000);
  drive_tl(50);
  //intake_open();
  drive_fwd(60, 999);

  //cycles out the balls
  indexerMotor.spin(reverse, 100, percent);
  //intake_close(2);
  flywheel.spin(reverse, 100, percent);
  wait(3, sec);
  flywheel.spin(forward);
  intake_close(5);
}