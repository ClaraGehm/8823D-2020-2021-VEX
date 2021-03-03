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

    //if(turnSpeed > -3)
    //{turnSpeed = -3;}
    
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
  driveLB.stop(brake);
  driveRB.stop(brake);
  driveLF.stop(brake);
  driveRF.stop(brake);
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
  /////////////////////////////////////////////
  //     FIRST GOAL
  /////////////////////////////////////////////
  
  //goal 1 (hodd lifting up and scoring)
  leftInertial.setHeading(0, deg);
  rightInertial.setHeading(0, deg);
  flywheel.setVelocity(100,percent);
  flywheel.spinFor(forward,380,degrees);
  flywheel.stop();

  //////////////////////////////////////////////
  //    SECOND GOAL
  //////////////////////////////////////////////

  intake_open();

  //moves forward away from the goal
  drive_fwd(40,850);
  flywheel.stop(coast);
  //opens the intake to intake first ball 
  leftIntake.spin(forward, 12.0, voltageUnits::volt);
  rightIntake.spin(forward, 12.0, voltageUnits::volt);
  indexerMotor.spin(forward, 12.0, voltageUnits::volt);
  //drive_fwd(40,100);


  //waits a second so the ball can go in
  wait(.35, sec);
  indexerMotor.stop(coast); 

  //moves toward corner goal

  drive_fwd(40,600); //value between intaking first ball and turning before goal
  drive_tl(-65);
  drive_fwd(40,735);
  flywheel.spin(forward, 12.0, voltageUnits::volt);
  indexerMotor.spin(forward, 12.0, voltageUnits::volt);
  leftIntake.stop(); 
  rightIntake.stop();
  //waits until the ball exits the system and scores into the corner goal (2nd goal)
  while (!BallExit.pressing()) 
  {

  }
  //waits a second so we know it has deffinitly left the system 
  wait(.2, seconds); 
  flywheel.stop();

  /////////////////////////////////////////////////
  //    THIRD GOAL
  /////////////////////////////////////////////////
 
  //backs away from the goal, opens intakes, turns, and heads towards the ball in front of the third goal
  drive_bwd(40,1700);
  intake_open();
  drive_tr(89);
  intake_open();
  drive_fwd(40, 1350);
  
  //starts intaking the ball and keeps going until it is in the system (triggered by ballDetect), then turns everything off
  leftIntake.spin(fwd,12.0,voltageUnits::volt);
  rightIntake.spin(fwd,12.0,voltageUnits::volt);
  while(!BallDetect.pressing())
  {

  }
  wait(300, msec);
  indexerMotor.stop();
  leftIntake.stop();
  rightIntake.stop();

  //turns towards the third goal and approaches it.
  drive_tl(-45);
  intake_open();
  drive_fwd(25,300);
  driveLB.spin(forward, 6.0, voltageUnits::volt);
  driveLF.spin(forward, 6.0, voltageUnits::volt);
  driveRB.spin(forward, 6.0, voltageUnits::volt);
  driveRF.spin(forward, 6.0, voltageUnits::volt);
  wait(150, msec);
  driveLB.stop();
  driveLF.stop();
  driveRB.stop();
  driveRF.stop();

  //runs the indexer and flywheel until the ball leaves the system 
  while(!BallExit.pressing())
  {
    flywheel.spin(forward,12.0,voltageUnits::volt);
    indexerMotor.spin(forward,12.0,voltageUnits::volt);
  }
  //wait sometime to make sure the ball leaves the system then stops the motors 
  wait(800,msec);
  flywheel.stop();
  indexerMotor.stop();

  //////////////////////////////////////////////////////////////////////
  //      FOURTH GOAL
  //////////////////////////////////////////////////////////////////////

  //backs away from the goal and starts heading towards the ball for the first goal
  drive_bwd(30,610);
  wait(500, msec);
  drive_tr(89);
  intake_open();
  drive_fwd(40,1600);

  //intakes ball for fourth goal
  while(BallDetect.value() == 0)
  {
    rightIntake.spin(fwd, 12.0, voltageUnits::volt);
    leftIntake.spin(fwd, 12.0, voltageUnits::volt);
    indexerMotor.spin(forward, 12.0, voltageUnits::volt);
  }
  wait(200, msec);
  rightIntake.stop(coast);
  leftIntake.stop(coast);
  indexerMotor.stop(coast);


  //turns and heads towards the fourth goal
  drive_tl(-50);
  //intake_open();
  drive_fwd(40,1000); 
  driveLB.spin(fwd, 30, percent);
  driveLF.spin(forward, 30, percent);
  driveRB.spin(forward, 30, percent);
  driveRF.spin(forward, 30, percent);
  wait(200, msec);
  driveLB.stop();
  driveLF.stop();
  driveRB.stop();
  driveRF.stop();

  //luanch ball into goal
  while(!BallExit.pressing())
  {
    flywheel.spin(forward,12.0,voltageUnits::volt);
    indexerMotor.spin(forward,12.0,voltageUnits::volt);
  }
  wait(500, msec);
  flywheel.stop(coast); 
  indexerMotor.stop(coast);

  ///////////////////////////////////////////////
  //     FITH GOAL
  //////////////////////////////////////////////

  //backs away from goal
  drive_bwd(30, 590);
  drive_tr(136);
  //drive_bwd(30, 810);
  driveLB.spin(reverse, 6.0, voltageUnits::volt);
  driveLF.spin(reverse, 6.0, voltageUnits::volt);
  driveRB.spin(reverse, 6.0, voltageUnits::volt);
  driveRF.spin(reverse, 6.0, voltageUnits::volt);
  wait(1000, msec);
  driveLB.stop(coast);
  driveLF.stop(coast);
  driveRB.stop(coast);
  driveRF.stop(coast); 
  wait(200, msec);

  //drives forward to the next ball
  drive_fwd(30, 1600);
  intake_open();//remove when full code is running
  drive_tr(43);
  drive_fwd(30, 1100);
  

  //pick up ball
  while(BallDetect.value() == 0)
  {
    rightIntake.spin(fwd, 12.0, voltageUnits::volt);
    leftIntake.spin(fwd, 12.0, voltageUnits::volt);
    indexerMotor.spin(forward, 12.0, voltageUnits::volt);
  }
  wait(200, msec);
  rightIntake.stop(coast);
  leftIntake.stop(coast);
  indexerMotor.stop(coast);

  //turns and moves towards goal
  drive_fwd(30, 220);
  drive_tl(-131);
  drive_fwd(30, 1280);
  driveLB.spin(fwd, 30, percent);
  driveLF.spin(forward, 30, percent);
  driveRB.spin(forward, 30, percent);
  driveRF.spin(forward, 30, percent);
  wait(200, msec);
  driveLB.stop();
  driveLF.stop();
  driveRB.stop();
  driveRF.stop();

  //luanch ball into goal
  while(!BallExit.pressing())
  {
    flywheel.spin(forward,12.0,voltageUnits::volt);
    indexerMotor.spin(forward,12.0,voltageUnits::volt);
  }
  wait(500, msec);
  flywheel.stop(coast); 
  indexerMotor.stop(coast);

  //////////////////////////////////////
  //  Sixth Goal
  //////////////////////////////////////

  //backup and move towards the ball
  drive_bwd(30, 600);
  drive_tr(70);
  intake_open();
  drive_fwd(30, 1200);

  //pick up ball
  while(BallDetect.value() == 0)
  {
    rightIntake.spin(fwd, 12.0, voltageUnits::volt);
    leftIntake.spin(fwd, 12.0, voltageUnits::volt);
    indexerMotor.spin(forward, 12.0, voltageUnits::volt);
  }
  wait(200, msec);
  rightIntake.stop(coast);
  leftIntake.stop(coast);
  indexerMotor.stop(coast);

  //drives towards goal
  drive_fwd(30, 1000);
  drive_tl(-15);
  drive_fwd(20, 150);

  driveLB.spin(fwd, 30, percent);
  driveLF.spin(forward, 30, percent);
  driveRB.spin(forward, 30, percent);
  driveRF.spin(forward, 30, percent);
  wait(200, msec);
  driveLB.stop();
  driveLF.stop();
  driveRB.stop();
  driveRF.stop();

  //luanch ball into goal
  while(!BallExit.pressing())
  {
    flywheel.spin(forward,12.0,voltageUnits::volt);
    indexerMotor.spin(forward,12.0,voltageUnits::volt);
  }
  wait(500, msec);
  flywheel.stop(coast); 
  indexerMotor.stop(coast);
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
