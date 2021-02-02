/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\leeperp                                          */
/*    Created:      Tue Jan 12 2021                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Motor10              motor         10              
// Distance1            distance      1               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int main() {
vexcodeInit();

char buffer[100];  //need a temp space to store data which will be written to the sd card
  //                 creates an array (list) 100 characters long
 
    int name = sprintf(buffer, "Distance, Spd Variable, Motor Speed \n");  
    //insted of printing to a terminal, we are printing a string to a variable
    Brain.SDcard.savefile("Distance Sensor Pcontrol.csv", (uint8_t *)buffer, name);
    // savefile needs: name, buffer, length
    // uint8_t  unsigned integer of 8 bits(1 byte)
  

  while (Distance1.objectDistance(inches) > 1)
   {
     int spd = Distance1.objectDistance(inches);
     Brain.Screen.print(spd);
     Motor10.setVelocity(spd, rpm); 
     Motor10.spin(forward);
     int pos = sprintf(buffer, "%f, %d, %f, \n", Distance1.objectDistance(inches), spd, Motor10.velocity(rpm));
      //sprintf takes a string and stores it as an integer
     Brain.SDcard.appendfile("Distance Sensor Pcontrol.csv", (uint8_t *)buffer, pos);
        
    wait(100, msec);
  
  }
  Motor10.stop();
}  

