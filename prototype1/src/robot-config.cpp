#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor driveLB = motor(PORT1, ratio18_1, false);
motor leftIntake = motor(PORT2, ratio18_1, false);
motor rightIntake = motor(PORT3, ratio18_1, true);
motor indexerMotor = motor(PORT4, ratio18_1, false);
motor flywheel = motor(PORT8, ratio6_1, true);
motor driveRB = motor(PORT10, ratio18_1, true);
motor driveRF = motor(PORT9, ratio18_1, false);
motor driveLF = motor(PORT5, ratio18_1, true);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}