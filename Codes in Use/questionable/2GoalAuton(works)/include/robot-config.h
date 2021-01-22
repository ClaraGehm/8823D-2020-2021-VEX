using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern motor driveLB;
extern motor leftIntake;
extern motor rightIntake;
extern motor indexerMotor;
extern motor flywheel;
extern controller Controller1;
extern motor driveRB;
extern motor driveRF;
extern motor driveLF;
extern signature eyes__SIG_1;
extern signature eyes__SIG_2;
extern signature eyes__SIG_3;
extern signature eyes__SIG_4;
extern signature eyes__SIG_5;
extern signature eyes__SIG_6;
extern signature eyes__SIG_7;
extern vision eyes;
extern bumper BumperG;
extern bumper BumperH;
extern encoder rightE;
extern encoder leftE;
extern inertial InertialSensor;
extern limit BallDetect;
extern limit BallExit;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );