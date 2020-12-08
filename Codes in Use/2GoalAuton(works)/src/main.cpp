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
// driveLB              motor         15
// leftIntake           motor         12
// rightIntake          motor         19
// indexerMotor         motor         11
// flywheel             motor         20
// Controller1          controller
// driveRB              motor         1
// driveRF              motor         9
// driveLF              motor         2
// eyes                 vision        16
// BumperG              bumper        G
// BumperH              bumper        H
// rightE               encoder       A, B
// leftE                encoder       E, F
// InertialSensor       inertial      3
// BallDetect           limit         C
// BallExit             limit         D
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

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  InertialSensor.calibrate();
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
int kill = 1;
int x1;
int y4;
int r = 1;

void drive_fwd(int speed, int dist) {
  driveLB.setVelocity(speed, pct);
  driveRB.setVelocity(speed, pct);
  driveLF.setVelocity(speed, pct);
  driveRF.setVelocity(speed, pct);
  driveLB.spinFor(fwd, dist, deg, false);
  driveRB.spinFor(fwd, dist, deg, false);
  driveLF.spinFor(fwd, dist, deg, false);
  driveRF.spinFor(fwd, dist, deg);
  driveLB.stop(brake);
  driveRB.stop(brake);
  driveLF.stop(brake);
  driveRF.stop(brake);
}

void drive_bwd(int speed, int dist) {
  driveLB.setVelocity(-speed, pct);
  driveRB.setVelocity(-speed, pct);
  driveLF.setVelocity(-speed, pct);
  driveRF.setVelocity(-speed, pct);
  driveLB.spinFor(fwd, -dist, deg, false);
  driveRB.spinFor(fwd, -dist, deg, false);
  driveLF.spinFor(fwd, -dist, deg, false);
  driveRF.spinFor(fwd, -dist, deg);
  driveLB.stop(brake);
  driveRB.stop(brake);
  driveLF.stop(brake);
  driveRF.stop(brake);
}

void drive_tr(/*int speed,*/ int target) {
  InertialSensor.setRotation(0, degrees);
  wait(100, msec);
  int speed = 50; // (target-InertialSensor.rotation(degrees))/3;
  while (InertialSensor.rotation(degrees) < target) {
    driveLB.spin(forward, speed, pct);
    driveLF.spin(forward, speed, pct);
    driveRF.spin(reverse, speed, pct);
    driveRB.spin(reverse, speed, pct);
  }
  driveLB.stop(brake);
  driveLF.stop(brake);
  driveRB.stop(brake);
  driveRF.stop(brake);
}

void drive_tl(int target) {
  InertialSensor.setHeading(0, degrees);
  wait(100, msec);
  int speed = (target - InertialSensor.rotation(degrees)) / 3;
  while (InertialSensor.rotation(degrees) > -target) {
    driveLB.spin(reverse, speed, pct);
    driveLF.spin(reverse, speed, pct);
    driveRF.spin(forward, speed, pct);
    driveRB.spin(forward, speed, pct);
    // printf(degrees);
  }
  driveLB.stop(brake);
  driveLF.stop(brake);
  driveRB.stop(brake);
  driveRF.stop(brake);
}

void intake_open() {
  while (!BumperG.pressing() && !BumperH.pressing()) {
    leftIntake.setVelocity(-95, pct);
    rightIntake.setVelocity(-100, pct);
    leftIntake.spin(fwd);
    rightIntake.spin(fwd);
    Brain.Screen.print(BumperG.value());
  }
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

void ballKickout() {
  if (kill == 1) {
    if (eyes.takeSnapshot(eyes__BLUEB)) {
      flywheel.spin(forward, 12.0, voltageUnits::volt);
      Brain.Screen.printAt(150, 105, "i did it");
      wait(0.1, seconds);
    } else if (eyes.takeSnapshot(eyes__REDB)) {
      flywheel.spin(reverse, 12.0, voltageUnits::volt);
      Brain.Screen.printAt(150, 105, "i did it");
      while (!BallExit.pressing()) { /*do nothing until limitswitch is pressed*/
        wait(500, msec);
        flywheel.stop();
      }

    } else {
      flywheel.stop(coast);
      wait(0.1, seconds);
    }
  }
}

void changeX() {
  y4 = -1;
  x1 = x1 * -1;
}

void changeY() {
  x1 = -1;
  y4 = y4 * -1;
}

void autonomous(void) {
  // Initializing Robot Configuration. DO NOT
  // REMOVE!Controller1.Axis3.position(percent) +
  // Controller1.Axis1.position(percent)
  vexcodeInit();
  InertialSensor.calibrate();
  wait(1.5, seconds);

  // scores the ball in the goal
  flywheel.spin(fwd, -100, pct);
  wait(1, seconds);

  // moves forward
  drive_fwd(30, 100);
  flywheel.stop(coast);
  drive_fwd(30, 2400);

  // moves toward corner goal
  drive_tl(62);
  intake_open();
  drive_fwd(40, 1100);

  // detects for the first red ball ot go through the system
  while (BallDetect.value() == 0) {
    leftIntake.spin(forward, 12.0, voltageUnits::volt);
    rightIntake.spin(forward, 12.0, voltageUnits::volt);
  }

  while (!eyes.takeSnapshot(eyes__REDB)) {
    wait(.8, seconds);
    flywheel.stop(coast);
    indexerMotor.stop(coast);
  }
  // makes sure that the second red ball goes through the system
  while (BallDetect.value() == 0) {
    leftIntake.spin(forward, 12.0, voltageUnits::volt);
    rightIntake.spin(forward, 12.0, voltageUnits::volt);
  }

  while (!eyes.takeSnapshot(eyes__REDB)) {
    wait(.8, seconds);
    flywheel.stop(coast);
    indexerMotor.stop(coast);
  }

  leftIntake.spin(reverse, 100, percent);
  rightIntake.spin(reverse, 100, percent);
  wait(.5, seconds);
  leftIntake.stop();
  rightIntake.stop();
  int count = 0;
  while (count < 1) {
    intake_close(.1);
    if (BallDetect.value() == 1) {
      count += 1;
    }
  }
  leftIntake.spin(reverse, 100, percent);
  rightIntake.spin(reverse, 100, percent);
  wait(.5, seconds);
  leftIntake.stop();
  rightIntake.stop();
  indexerMotor.spin(forward);
  wait(.3, seconds);
  indexerMotor.stop();

  // backs up and turns towards
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

void usercontrol(void) {
  // User control code here, inside the loop
  x1 = -1;
  y4 = -1;

  // sets up the intakes so that their starting position is at 0 degrees
  rightIntake.setPosition(0, deg);
  leftIntake.setPosition(0, deg);
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // This is for the driving motors
    driveLB.spin(reverse, Controller1.Axis3.value(), percent);
    driveLF.spin(reverse, Controller1.Axis3.value(), percent);
    driveRB.spin(reverse, Controller1.Axis2.value(), percent);
    driveRF.spin(reverse, Controller1.Axis2.value(), percent);

    // kicks out red balls
    /*if (Controller1.ButtonRight.pressing())
    {
      ballKickout();
    }
    else
    {

    }*/

    // When L1 is pressed the intake moves forward
    if (Controller1.ButtonL2.pressing() && r == 1) // movs out 20 degrees
    {
      rightIntake.spin(forward, 12.0, voltageUnits::volt);
      leftIntake.spin(forward, 12.0, voltageUnits::volt);
      rightIntake.setStopping(coast);
      leftIntake.setStopping(coast);
      if (BumperG.pressing() && BumperH.pressing()) {
        r = -1;
      }
    } else if (Controller1.ButtonL1.pressing()) {
      r = 1;
      rightIntake.spin(reverse, 12.0, voltageUnits::volt);
      leftIntake.spin(reverse, 12.0, voltageUnits::volt);
      rightIntake.setStopping(hold);
      leftIntake.setStopping(hold);
    } else {
      rightIntake.stop();
      leftIntake.stop();
    }

    // if (Controller1.ButtonUp.pressing())
    //{
    // kill = kill*-1;
    //}

    // When R1 is pressed indexerMotor moves forward
    if (Controller1.ButtonR1.pressing()) {
      indexerMotor.spin(forward, 12.0, voltageUnits::volt);
    }
    // When R2 is pressed indexerMotor moves backwards
    else if (Controller1.ButtonR2.pressing()) {
      indexerMotor.spin(reverse, 12.0, voltageUnits::volt);
    } else {
      indexerMotor.stop(coast);
    }

    // When A is pressed the flywheel moves forward
    // Directional
    // logic------------------------------------------------------------------------------------------------------------------------------------------
    if (x1 == -1 && y4 == -1) {
      flywheel.stop(coast);
    } else if (x1 == 1) {
      flywheel.spin(forward, 12.0, voltageUnits::volt);
    } else if (x1 == -1 && y4 == 1) {
      flywheel.spin(reverse, 12.0, voltageUnits::volt);
    } else {
    }

    // Button
    // input-----------------------------------------------------------------------------------------------------------------------------------------------
    if (Controller1.ButtonA.pressing()) // when button A is pressed the first
                                        // time, the flywheel
    {
      changeX();
      wait(250, msec);
    } else if (Controller1.ButtonB.pressing()) {
      changeY();
      wait(250, msec);
    } else {
    }
    // Has all motors stop
  }
  wait(20, msec); // Sleep the task for a short amount of time to
                  // prevent wasted resources.
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
