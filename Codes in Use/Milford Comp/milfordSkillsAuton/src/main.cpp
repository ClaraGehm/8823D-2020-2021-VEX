// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// InertialSensor       inertial      3               
// leftE                encoder       E, F            
// rightE               encoder       A, B            
// eyes                 vision        16              
// BallDetect           limit         C               
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
// BumperG              limit         G               
// BumperH              limit         H               
// locator              distance      4               
// ---- END VEXCODE CONFIGURED DEVICES ----
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
// BallDetect           limit         C               
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
// BumperG              limit         G               
// BumperH              limit         H               
// ---- END VEXCODE CONFIGURED DEVICES ----


#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

//-----Array Variables
    int Axis3Cap = 0;
    int NegAxis3Cap = 0;
    int step = 1;
    double kP;
//------Proportional Values
double kD, derivative;
double target;
double error;
double prevError; 
int speed;
double currentDist, dist;
double totalDist;

// define your global instances of motors and other devices here
double turnkP = .5;     //tuning value for our turnError.
double turnSpeed = 0; // motor speed, starts at zero.
double turnError; // the difference between our turnTarget and our current position
double currentRotationsL = 0;
double currentRotationsR = 0;
double prevRotationsL = 0;
double prevRotationsR = 0;
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

//forward speed, distance

//DRIVE FORWARD TO BALL FUNCTION
void drive_fwd_to_ball()
{
  char buffer[100]; // need a temp space to store data which will be written to the sd card creates an array (list) 100 characters long

  int name = sprintf(buffer, "Distance, Spd Variable, Motor Speed \n");             // insted of printing to a terminal, we are printing a string to a variable
  Brain.SDcard.savefile("Distance Sensor Pcontrol.csv", (uint8_t *)buffer, name);   // savefile needs: name, buffer, length uint8_t  unsigned integer of 8 bits(1 byte)
  kP = .9;  //value will need to be changed to fit our robot
  //kD = .5; //value will need to be changed to fit our robot

  totalDist = locator.objectDistance(inches); //sets the total distance that it needs to travel to the current reading of the distance sensor

  while (locator.objectDistance(inches) > 1) //the number 1 will need to be changed so that it looks for the distance where the ball enters the system
  {
    prevError = error;                      // setting current error to previous error so we can reset the value of error
    dist = locator.objectDistance(inches);  //how far the ball is
    currentDist = totalDist - dist;         //determining how far the robot has traveled
    error = currentDist - dist;             //setting error to equal the distance remaining (negative number)
    derivative = error-prevError;           // derivative is equal to the change in error since the last time the lop was played

    speed = error*kP; // + derivative*kD;  //setting the speed of the robot

    if (speed > 100)                   //puts a cap on the speed
    {
      speed = 100;
      Brain.Screen.printAt(50, 100, "speed was calculated at over 100");
    }

    if (currentDist >= (totalDist*.9)) //90% of the way through the robot with turn on the intakes
    {
      rightIntake.spin(fwd, 12.0, voltageUnits::volt);
      leftIntake.spin(fwd, 12.0, voltageUnits::volt);
    }

    Brain.Screen.clearScreen();
    Brain.Screen.printAt(50, 50, "%d", speed); //printing the speed to the brain screen so we know what is the current speed of the robot

    //making all of the motors move at the desired speed
    driveLB.setVelocity(speed, rpm);
    driveRB.setVelocity(speed, rpm);
    driveRF.setVelocity(speed, rpm);
    driveLF.setVelocity(speed, rpm);
    driveLB.spin(forward);
    driveRB.spin(forward);
    driveRF.spin(forward);
    driveLF.spin(forward);

    //puts the distance, speed,a nd velocity of one of the motors on the sd card so we can examine the data
    int pos = sprintf(buffer, "%f, %d, %f, \n", locator.objectDistance(inches), speed, driveLB.velocity(rpm));  // sprintf takes a string and stores it as an integer
    Brain.SDcard.appendfile("Distance Sensor Pcontrol.csv", (uint8_t *)buffer, pos);

    //wait 20 msec so that we do not use up all of the cpu on the brain
    wait(20, msec);
  }
  //stops motors at the end
  driveLB.stop();
  driveRB.stop();
  driveRF.stop();
  driveLF.stop();
}

//DRIVE FORWARD NORMAL FUNCTION
void drive_fwd(int speed, int dist)
{
  leftE.setPosition(0, deg);
  rightE.setPosition(0, deg);
  Brain.Screen.setPenColor(white);
  while (Brain.Timer.value() < 0.3)
  {
    //reseting the values so that they are updates
    prevRotationsL = currentRotationsL;
    currentRotationsL = leftE.value();
    prevRotationsR = currentRotationsR;
    currentRotationsR = rightE.value();
    
    //telling the motors to move
    driveLB.spin(fwd, speed, pct);
    driveRB.spin(fwd, speed, pct);
    driveLF.spin(fwd, speed, pct);
    driveRF.spin(fwd, speed, pct);

    //checks to see if the robot is above the number of ticks it should go
    if(-leftE.value() <= dist && rightE.value() <= dist)
    {
      driveLB.stop(brake);
      driveRB.stop(brake);
      driveLF.stop(brake);
      driveRF.stop(brake);
    }
    //if the robot is stuck it will stop it
    else if(prevRotationsL == currentRotationsL || prevRotationsR == currentRotationsR)
    {

    }
    //if neither of those things are true it resets the Brain Timer 
    else
    {
      Brain.Timer.reset();
    }
    
    wait(20, msec);
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(50, 50, "heading %5.2d", rightE.value());
    Brain.Screen.printAt(50, 100, "heading %5.2d", leftE.value());
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
  Brain.Screen.setPenColor(white);
  while (leftE.value() <= dist && -rightE.value() <= dist)
  {
    if(-leftE.value() == rightE.value())
    {
      driveLB.spin(reverse, speed, pct);
      driveRB.spin(reverse, speed, pct);
      driveLF.spin(reverse, speed, pct);
      driveRF.spin(reverse, speed, pct);
    }
    
    wait(20, msec);
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(50, 50, "rotation %5.2d", rightE.value());
    Brain.Screen.printAt(50, 100, "rotation %5.2d", leftE.value());
  }
  driveLB.stop(brake);
  driveRB.stop(brake);
  driveLF.stop(brake);
  driveRF.stop(brake);
  
}
///////////////// TURN RIGHT PROPORTIONAL FUNCTION //////////////////////////////////////////////////////////////////////

void drive_tr(int target) //setting target as a parameter to be
// defined every time function is called

{
  InertialSensor.setRotation(0,degrees); 
  wait(100,msec); //inertialSensor resets with every call of the functon

  while(InertialSensor.rotation(degrees) < target) // loop runs until robot has reached target.
  { Brain.Screen.printAt(50,50,"%f",InertialSensor.rotation(degrees));
    
    //setting up error as required travel distance.  
    turnError = (target-InertialSensor.rotation(degrees));
    
    Brain.Screen.printAt( 70,70,"%f",turnError);
    turnSpeed = (turnError*turnkP); //tune error with kP and assign value to turnSpeed.
    
    Brain.Screen.printAt(90,90,"%f",turnSpeed);

      //maximum motorspeed: 40%
    if(turnSpeed < 3)
    {turnSpeed = 3;}
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
  turnError = (target-InertialSensor.rotation(degrees));
    
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
  InertialSensor.setRotation(0,degrees); 
  wait(100,msec); //inertialSensor resets with every call of the functon

  while(InertialSensor.rotation(degrees) > target) // loop runs until robot has reached target.
  { Brain.Screen.printAt(50,50,"Inertial: %f",InertialSensor.rotation(degrees));
    
    //setting up error as required travel distance.  
    turnError = (target-InertialSensor.rotation(degrees));
    
    Brain.Screen.printAt(50,70,"%f",turnError);
    turnSpeed = (turnError*turnkP); //tune error with kP and assign value to turnSpeed.
    
    Brain.Screen.printAt(50,90,"%f",turnSpeed);

    if(turnSpeed > -3)
    {turnSpeed = -3;}
    
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
  turnError = (target-InertialSensor.rotation(degrees));
    
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

void intake_open()
{
  while(!BumperG.pressing() && !BumperH.pressing())
  {
  leftIntake.setVelocity(-95,pct);
  rightIntake.setVelocity(-100,pct);
  leftIntake.spin(fwd);
  rightIntake.spin(fwd);
  Brain.Screen.print(BumperG.value());
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
          wait(10,msec);
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
  InertialSensor.setHeading(0, deg);
  flywheel.setVelocity(100,percent);
  flywheel.spinFor(forward,-380,degrees);
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
  indexerMotor.spin(reverse, 12.0, voltageUnits::volt);
  //drive_fwd(40,100);


  //waits a second so the ball can go in
  wait(.35, sec);
  indexerMotor.stop(coast); 

  //moves toward corner goal
  InertialSensor.setHeading(0, degrees);
  Brain.Screen.printAt(100, 100, "Sensor Value: %f", InertialSensor.value());
  drive_fwd(40,600); //value between intaking first ball and turning before goal
  drive_tl(-65);
  drive_fwd(40,630);
  flywheel.spin(reverse, 12.0, voltageUnits::volt);
  indexerMotor.spin(reverse, 12.0, voltageUnits::volt);
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
  drive_bwd(40,1800);
  intake_open();
  drive_tr(89);
  intake_open();
  drive_fwd(40, 1450);
  
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
    flywheel.spin(reverse,12.0,voltageUnits::volt);
    indexerMotor.spin(reverse,12.0,voltageUnits::volt);
  }
  //wait sometime to make sure the ball leaves the system then stops the motors 
  wait(800,msec);
  flywheel.stop();
  indexerMotor.stop();

  //////////////////////////////////////////////////////////////////////
  //      FOURTH GOAL
  //////////////////////////////////////////////////////////////////////

  //backs away from the goal and starts heading towards the ball for the first goal
  drive_bwd(30,715);
  wait(500, msec);
  drive_tr(89);
  intake_open();
  drive_fwd(40,1600);

  //intakes ball for fourth goal
  while(BallDetect.value() == 0)
  {
    rightIntake.spin(fwd, 12.0, voltageUnits::volt);
    leftIntake.spin(fwd, 12.0, voltageUnits::volt);
    indexerMotor.spin(reverse, 12.0, voltageUnits::volt);
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
    flywheel.spin(reverse,12.0,voltageUnits::volt);
    indexerMotor.spin(reverse,12.0,voltageUnits::volt);
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
    indexerMotor.spin(reverse, 12.0, voltageUnits::volt);
  }
  wait(200, msec);
  rightIntake.stop(coast);
  leftIntake.stop(coast);
  indexerMotor.stop(coast);

  //turns and moves towards goal
  drive_fwd(30, 220);
  drive_tl(-131);
  drive_fwd(30, 1180);
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
    flywheel.spin(reverse,12.0,voltageUnits::volt);
    indexerMotor.spin(reverse,12.0,voltageUnits::volt);
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
    indexerMotor.spin(reverse, 12.0, voltageUnits::volt);
  }
  wait(200, msec);
  rightIntake.stop(coast);
  leftIntake.stop(coast);
  indexerMotor.stop(coast);

  //drives towards goal
  drive_fwd(30, 1000);
  drive_tl(-25);
  drive_fwd(20, 150);

  //luanch ball into goal
  while(!BallExit.pressing())
  {
    flywheel.spin(reverse,12.0,voltageUnits::volt);
    indexerMotor.spin(reverse,12.0,voltageUnits::volt);
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
  // User control code here, inside the loop
  while (1) 
  { //This is for the driving motors
   // Get the velocity percentage of the left motor. (Axis3 + Axis1)
   int turnValue = Controller1.Axis1.value();  
    if  (turnValue < 5 && turnValue > -5)
    {
      turnValue = 0;
    }
	// Drive Control
		// Control layout is R/C with dedicated joysticks for moving forward/backward and rotating left/right
		// The sum of the inputs is the mapped to an array to control motor power
		
   
    int forwardDriveValue = Controller1.Axis3.position(); 

    if(forwardDriveValue > 2 || forwardDriveValue < -2)
    {   
    Axis3Cap = Axis3Cap + step;
    NegAxis3Cap = NegAxis3Cap - step;
    }

    if(forwardDriveValue >= Axis3Cap)
    {
      forwardDriveValue = Axis3Cap;
      NegAxis3Cap = 0;
    }
    
    if(forwardDriveValue <= NegAxis3Cap)
    {
      forwardDriveValue = NegAxis3Cap;
      Axis3Cap = 0;
    }

    Brain.Screen.printAt(10,20,"%d",forwardDriveValue);
    Brain.Screen.printAt(10,40,"%d",Axis3Cap);
    Brain.Screen.printAt(10,60,"%d",NegAxis3Cap);
    
   int leftMotorSpeed = forwardDriveValue + turnValue;
    // Get the velocity percentage of the right motor. (Axis3 - Axis1)
   int rightMotorSpeed = forwardDriveValue - turnValue;

    driveLB.spin(forward,leftMotorSpeed,pct); //(Axis3+Axis4)/2;
    driveLF.spin(forward,leftMotorSpeed,percent);
    driveRB.spin(forward,rightMotorSpeed,percent);
    driveRF.spin(forward,rightMotorSpeed,percent);

    
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
   vex::task::sleep(25);
}
