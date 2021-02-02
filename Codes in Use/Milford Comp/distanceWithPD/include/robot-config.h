using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor driveLB;
extern distance locator;
extern motor driveRB;
extern inertial Inertial3;
extern motor driveRF;
extern motor driveLF;
extern motor rightIntake;
extern motor leftIntake;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );