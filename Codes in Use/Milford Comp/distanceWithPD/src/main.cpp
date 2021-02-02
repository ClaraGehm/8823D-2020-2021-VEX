/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\leeperp                                          */
/*    Created:      Tue Jan 12 2021                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// driveLB              motor         15              
// locator              distance      4               
// driveRB              motor         1               
// Inertial3            inertial      3               
// driveRF              motor         9               
// driveLF              motor         2               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

//initalizing the variables 
double kP;
double kD, derivative;
double target;
double error;
double prevError; 
int speed;
double currentDist, dist;

void drive_fwd_to_ball()
{
  char buffer[100]; // need a temp space to store data which will be written to the sd card creates an array (list) 100 characters long

  int name = sprintf(buffer, "Distance, Spd Variable, Motor Speed \n");             // insted of printing to a terminal, we are printing a string to a variable
  Brain.SDcard.savefile("Distance Sensor Pcontrol.csv", (uint8_t *)buffer, name);   // savefile needs: name, buffer, length uint8_t  unsigned integer of 8 bits(1 byte)
  kP = 1;  //value will need to be changed to fit our robot
  kD = .5; //value will need to be changed to fit our robot

  while (locator.objectDistance(inches) > 1) //the number 1 will need to be changed so that it looks for the distance where the ball enters the system
  {
    prevError = error;                      // setting current error to previous error so we can reset the value of error
    dist = locator.objectDistance(inches);  //hw far the ball is
    error = currentDist - dist;             //setting error to equal the distance remaining (negative number)
    derivative = error-prevError;           // derivative is equal to the change in error since the last time the lop was played

    speed = error*kP + derivative*kD;  //setting the speed of the robot

    if (speed > 100) //puts a cap on the speed
    {
      speed = 100;
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

int main() 
{
  vexcodeInit();

  drive_fwd_to_ball();
}
