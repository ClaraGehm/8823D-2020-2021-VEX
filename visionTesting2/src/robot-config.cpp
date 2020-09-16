#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
/*vex-vision-config:begin*/
signature Vision20__SIG_2 = signature (2, 3639, 7281, 5460, -897, 1, -448, 1.6, 0);
signature Vision20__SIG_3 = signature (3, 0, 0, 0, 0, 0, 0, 3, 0);
vision Vision20 = vision (PORT19, 61, Vision20__SIG_2, Vision20__SIG_3);
/*vex-vision-config:end*/
motor RightMotor = motor(PORT3, ratio18_1, false);
motor LeftMotor = motor(PORT1, ratio18_1, true);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}