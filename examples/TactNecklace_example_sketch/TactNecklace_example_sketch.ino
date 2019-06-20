/* Aiden and Jonny (Date started= Summer 2018, Date completed 6/19/19): 
 *  Our tactile necklace was created with the goal of allowing a spatially disoriented wearer to be able to better understand his/her position relative to gravity. 
 *  The necklace operates using an accelerometer and gyroscope Arduino microcontroller which transmits data to an Arduino Nano. 
 *  The Nano then interprets the accelerometer/ gyroscope data and converts it into an output in the form of vibrations in the necklace. 
 *  The necklace consists of eight brush motor vibrators that pulsate independently based on the position of the sensor. 
 *  https://github.com/AidenKunkler-Peck/Tactile-Necklace
*/
//Including the libraries used in the code
#include "TactNecklace.h"
//establishing Global Variables
TactNecklace myNecklace();
//runs once, gets the necklace ready
void setup() {
  int vpins[8]={2,3,4,5,6,9,10,11};//sets pins to the Arduino ports
  myNecklace.begin();//initializes communication with the Arduino
  myNecklace.circle();//vibrators pulsate one at a time clockwise
  myNecklace.pulse();//vibrators pulsate all at the same time
}
//runs forever
void loop() {
  myNecklace.sendVibration();//reads acceleration values and pulses vibrators on that axis
}
