#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor driveLB = motor(PORT15, ratio6_1, true);
motor leftIntake = motor(PORT12, ratio18_1, false);
motor rightIntake = motor(PORT19, ratio18_1, true);
motor indexerMotor = motor(PORT11, ratio6_1, false);
motor flywheel = motor(PORT20, ratio6_1, true);
motor driveRB = motor(PORT1, ratio6_1, false);
motor driveRF = motor(PORT9, ratio6_1, false);
motor driveLF = motor(PORT2, ratio6_1, true);
/*vex-vision-config:begin*/
signature eyes__REDB = signature (1, 6017, 8447, 7232, -813, 65, -374, 2.6, 0);
signature eyes__BLUEB = signature (2, -3073, -2001, -2538, 7697, 11977, 9838, 2.5, 0);
vision eyes = vision (PORT16, 50, eyes__REDB, eyes__BLUEB);
/*vex-vision-config:end*/
bumper BumperG = bumper(Brain.ThreeWirePort.G);
bumper BumperH = bumper(Brain.ThreeWirePort.H);
encoder rightE = encoder(Brain.ThreeWirePort.A);
encoder leftE = encoder(Brain.ThreeWirePort.E);
inertial InertialSensor = inertial(PORT3);
limit BallDetect = limit(Brain.ThreeWirePort.C);

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