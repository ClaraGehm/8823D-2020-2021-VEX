using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor driveLB;
extern motor leftIntake;
extern motor rightIntake;
extern motor indexerMotor;
extern motor flywheel;
extern motor driveRB;
extern motor driveRF;
extern motor driveLF;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );