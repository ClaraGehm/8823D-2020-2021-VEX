// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// driveLB              motor         15              
// locator              distance      4               
// driveRB              motor         1               
// Inertial3            inertial      3               
// driveRF              motor         9               
// driveLF              motor         2               
// rightIntake          motor         19              
// leftIntake           motor         12              
// indexerMotor         motor         11              
// ---- END VEXCODE CONFIGURED DEVICES ----
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
// rightIntake          motor         19              
// leftIntake           motor         12              
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
double totalDist;
double speedCap = 60;
const unsigned short rampUpArray []
{
  20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 
  30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
  40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40
};
int i =0;



void intake_open(double stay)
{
  leftIntake.spin(reverse, 12.0, voltageUnits::volt);
  rightIntake.spin(reverse, 12.0, voltageUnits::volt);
  wait(stay, sec);
  leftIntake.stop(hold);
  rightIntake.stop(hold);
}

void drive_fwd_to_ball()
{
  intake_open(0.3); 
  printf("intakes opened");
  wait(1, sec);

  char buffer[100]; // need a temp space to store data which will be written to the sd card creates an array (list) 100 characters long

  int name = sprintf(buffer, "Distance, Spd Variable, Motor Speed \n");             // insted of printing to a terminal, we are printing a string to a variable
  Brain.SDcard.savefile("distanceWithPDvalues.csv", (uint8_t *)buffer, name);   // savefile needs: name, buffer, length uint8_t  unsigned integer of 8 bits(1 byte)
  kP = .9;  //value will need to be changed to fit our robot    .3
  kD = 0; //value will need to be changed to fit our robot

  totalDist = locator.objectDistance(inches); //sets the total distance that it needs to travel to the current reading of the distance sensor

  while (locator.objectDistance(inches) > 5) //the number 1 will need to be changed so that it looks for the distance where the ball enters the system
  {
    i++;
    //printf("%2f", locator.objectDistance(inches), "\n");
    dist = locator.objectDistance(inches);  //how far the ball is
    currentDist = totalDist - dist;         //determining how far the robot has traveled
    error = currentDist - dist;             //setting error to equal the distance remaining (negative number)
    derivative = error-prevError;           // derivative is equal to the change in error since the last time the lop was played

    speed = -(error*kP) + (-(derivative*kD));  //setting the speed of the robot
    

    if(currentDist < locator.objectDistance(inches)*.2)
    {
      speed = rampUpArray[i];
      Brain.Screen.printAt(50, 150, "In ramp up array");
      printf("in ramp up array\n");
    }

    if (speed > speedCap)                   //puts a cap on the speed
    {
      speed = speedCap;
      Brain.Screen.printAt(50, 100, "speed was calculated at over 100");
    }
    else if (speed < 20 && locator.objectDistance(inches)*.2 > currentDist)
    {
      speed = 20;
    }

    if (locator.objectDistance(inches) < 12) // of the way through the robot with turn on the intakes
    {
      rightIntake.spin(fwd, 12.0, voltageUnits::volt);
      leftIntake.spin(fwd, 12.0, voltageUnits::volt);
      indexerMotor.spin(reverse, 12.0, voltageUnits::volt);
      //printf("intakes run");
    }

    Brain.Screen.clearScreen();
    Brain.Screen.printAt(50, 50, "%d", speed); //printing the speed to the brain screen so we know what is the current speed of the robot
    printf("%d\n", speed);
    //printf("%f\n", locator.objectDistance(inches));

    //prevError = error;                      // setting current error to previous error so we can reset the value of error

    //making all of the motors move at the desired speed
    driveLB.setVelocity(speed, percent);
    driveRB.setVelocity(speed, percent);
    driveRF.setVelocity(speed, percent);
    driveLF.setVelocity(speed, percent);
    driveLB.spin(forward);
    driveRB.spin(forward);
    driveRF.spin(forward);
    driveLF.spin(forward);

    //puts the distance, speed,a nd velocity of one of the motors on the sd card so we can examine the data
    int pos = sprintf(buffer, "%f, %d, %f, \n", locator.objectDistance(inches), speed, driveLB.velocity(percent));  // sprintf takes a string and stores it as an integer
    Brain.SDcard.appendfile("distanceWithPDvalue.csv", (uint8_t *)buffer, pos);
    printf("%d\n", speed);
    printf("%f\n", currentDist);

    //wait 20 msec so that we do not use up all of the cpu on the brain
    wait(20, msec);
  }
  //stops motors at the end
  driveLB.stop();
  driveRB.stop();
  driveRF.stop();
  driveLF.stop();
  indexerMotor.stop();
  rightIntake.stop(coast);
  leftIntake.stop(coast);
  printf("jumped out of loop\n");
}

void distUpdate()
{
  intake_open(.3);
  while(1==1)
  {
    printf("%f\n", locator.objectDistance(inches));
    wait(.5, seconds);
  }
}

int main() 
{
  vexcodeInit();

  /*
  driveLB.spin(reverse);
  driveRB.spin(reverse);
  driveRF.spin(reverse);
  driveLF.spin(reverse);
  wait(1, sec);
  driveLB.stop(coast);
  driveLF.stop(coast);
  driveRB.stop(coast);
  driveRF.stop(coast); */
  drive_fwd_to_ball();
  //distUpdate();
  printf("function ended");
  Brain.Screen.printAt(50, 150, "Hello from the other siiiiideeeeee");
}
