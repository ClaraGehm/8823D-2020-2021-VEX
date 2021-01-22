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


///////////////////////////Proportional Turn variables//////////////////////////////////////////////////////////////////

  double turnkP = .5;     //tuning value for our turnError.
  double turnSpeed = 0; // motor speed, starts at zero.
  double turnError; // the difference between our turnTarget and our current position

//////////////////////////// Slew Rate Acceleration variables /////////////////////////////////////////////////////////////////////////// 
  
  int motorSpeedCap = 5;  //maximum motorSpeed starts at this value, but will grow every few msecs 
  int step = 3;           //by a value of "step", this creates a slew rate of speed growth.
  // the motorSpeed will grow towards 100, and will then be controlled by our PID value,
  // since they will be smaller than 100, the eventual maximum motorSpeed.
 

//////////////////////////Drive PID variables//////////////////////////////////////////////////////////////////////////////////////////////

double kP = 0.08; //tuning value for proportional (the dominiant value)

double kI = 0.00001; //tuning value for integral (adds further speed to overcome resistance, especially when approching the 
// target, because the motorSpeed will be fairly small then)
double kD = 0.02; //tuning value for derivitive (further tuning
//              the speed at which we approach our target by comparing our errors every 20 msecs)

double error;      // the difference between where we are and where we want to be.
double prevError=0; // our previous error. these values update every 20 msecs, so it equals the error 20 msecs ago.
//                 there is no previous error at the start of the program, we set this to 0.            

double derivative; // equals the rate of change (error minus prevError). 
//                 the more our bot accelerates, the bigger the difference btwn the two, and this builds up. 
//                 multiply this by Kd to tune it and subtract it from motorPower (to even out jerks or fast accelerations)   
double totalError = 0;
 //the sum of all errors. if the robot stops or slows down, the totalError skyrockets because the errors will still be fairly large
 // this multiplied by kI added to motorSpeed gives a little "push" to overcome the robots weight, or other resistance.

double integralCap = 90000; //maximum allowed totalError, keeps it from skyrocketing to an unusably high value
                        //once the robot overcomes the resistance, the integral will NOT shrink, so we must limit its size.
double encoderAvg;
  //averaging tracker wheels to use in drive_fwd PID loop.

double motorSpeed; // the sum of the tuned Proportional,Integral,and Derivative.

bool timerReset = true; // boolean acts as an on/off switch for our timer. when this is true, the timer
//will constantly reset, and therefore only begin counting when this is set to false.
float timerCap = 1.7;

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
{                                 // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  InertialSensor.calibrate();
  //wait(1.5, seconds);
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

//////////////// DRIVE FORWARD PID CONTROL LOOP //////////////////////////////////////////////////////////////////////////
  void drive_fwd(int target)
{
  leftE.setPosition(0,degrees); // clear encoders and brain timer
  rightE.setPosition(0,degrees);
  Brain.Timer.reset();

  while(Brain.Timer.value() < timerCap) // loop will run as long as the timer remains under time limit.
  {                                     
    if(timerReset == true)  //constantly reset timer as long as boolean is true (which it is, so the loop will
    {                       //run forever until the boolean is kept false for the set amount of time).
      Brain.Timer.reset();
      Brain.Screen.printAt(10, 160,"timerReset = true");
    }

    if(derivative < 1 && derivative > -1) //if derivative (rate of change) remains at approx. 0, set the boolean to false.
    {                                 // this will allow the timer to count up.   
      timerReset = false;       // derivative can only equal 0 if robot stops moving. (if it gets completely stuck OR reaches target)
      Brain.Screen.printAt(10,180,"timerReset = false");
    }  

    else if(derivative < -1 && derivative > 1)
    {
      timerReset = true; //if derivative ever grows again (if robot moves again), set boolean true. timer resets again.
    }
//                                       Calculating PID values
      encoderAvg = (rightE.position(degrees) - (leftE.position(degrees)))/2; // averaging encoder values

      error = target - encoderAvg;     // setting the error as difference between our target and current position.
      
      derivative = error - prevError;  // set derivative as rate of change in error, if the robot is very fast,
      // the difference between current error and error 20 msecs ago will be large. subtract this from motorSpeed to flatten
      // acceleration curve.
      prevError = error; // update prevError just after derivative is calculated. this ensures that prevError is always 20 mses
                         //behind current error.
      
      totalError = totalError + error; // set totalError to be continual sum of all errors.
      motorSpeed = (error*kP) + (totalError*kI) + (derivative*kD);
      //set motorSpeed to equal the sum of all tuned values (full PID)
      
      motorSpeedCap = motorSpeedCap + step; //growing motorSpeedCap by a rate of step/20 msecs. 
        if(motorSpeedCap > 100)            //motorSpeedCap never can exceed 100.
          {   motorSpeedCap = 100;}

        if(motorSpeed > motorSpeedCap)      //motorSpeed may not exceed the motorSpeedCap
          {   motorSpeed = motorSpeedCap;} // if motorSpeed is tiny, then motorSpeed will grow with the Cap
                                          //  the motorSpeed CAN however, shrink below the Cap as governed by PID.
        if(totalError > integralCap)
          {   totalError = integralCap;} //limit totalError from skyrocketing to a uslessly high value.

      driveLB.spin(forward,motorSpeed,pct);
      driveRB.spin(forward,motorSpeed,pct); // assign motorSpeed to all the drivetrain.
      driveLF.spin(forward,motorSpeed,pct);
      driveRF.spin(forward,motorSpeed,pct);

      Brain.Screen.printAt(10,20,"encoderAvg %lf", encoderAvg);
      Brain.Screen.printAt(10,100,"totalError %lf", totalError); // print all updated values to track code's progress
      Brain.Screen.printAt(10,120,"derivative %lf",derivative);
      Brain.Screen.printAt(10,140,"prevError %lf",prevError);
      Brain.Screen.printAt(10, 160,"error %lf",error);
      wait(20,msec);
    
   }

    driveLB.stop(coast); //loop is finished. timer has exceeded cap. therfore robot is either in steady-state error,
    driveLF.stop(coast); //or has reached target. stop motors.
    driveRB.stop(coast);
    driveRF.stop(coast);

    wait(1000,msec);
    Brain.Screen.printAt(10,40,"final encoder value %f",encoderAvg);
    Brain.Screen.setPenColor(blue);
    Brain.Screen.printAt(10,200,"drive_fwd function Complete");
   
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
//scores the ball in the goal
  flywheel.spin(fwd, -100, pct);
  wait(1, seconds);

  //moves forward
  drive_fwd(30, 100);
  flywheel.stop(coast);
  drive_fwd(30, 1490);

  //moves toward corner goal
  drive_tl(60);
  intake_open();
  drive_fwd(40, 600);

  //detects for the first red ball ot go through the system
  
  //flywheel.spin(reverse,12.0,voltageUnits::volt);
  flywheel.spin(reverse, 12.0, voltageUnits::volt);
  while(!BallDetect.pressing())
  {
    leftIntake.spin(forward,12.0,voltageUnits::volt);
    rightIntake.spin(forward,12.0,voltageUnits::volt);
    indexerMotor.spin(reverse,12.0,voltageUnits::volt);
  }
  leftIntake.spinFor(reverse, 10, degrees);
  rightIntake.spinFor(reverse, 10, degrees);
  while(!BallExit.pressing())
  {
    leftIntake.stop();
    rightIntake.stop();
  }
  leftIntake.spin(fwd);
  rightIntake.spin(fwd); 
  while(!BallDetect.pressing())
  {
    leftIntake.spin(forward,12.0,voltageUnits::volt);
    rightIntake.spin(forward,12.0,voltageUnits::volt);
    indexerMotor.spin(reverse,12.0,voltageUnits::volt);
  }
  leftIntake.stop();
  rightIntake.stop();
  flywheel.spin(reverse,12.0,voltageUnits::volt);
  /*while(!eyes.takeSnapshot(eyes__REDB))
  {
    wait(.4, seconds);
    indexerMotor.stop(coast);
  }*/
   
  /*while(!eyes.takeSnapshot(eyes__REDB))
  {
    wait(.8, seconds);
    indexerMotor.stop(coast);
  }*/

  while(!BallExit.pressing())
  {
    
  }
  wait(.75,sec);
  flywheel.stop();
  leftIntake.spin(reverse, 70, percent);
  rightIntake.spin(reverse, 70, percent);
  wait(.5, seconds);
  leftIntake.stop();
  rightIntake.stop();
  /*
  int count = 0;
  while(count == 0)
  {
    intake_close(.1);
    if(BallDetect.value() == 1)
    {
      count = 1;
    }
  }
  indexerMotor.stop();
  leftIntake.spin(reverse, 100, percent);
  rightIntake.spin(reverse, 100, percent);
  wait(.8, seconds);
  leftIntake.stop(hold);
  rightIntake.stop(hold);

  indexerMotor.spin(forward);
  wait(.06, seconds);
  indexerMotor.stop();
*/
  //backs up and turns towards 
  drive_bwd(60, 999);
  indexerMotor.stop(coast);
  flywheel.stop(coast);
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
