/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\User                                             */
/*    Created:      Tue Sep 15 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Vision20             vision        19              
// RightMotor           motor         3               
// LeftMotor            motor         1               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // while (1 == 1)
  //{
  // Brain.Screen.clearLine();
  // Vision20.takeSnapshot(Vision20__SIG_2);
  // if (Vision20.largestObject.exists)
  // {
  //   Brain.Screen.print("W: %d", Vision20.largestObject.width);
  //   Brain.Screen.print("      H: %d", Vision20.largestObject.height);
  // }
  // else
  // {
  //   Brain.Screen.print("Vision Sensor: Color Signature Not FOund");
  // }
  // task::sleep(100);
  int x = 0;
  int center = 158; // The x coordinate for the center of the vision sensor
  int OKError = 50; // Used to set a range of values to count is being just in
  while (true) 
  {
    Vision20.takeSnapshot(Vision20__SIG_2);
    if (Vision20.largestObject.exists) {
      x = Vision20.largestObject.centerX;
      if (x < (center - OKError)) // If the object is to the left of center
      {
        RightMotor.spin(directionType::fwd, 25, velocityUnits::pct);
        LeftMotor.spin(directionType::fwd, 25, velocityUnits::pct);
      } else if (x >
                 center + OKError) // If the object is to the right of center
      {
        RightMotor.spin(directionType::fwd, 25, velocityUnits::pct);
        LeftMotor.spin(directionType::rev, 25, velocityUnits::pct);
      } else // The object is not to the right of center and not to the left
             // of center
      {
        LeftMotor.stop(brakeType::brake);
        RightMotor.stop(brakeType::brake);
      }
    }
    task::sleep(100);
  }
}
