using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern inertial InertialSensor;
extern encoder leftE;
extern encoder rightE;
extern signature eyes__REDB;
extern signature eyes__BLUEB;
extern signature eyes__SIG_3;
extern signature eyes__SIG_4;
extern signature eyes__SIG_5;
extern signature eyes__SIG_6;
extern signature eyes__SIG_7;
extern vision eyes;
extern limit BallDetect;
extern limit BallExit;
extern controller Controller1;
extern motor driveLB;
extern motor leftIntake;
extern motor rightIntake;
extern motor indexerMotor;
extern motor flywheel;
extern motor driveRB;
extern motor driveRF;
extern motor driveLF;
extern optical OpticalSensor;
extern distance DistanceSensor;
extern limit BumperG;
extern limit BumperH;
extern distance locator;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );