#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor driveLB = motor(PORT15, ratio6_1, true);
distance locator = distance(PORT4);
motor driveRB = motor(PORT1, ratio6_1, false);
inertial Inertial3 = inertial(PORT3);
motor driveRF = motor(PORT9, ratio6_1, false);
motor driveLF = motor(PORT2, ratio6_1, true);
motor rightIntake = motor(PORT19, ratio18_1, true);
motor leftIntake = motor(PORT12, ratio18_1, false);
motor indexerMotor = motor(PORT11, ratio6_1, false);
encoder leftE = encoder(Brain.ThreeWirePort.A);
encoder rightE = encoder(Brain.ThreeWirePort.C);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}