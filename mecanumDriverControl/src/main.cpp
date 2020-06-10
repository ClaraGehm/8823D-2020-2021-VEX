/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Clara Gehm                                                */
/*    Created:      Wed Jun 10 2020                                           */
/*    Description:  Driver control program for a mecanum drive (H style)      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// frontLeft            motor         1               
// frontRight           motor         2               
// backLeft             motor         3               
// backRight            motor         4               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int main() 
{
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  while (1==1)
  {
    int forward = Controller1.Axis3.position(vex::percent);
    int sideways = Controller1.Axis4.position(vex::percent);
    int turn = Controller1.Axis1.position(vex::percent);

    frontRight.spin(vex::forward, forward - sideways + turn, vex::percent);
    frontLeft.spin(vex::forward,  forward + sideways - turn, vex::percent);
    backRight.spin(vex::forward,  forward + sideways + turn, vex::percent);
    backLeft.spin(vex::forward,   forward - sideways - turn, vex::percent);
  }
}
