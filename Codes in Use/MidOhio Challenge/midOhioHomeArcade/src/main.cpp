/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\David                                            */
/*    Created:      Sun Feb 07 2021                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// driveLF              motor         15              
// driveRF              motor         16              
// Controller1          controller                    
// driveLB              motor         5               
// driveRB              motor         6               
// flywheel             motor         9               
// indexerMotor         motor         2               
// leftIntake           motor         10              
// rightIntake          motor         20              
// BallExit             limit         H               
// BallDetect           limit         G               
// leftEncoder          encoder       C, D            
// rightEncoder         encoder       A, B            
// leftInertial         inertial      1               
// rightInertial        inertial      19              
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
  int motorSpeedCap = 5;  //maximum motorSpeed starts at this value, but will grow every few msecs 
  int step = 2;
/////////////////////////////Drive Forward Variables///////////////////////////////////
double currentRotationsL = 0;
double currentRotationsR = 0;
double prevRotationsL = 0;
double prevRotationsR = 0;
int firstLoop = 0;
///////////////////////////Proportional Turn variables//////////////////////////////////////////////////////////////////

  double turnkP = .7;     //tuning value for our turnError.
  double turnSpeed = 0; // motor speed, starts at zero.
  double turnError; // the difference between our turnTarget and our current position
  double avgRotation;
///////////////// TURN RIGHT PROPORTIONAL FUNCTION //////////////////////////////////////////////////////////////////////

void drive_tr(int target) //setting target as a parameter to be
// defined every time function is called
{ 
  avgRotation = 0;
  leftInertial.setRotation(0,degrees);
  rightInertial.setRotation(0,degrees);
  wait(300,msec); //inertialSensor resets with every call of the functon

  while(avgRotation < target) // loop runs until robot has reached target.
  { 
    avgRotation = (leftInertial.rotation(degrees) + rightInertial.rotation(degrees))/2;  
    Brain.Screen.printAt(50,50,"avgRotation %f",avgRotation);
    
    //setting up error as required travel distance.  
    turnError = (target-avgRotation);
    
    Brain.Screen.printAt( 70,70,"turnError %f",turnError);
    turnSpeed = (turnError*turnkP); //tune error with kP and assign value to turnSpeed.
    
    Brain.Screen.printAt(90,90,"turnSpeed %f",turnSpeed);

      //maximum motorspeed: 40%
    if(turnSpeed < 7)
    {turnSpeed = 7;}
      //mininum motorspeed: 10%

    driveLB.spin(forward,turnSpeed,pct);
    driveLF.spin(forward,turnSpeed,pct);
    driveRF.spin(reverse,turnSpeed,pct);
    driveRB.spin(reverse,turnSpeed,pct);
    //assign motor speed to constantly updating turnSpeed variable.
    wait(20,msec);
  }

  Brain.Screen.print("Exited while loop");
  driveLB.stop(coast);
  driveRB.stop(coast);
  driveLF.stop(coast);
  driveRF.stop(coast);
  wait(200,msec);
    
  while(turnError < 0)
  {   // if and when robot overshoots, run a smaller 
    // proportional turnin the other direction to correct
  avgRotation = (leftInertial.rotation(degrees) + rightInertial.rotation(degrees))/2;  
  turnError = (target-avgRotation);
    
  turnSpeed = (turnError*turnkP);

  //setting smaller negative min motorspeeds
  if (turnSpeed > -7)
  {
    turnSpeed = -7;
  } //absolute value of motor speed never dips below 7%
    // (avoids steadystate error).
   
    driveLB.spin(forward,turnSpeed,pct);
    driveLF.spin(forward,turnSpeed,pct);
    driveRF.spin(reverse,turnSpeed,pct);
    driveRB.spin(reverse,turnSpeed,pct);
 }

  driveLB.stop(coast);
  driveRB.stop(coast);
  driveLF.stop(coast);
  driveRF.stop(coast);
}

////////////// TURN LEFT PROPORTIONAL FUNCTION ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

void drive_tl(int target) //setting target as a parameter to be
// defined every time function is called
{
  leftInertial.setRotation(0,degrees); 
  rightInertial.setRotation(0,degrees);
  avgRotation = 0;  
  wait(300,msec); //inertialSensor resets with every call of the functon

  while(avgRotation > target) // loop runs until robot has reached target.
  { 
    avgRotation = (leftInertial.rotation(degrees) + rightInertial.rotation(degrees))/2;  
    Brain.Screen.printAt(50,50,"avgRotation: %f",avgRotation);
    
    //setting up error as required travel distance.  
    turnError = target-avgRotation;
    
    Brain.Screen.printAt(50,70,"turnError %f",turnError);
    turnSpeed = turnError*turnkP; //tune error with kP and assign value to turnSpeed.
    
    Brain.Screen.printAt(50,90,"turnSpeed %f",turnSpeed);

    if(turnSpeed > -10)
    {turnSpeed = -10;}
    
    driveLB.spin(forward,turnSpeed,pct);
    driveLF.spin(forward,turnSpeed,pct);
    driveRF.spin(reverse,turnSpeed,pct);
    driveRB.spin(reverse,turnSpeed,pct);
    //assign motor speed to constantly updating turnSpeed variable.
    wait(20,msec);
  }

  Brain.Screen.print("Exited while loop");
  driveLB.stop(coast);
  driveRB.stop(coast);
  driveLF.stop(coast);
  driveRF.stop(coast);
  wait(200,msec);
    
  while(turnError > 0)
  {   // if and when robot overshoots, run a smaller 
    // proportional turning the other direction to correct
  avgRotation = (leftInertial.rotation(degrees) + rightInertial.rotation(degrees))/2;  
  turnError = (target-avgRotation);
    
  turnSpeed = (turnError*turnkP);

  //setting smaller negative min motorspeeds
  if (turnSpeed < 7)
  {
    turnSpeed = 7;
  } //absolute value of motor speed never dips below 7%
    // (avoids steadystate error).
   
    driveLB.spin(forward,turnSpeed,pct);
    driveLF.spin(forward,turnSpeed,pct);
    driveRF.spin(reverse,turnSpeed,pct);
    driveRB.spin(reverse,turnSpeed,pct);
 }

  driveLB.stop(coast);
  driveRB.stop(coast);
  driveLF.stop(coast);
  driveRF.stop(coast);
}
void drive_fwd(int speed, int dist)
{
  Brain.Timer.reset();
  leftEncoder.setPosition(0, deg);
  rightEncoder.setPosition(0, deg);
  Brain.Screen.setPenColor(white);
  while (Brain.Timer.value() < 0.3)
  {
    Brain.Screen.printAt(50, 200, "entered loop");
    //reseting the values so that they are updates
    prevRotationsL = currentRotationsL;
    currentRotationsL = -leftEncoder.value();
    prevRotationsR = currentRotationsR;
    currentRotationsR = rightEncoder.value();
    
    
    //telling the motors to move
    driveLB.spin(fwd, speed, pct);
    driveRB.spin(fwd, speed, pct);
    driveLF.spin(fwd, speed, pct);
    driveRF.spin(fwd, speed, pct);

    //checks to see if the robot is above the number of ticks it should go
    if(-leftEncoder.value() >= dist && rightEncoder.value() >= dist)
    {
      driveLB.stop(brake);
      driveRB.stop(brake);
      driveLF.stop(brake);
      driveRF.stop(brake);
      //Brain.Screen.printAt(50, 150, "Not supposed to be here");
    }
    //if the robot is stuck it will stop it
    else if((prevRotationsL == currentRotationsL || prevRotationsR == currentRotationsR) && firstLoop == 1)
    {
      Brain.Screen.printAt(50, 150, "Ran into goal");
    }
    //if neither of those things are true it resets the Brain Timer 
    else
    {
      Brain.Timer.reset();
    }
    firstLoop = 1;
    wait(20, msec);
    
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(50, 50, "heading %5.2d", rightEncoder.value());
    Brain.Screen.printAt(50, 100, "heading %5.2d", leftEncoder.value());
  }
  driveLB.stop(brake);
  driveRB.stop(brake);
  driveLF.stop(brake);
  driveRF.stop(brake);
  Brain.Screen.printAt(50, 150, "done:");
}

void drive_fwdNoStop(int speed, int dist)
  { 
    leftEncoder.setPosition(0, deg);
  rightEncoder.setPosition(0, deg);
  Brain.Screen.setPenColor(white);
  while (leftEncoder.value() <= dist && -rightEncoder.value() <= dist) {
    if (leftEncoder.value() == -rightEncoder.value()) 
    {
      driveLB.spin(fwd, speed, pct);
      driveRB.spin(fwd, speed, pct);
      driveLF.spin(fwd, speed, pct);
      driveRF.spin(fwd, speed, pct);
    }
    wait(20, msec);
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(50, 50, "heading %5.2d", rightEncoder.value());
    Brain.Screen.printAt(50, 100, "heading %5.2d", leftEncoder.value());
  }
}

void drive_bwd(int speed, int dist) {
  leftEncoder.setPosition(0, deg);
  rightEncoder.setPosition(0, deg);
  Brain.Screen.setPenColor(white);
  while (-leftEncoder.value() <= dist && rightEncoder.value() <= dist) {
    if (leftEncoder.value() == -rightEncoder.value()) {
      driveLB.spin(reverse, speed, pct);
      driveRB.spin(reverse, speed, pct);
      driveLF.spin(reverse, speed, pct);
      driveRF.spin(reverse, speed, pct);
    }
    wait(20, msec);
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(50, 50, "rotation %5.2d", rightEncoder.value());
    Brain.Screen.printAt(50, 100, "rotation %5.2d", leftEncoder.value());
  }
  driveLB.stop(brake);
  driveRB.stop(brake);
  driveLF.stop(brake);
  driveRF.stop(brake);
}

void intake_open() {
  leftIntake.spin(reverse, 100, percent);
  rightIntake.spin(reverse, 100, percent);
  wait(500, msec);
  leftIntake.stop(hold);
  rightIntake.stop(hold);
}
void intake_close(float stay) {
  leftIntake.setVelocity(-100, pct);
  rightIntake.setVelocity(-100, pct);
  leftIntake.spin(reverse);
  rightIntake.spin(reverse);
  wait(stay, sec);
  leftIntake.stop(coast);
  rightIntake.stop(coast);
}
  
////////////////////////////////// Drive Forward Functions//////////////////////////////////////////

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  leftInertial.calibrate();
  rightInertial.calibrate();
  wait(3, sec);

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
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
  // scores the ball in the goal
  flywheel.spin(fwd, 100, pct);
  wait(300, msec);
  intake_open();

  // moves forward
  drive_fwd(50, 100);
  flywheel.stop(coast);
  drive_fwd(30, 1350);  //VALUE MIGHT NEED TO BE CHANGED 

  // moves toward corner goal
  drive_tl(-72);
  //intake_open();
  drive_fwd(50, 680);

  // detects for the first red ball it go through the system
  while (BallExit.value() == 0) {
    rightIntake.spin(forward, 12.0, voltageUnits::volt);
    leftIntake.spin(forward, 12.0, voltageUnits::volt);
    indexerMotor.spin(forward, 12.0, voltageUnits::volt);
    flywheel.spin(forward, 12.0, voltageUnits::volt);
  }

  // waits for the sensor to return to origonal position
  while (BallExit.value() == 1) {
  }

  // once the sensor = 1 (ball triggering it) it exits the loop
  while (BallExit.value() == 0) {
  }

  leftIntake.stop(coast);
  rightIntake.stop(coast);
  // waits for the ball to leave the flywheel and turns it off
  wait(300, msec);
  flywheel.stop(hold);

  /*
  //makes the intakes spin in reverse so that we don't take any of our color
  ball with us leftIntake.spin(reverse); rightIntake.spin(reverse);
  */
  wait(300, msec);
  //intake_close(.4);

  // backs away so that all balls count for us
  drive_bwd(40, 2100);

  //turns flywheel on to discard the red ball and turns on intakes on so that when we aproach the corner goal it imediatly picks up the balls
  flywheel.spin(forward, 100, pct);
  leftIntake.spin(forward, 100, pct);
  rightIntake.spin(forward, 100, pct);

  //turns towards far corner goal
  drive_tl(-110);

  //drives towards the goal
  drive_fwdNoStop(25, 500);
  drive_fwd(60, 3000);

  // stops everything
  leftIntake.stop();
  rightIntake.stop();
  indexerMotor.stop();
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
  while (1) {
    
    //This is for the driving motors
    driveLB.spin(forward,Controller1.Axis3.value() + Controller1.Axis1.value(), percent);
    driveLF.spin(forward,Controller1.Axis3.value() + Controller1.Axis1.value(), percent);
    driveRB.spin(forward,Controller1.Axis3.value() - Controller1.Axis1.value(), percent);
    driveRF.spin(forward,Controller1.Axis3.value() - Controller1.Axis1.value(), percent);

    
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

    //When R1 is pressed ball is ejected
    if(Controller1.ButtonR1.pressing())
    {
      indexerMotor.spin(forward, 12.0, voltageUnits::volt);
      flywheel.spin(reverse,12.0,voltageUnits::volt);
    }
    //When R2 is ball is fired
    else if(Controller1.ButtonR2.pressing())
    {
      indexerMotor.spin(forward, 12.0, voltageUnits::volt);
      flywheel.spin(forward,12.0,voltageUnits::volt);
    }
    else
    {
      indexerMotor.stop(coast);
      flywheel.stop();
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
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
