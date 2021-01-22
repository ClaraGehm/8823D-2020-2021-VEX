#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
inertial InertialSensor = inertial(PORT3);
encoder leftE = encoder(Brain.ThreeWirePort.E);
encoder rightE = encoder(Brain.ThreeWirePort.A);
/*vex-vision-config:begin*/
signature eyes__REDB = signature (1, 4997, 6433, 5716, -1765, -421, -1094, 3, 0);
signature eyes__BLUEB = signature (2, -2437, -1591, -2014, 2415, 3937, 3176, 3, 0);
vision eyes = vision (PORT16, 50, eyes__REDB, eyes__BLUEB);
/*vex-vision-config:end*/
limit BallDetect = limit(Brain.ThreeWirePort.C);
limit BallExit = limit(Brain.ThreeWirePort.D);
controller Controller1 = controller(primary);
motor driveLB = motor(PORT15, ratio6_1, true);
motor leftIntake = motor(PORT12, ratio18_1, false);
motor rightIntake = motor(PORT19, ratio18_1, true);
motor indexerMotor = motor(PORT11, ratio6_1, false);
motor flywheel = motor(PORT20, ratio6_1, true);
motor driveRB = motor(PORT1, ratio6_1, false);
motor driveRF = motor(PORT9, ratio6_1, false);
motor driveLF = motor(PORT2, ratio6_1, true);
limit LIntakeLimit = limit(Brain.ThreeWirePort.G);
limit RIntakeLimit = limit(Brain.ThreeWirePort.H);

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