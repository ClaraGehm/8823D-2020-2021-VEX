#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor leftDrive = motor(PORT1, ratio18_1, false);
controller Controller1 = controller(primary);
motor rightDrive = motor(PORT10, ratio18_1, true);
motor claw = motor(PORT3, ratio18_1, false);
motor arm = motor(PORT8, ratio18_1, false);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}