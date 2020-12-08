/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// InertialSensor       inertial      3               
// leftE                encoder       E, F            
// rightE               encoder       A, B            
// eyes                 vision        16              
// BumperG              bumper        G               
// BumperH              bumper        H               
// BallExit             limit         D               
// Controller1          controller                    
// driveLB              motor         15              
// leftIntake           motor         12              
// rightIntake          motor         19              
// indexerMotor         motor         11              
// flywheel             motor         20              
// driveRB              motor         1               
// driveRF              motor         9               
// driveLF              motor         2               
// OpticalSensor        optical       7               
// DistanceSensor       distance      6               
// BallDetect           bumper        C               
// ---- END VEXCODE CONFIGURED DEVICES ----


#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) 
{

  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
   InertialSensor.calibrate();
  wait(1.5, seconds);
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}
int kill = 1;

void drive_fwd(int speed, int dist)
{
  leftE.setPosition(0, deg);
  rightE.setPosition(0, deg);
  Brain.Screen.setPenColor(white);
  while (-leftE.value() <= dist && rightE.value() <= dist)
  {
    if(-leftE.value() == rightE.value())
    {
      driveLB.spin(fwd, speed, pct);
      driveRB.spin(fwd, speed, pct);
      driveLF.spin(fwd, speed, pct);
      driveRF.spin(fwd, speed, pct);
    }
    /*
    else if(-leftE.value() < rightE.value())
    {
      driveLB.spin(fwd, speed+5, pct);
      driveRB.spin(fwd, speed, pct);
      driveLF.spin(fwd, speed+5, pct);
      driveRF.spin(fwd, speed, pct);
    }
    else if(-leftE.value() > rightE.value())
    {
      driveLB.spin(fwd, speed, pct);
      driveRB.spin(fwd, speed+5, pct);
      driveLF.spin(fwd, speed, pct);
      driveRF.spin(fwd, speed+5, pct);
    }
    */
    wait(20, msec);
    Brain.Screen.clearScreen();
    //Brain.Screen.printAt(50, 50, "heading %5.2d", rightE.value());
    //Brain.Screen.printAt(50, 100, "heading %5.2d", leftE.value());
  }
  driveLB.stop(brake);
  driveRB.stop(brake);
  driveLF.stop(brake);
  driveRF.stop(brake);
}

void drive_bwd(int speed, int dist)
{
  leftE.setPosition(0, deg);
  rightE.setPosition(0, deg);
  while (leftE.value() <= dist && rightE.value() <= dist)
  {
    if(leftE.value() == rightE.value())
    {
      driveLB.spin(reverse, speed, pct);
      driveRB.spin(reverse, speed, pct);
      driveLF.spin(reverse, speed, pct);
      driveRF.spin(reverse, speed, pct);
    }
    /*
    else if(leftE.value() < rightE.value())
    {
      driveLB.spin(reverse, speed-5, pct);
      driveRB.spin(reverse, speed, pct);
      driveLF.spin(reverse, speed-5, pct);
      driveRF.spin(reverse, speed, pct);
    }
    else if(leftE.value() > rightE.value())
    {
      driveLB.spin(reverse, speed, pct);
      driveRB.spin(reverse, speed-5, pct);
      driveLF.spin(reverse, speed, pct);
      driveRF.spin(reverse, speed-5, pct);
    }
    */
  }
  driveLB.stop(brake);
  driveRB.stop(brake);
  driveLF.stop(brake);
  driveRF.stop(brake);
}

void drive_tr(int target)
{
  InertialSensor.setRotation(0,degrees);
  wait(100,msec);
  int speed = (target-InertialSensor.rotation(degrees))/3;
  while(InertialSensor.rotation(degrees) < target)
  {
    driveLB.spin(forward,speed,pct);
    driveLF.spin(forward,speed,pct);
    driveRF.spin(reverse,speed,pct);
    driveRB.spin(reverse,speed,pct);
    Brain.Screen.printAt(50,50, "5.4f", InertialSensor.value());
  }
  driveLB.stop(brake);
  driveLF.stop(brake);
  driveRB.stop(brake);
  driveRF.stop(brake);
}

void drive_tl(int target)
{
  InertialSensor.setRotation(0,degrees);
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
  leftIntake.setVelocity(-95,pct);
  rightIntake.setVelocity(-95,pct);
  leftIntake.spin(fwd);
  rightIntake.spin(fwd);
  //Brain.Screen.print(BumperG.value());
  }
  leftIntake.stop(hold);
  rightIntake.stop(hold);
}

void intake_close(float stay)
{
  leftIntake.setVelocity(-100,pct);
  rightIntake.setVelocity(-100,pct);
  leftIntake.spin(reverse);
  rightIntake.spin(reverse);
  wait(stay, sec);
  leftIntake.stop(coast);
  rightIntake.stop(coast);
}

void ballKickout()
{
  OpticalSensor.setLightPower(25, percent);
  
  if(DistanceSensor.objectDistance(inches) < 1.4)
  {
    if (OpticalSensor.hue() < 16)
    {
        Brain.Screen.setPenColor(red);
        Brain.Screen.printAt(200,100,"Red Ball Found");
     
      if(!BallExit.pressing())
      {
        flywheel.spin(reverse, 12.0, voltageUnits::volt);
      }
    
      else if(BallExit.pressing())
      {
        wait(100,msec);
        flywheel.stop();
        Brain.Screen.setPenColor(red); 
        Brain.Screen.printAt(200,130,"Red Ball Fired");

        Controller1.Screen.clearLine(160); 
      } 
    }    
      
  
      else if (OpticalSensor.hue()>=225 && OpticalSensor.hue()<=245)
      {
        flywheel.spin(forward, 12.0, voltageUnits::volt);
        Brain.Screen.setPenColor(blue);
        Brain.Screen.printAt(200,160, "Blue was not The Imposter");
        wait(0.18,seconds);
        flywheel.stop();

      }
  }
      else if(DistanceSensor.objectDistance(inches) >= 1.4)
      {
        if(BallExit.pressing())
        { 
          wait(100,msec);
          flywheel.stop();
          Brain.Screen.setPenColor(red); 
          Brain.Screen.printAt(200,130,"Red Ball Fired");

          Controller1.Screen.clearLine(160); 
     
        }
      } 
}
 

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) 
{
  intake_open();
  flywheel.spin(reverse);
  wait(.1, seconds);
  drive_fwd(30, 1700);
  drive_tr(15, 3);
  drive_fwd(30, 200);
  while(!(BallExit.value() == 1))
  {
    indexerMotor.spin(reverse, 12.0, voltageUnits::volt);
    flywheel.spin(reverse, 12.0, voltageUnits::volt);
  }
  wait(.5, seconds);
  indexerMotor.stop();
  flywheel.stop();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) 
{
  // User control code here, inside the loop
  while (1) 
  { //This is for the driving motors
    driveLB.spin(forward,Controller1.Axis3.value(), percent);
    driveLF.spin(forward,Controller1.Axis3.value(),percent);
    driveRB.spin(forward,Controller1.Axis2.value(),percent);
    driveRF.spin(forward,Controller1.Axis2.value(),percent);

    
    //When L1 is pressed the intake moves forward
    if (Controller1.ButtonL2.pressing())
    {
      rightIntake.spin(forward, 12.0, voltageUnits::volt);
      leftIntake.spin(forward, 12.0, voltageUnits::volt);
      rightIntake.setStopping(coast);
      leftIntake.setStopping(coast);
    }
      
    else if(Controller1.ButtonL1.pressing())//|| (Controller1.ButtonL1.pressing() && r == -1)) //movs out 20 degrees 
    {
       if (BumperG.pressing() && BumperH.pressing())
      {
        //r = -1;
        rightIntake.stop();
        leftIntake.stop();
      }
      //r = 1;
      else
      {
        rightIntake.spin(reverse, 12.0, voltageUnits::volt);
        leftIntake.spin(reverse, 12.0, voltageUnits::volt);
        rightIntake.setStopping(hold);
        leftIntake.setStopping(hold);
      }
     
    }
    else //if (!Controller1.ButtonL1.pressing()&&!Controller1.ButtonL2.pressing())
    {
      rightIntake.stop();
      leftIntake.stop();
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
      ballKickout();
    }
    else
    {
      indexerMotor.stop(coast);
    }

    if(Controller1.ButtonDown.pressing())
    {
      flywheel.spin(reverse,12.0,voltageUnits::volt);
    }
    
    else if(Controller1.ButtonLeft.pressing())
    {
      flywheel.stop();
    }
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() 
{
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) 
  {
    wait(100, msec);
  }
}
