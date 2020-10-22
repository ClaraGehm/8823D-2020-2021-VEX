#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor driveRB = motor(PORT1, ratio18_1, false);
controller Controller1 = controller(primary);
motor driveLF = motor(PORT2, ratio18_1, true);
encoder rightE = encoder(Brain.ThreeWirePort.A);
encoder leftE = encoder(Brain.ThreeWirePort.E);
motor driveLB = motor(PORT15, ratio18_1, true);
motor driveRF = motor(PORT9, ratio18_1, false);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}