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

    int Axis3Cap = 0;
    int NegAxis3Cap = 0;
    int step = 1;

// define your global instances of motors and other devices here
double turnkP = .5;     //tuning value for our turnError.
double turnSpeed = 0; // motor speed, starts at zero.
double turnError; // the difference between our turnTarget and our current position
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
  
  if(DistanceSensor.objectDistance(inches) < .5)
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
      else if(DistanceSensor.objectDistance(inches) >= .5)
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
  // scores the ball in the goal
  flywheel.spin(fwd, -100, pct);
  wait(400, msec);

  // moves forward
  drive_fwd(30, 100);
  flywheel.stop(coast);
  drive_fwd(30, 1300);

  // moves toward corner goal
  drive_tr(65);
  intake_open();
  drive_fwd(40, 680);

  //drives forward a little more so that we know we are up against the goal
  driveLB.spin(forward, 6.0, voltageUnits::volt);
  driveLF.spin(forward, 6.0, voltageUnits::volt);
  driveRB.spin(forward, 6.0, voltageUnits::volt);
  driveRF.spin(forward, 6.0, voltageUnits::volt);
  wait(300, msec);
  driveLB.stop(coast);
  driveLF.stop(coast);
  driveRB.stop(coast);
  driveRF.stop(coast);

  // detects for the first red ball it go through the system
  while (BallExit.value() == 0) {
    //rightIntake.spin(forward, 12.0, voltageUnits::volt);
    //leftIntake.spin(forward, 12.0, voltageUnits::volt);
    indexerMotor.spin(reverse, 12.0, voltageUnits::volt);
    flywheel.spin(reverse, 12.0, voltageUnits::volt);
  }

  //waits for the sensor to return to origonal position
  while(BallExit.value() == 1)
  {
    rightIntake.spin(forward, 12.0, voltageUnits::volt);
    leftIntake.spin(forward, 12.0, voltageUnits::volt);
  }

  //once the sensor = 1 (ball triggering it) it exits the loop
  while(BallExit.value() == 0)
  {
  
  }

    //waits for the sensor to return to origonal position
  while(BallExit.value() == 1)
  {
    rightIntake.spin(forward, 12.0, voltageUnits::volt);
    leftIntake.spin(forward, 12.0, voltageUnits::volt);
  }
  

  //once the sensor = 1 (ball triggering it) it exits the loop
  while(BallExit.value() == 0)
  {
  
  }
  leftIntake.stop();
  rightIntake.stop();
  //waits for the ball to leave the flywheel and turns it off
  wait(300, msec);
  flywheel.stop(hold);
  indexerMotor.stop(coast);

  wait(300,msec);
  
  //backs away so that all balls count for us
  drive_bwd(30, 700);
  //stops everything
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
