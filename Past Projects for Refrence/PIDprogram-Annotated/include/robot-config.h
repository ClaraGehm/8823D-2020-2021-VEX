using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor leftDrive;
extern controller Controller1;
extern motor rightDrive;
extern motor claw;
extern motor arm;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );