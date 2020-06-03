/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Sam B. and Clara G.                                       */
/*    Created:      Sun Jan 26 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:s
// [Name]               [Type]        [Port(s)]
// leftDrive            motor         1               
// Controller1          controller                    
// rightDrive           motor         10              
// claw                 motor         3               
// arm                  motor         8               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

double dabs (double num) //This function serves as a quick way to turn a number positive. First time it is used is on line 97. 
{
  if (num < 0)//If the number is negative it will run the if statment 
  {
    num *= -1;//Multiplies the number by -1 to turn it positive 
  }
  return num;//Returns the number 
}

/*double rampUp = 0;
double accelerate (double error, double rampBy)
{
  if(rampUp < 1)
  {
    rampUp += rampBy;
  }
  error *= rampUp;
  return error;
}*/
//The function abouve was an attempt at making the speeding up more reliable (did not work that well) 

void printRotations(motor l, motor r)//Pretty much what the function name says, prints both motors' roation numbers
{
  Brain.Screen.setCursor(1,1);//Puts the cursor on the first row and the first column
  Brain.Screen.print("Left rotations: ");//The text and variable names need to be in seprate liness 
  Brain.Screen.print(l.rotation(degrees));
  Brain.Screen.setCursor(2,1);//Puts the cursor on the second row and the first column 
  Brain.Screen.print("Right rotations: ");
  Brain.Screen.print(r.rotation(degrees));
}

void printSpeed(double speed, double speed2)//Same thing as the last function except it is for speed and it prints on the 3rd and 4th lines 
{
  Brain.Screen.setCursor(3,1);
  Brain.Screen.print("Left speed: ");
  Brain.Screen.print(speed);
  Brain.Screen.setCursor(4,1);
  Brain.Screen.print("Right speed: ");
  Brain.Screen.print(speed2);

}

void consoleTrack (double spd, double spd2, double err, double err2, double drv, double drv2, double integral, double integral2)//Prints the values for both motors to the terminal 
{
  printf("time:%f Left motor: speed %f, error %f, derivative %f, integral %f\n", Brain.Timer.value(), spd, err, drv, integral);
  printf("Right motor: speed %f, error %f, derivative %f, integral %f\n", spd2, err2, drv2, integral2);
}

//make into class (driving.go)
void go (double target, motor sally, motor bob, bool there)//The PID function, the bob motor is associated with all variables that has a two after and sally is associated with the other
{
  //Lines 76-96 is all of the variables used in this function 
  double kP = 1;
  double kD = 0.2;
  double kI = 0.005;
  double deltaTime = 0.004; //this is one of the sides of the rectangles that represents the integral
  double error = target; 
  double error2 = target;
  double integral = 0;
  double integral2 = 0;
  double derivative = 0;
  double derivative2 = 0;
  double prevErr = 0;
  double prevErr2 = 0;
  double range = 5;
  double currentDistance = sally.rotation(deg);
  double currentDistance2 = bob.rotation(deg);
  double speed;
  double speed2;
  double overflow = 250;//
  double cap = 5000000;//max
  //double rampBy = 0.05;
  int counter = 0;//a counting variable and is refrenced on line 158
  //clear encoders

  while((dabs(error) > range) || (dabs(error2) > range))//this is all of the fun PID math and everything else that goes along with it
  {
    currentDistance = sally.rotation(deg);//makes current distance the amount of rotations the motor has gone through
    error = target - currentDistance;//sets error to the distance that is left to go
    currentDistance2 = bob.rotation(deg);//makes current distance the amount of rotations the motor has gone through
    error2 = target - currentDistance2;//sets error to the distance that is left to go
    
    integral += error * deltaTime;//adds error *deltaTime to the integral
    derivative = error - prevErr;//the derivative is set as the diffrence between error and the previous error
    integral2 += error2 * deltaTime;//adds error *deltaTime to the integral for the other motor
    derivative2 = error2 - prevErr2;//the derivative is set as the diffrence between error and the previous error for the other motor
    
    if (dabs(error) > overflow)//if positive error is greater than overflow then integral is set to 0
    {
      integral = 0;
    }
    else if(dabs(error) < range)//if positive error is less than range then integral is set to 0
    {
      integral = 0;
    }

    if (dabs(error2) > overflow)//does the same thing as the function on 111 but for the other motor
    {
      integral2 = 0;
    }
    else if (dabs(error2) < range)//does the same thing as the function on 115 but for the other motor
    {
      integral2 = 0;
    }
    //target: 50 speed influence points max.
    if (integral > cap) //if integral is greater than it is supposed to be then it gets set to the max it can be (cap)
    {
      integral = cap;
    }
    else if (integral < -cap)//if integral is less than it is supposed to be then it gets set to the -cap
    {
      integral = -cap;
    }

    if (integral2 > cap) //does the same thing as the function on 129 but for the other motor
    {
      integral2 = cap;
    }
    else if (integral2 < -cap)//does the same thing as the function on 133 but for the other motor
    {
      integral2 = -cap;
    }


    
    speed = (error * kP) + (integral * kI) + (derivative * kD);//fun math
    speed2 = (error2 * kP) + (integral2 * kI) + (derivative2 * kD);//fun math for the other motor

    sally.spin(forward, speed, dps); //dps (degrees per second) deals in far larger numbers than percent
    bob.spin(forward, speed2, dps);//this line and the one before it tell the motors how fast the need to move forward in dps

    prevErr = error;//sets the prevErr as error for sally
    prevErr2 = error2;//sets the prevErr as error for bob

    counter++;//adds 1 to the counter, shows the amount of time that robot goes through the loop
    if(counter % 1500 == 0)
    {
      consoleTrack(speed, speed2, error, error2, derivative, derivative2, integral, integral2);
    }
    task::sleep(25);//waits 25 milliseconds
  }

  
  sally.stop();//turns off the two motors
  bob.stop();
  there = true;//changes the boolean to true
  Brain.Screen.newLine();
  Brain.Screen.print("Done");//prints done to the screen so we would know when it actually finshed and not just when it looked done
}

int main() 
{
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  bool done = true;//makes done true

  go(1000, leftDrive, rightDrive, done);//calls the PID function and goes 1000 rotations
}
