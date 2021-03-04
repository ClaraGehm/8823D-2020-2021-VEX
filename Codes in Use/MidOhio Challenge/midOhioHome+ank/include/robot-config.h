using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor driveLF;
extern motor driveRF;
extern controller Controller1;
extern motor driveLB;
extern motor driveRB;
extern motor flywheel;
extern motor indexerMotor;
extern motor leftIntake;
extern motor rightIntake;
extern limit BallExit;
extern limit BallDetect;
extern encoder leftEncoder;
extern encoder rightEncoder;
extern inertial leftInertial;
extern inertial rightInertial;
extern distance locator;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );