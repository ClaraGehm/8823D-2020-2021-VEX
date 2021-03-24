#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor driveLF = motor(PORT15, ratio18_1, true);
motor driveRF = motor(PORT16, ratio18_1, false);
controller Controller1 = controller(primary);
motor driveLB = motor(PORT5, ratio18_1, true);
motor driveRB = motor(PORT6, ratio18_1, false);
motor flywheel = motor(PORT9, ratio6_1, false);
motor indexerMotor = motor(PORT2, ratio6_1, false);
motor leftIntake = motor(PORT10, ratio6_1, false);
motor rightIntake = motor(PORT20, ratio6_1, true);
limit BallExit = limit(Brain.ThreeWirePort.H);
limit BallDetect = limit(Brain.ThreeWirePort.G);
encoder leftEncoder = encoder(Brain.ThreeWirePort.C);
encoder rightEncoder = encoder(Brain.ThreeWirePort.A);
inertial leftInertial = inertial(PORT1);
inertial rightInertial = inertial(PORT19);
distance locator = distance(PORT4);

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