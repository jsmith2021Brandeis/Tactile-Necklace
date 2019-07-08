/* Aiden and Jonny (Date started= Summer 2018, Date completed 6/19/19): 
 *  Our tactile necklace was created with the goal of allowing a spatially disoriented wearer to be able to better understand his/her position relative to gravity. 
 *  The necklace operates using an accelerometer and gyroscope Arduino microcontroller which transmits data to an Arduino Nano. 
 *  The Nano then interprets the accelerometer/ gyroscope data and converts it into an output in the form of vibrations in the necklace. 
 *  The necklace consists of eight brush motor vibrators that pulsate independently based on the position of the sensor. 
 *  https://github.com/AidenKunkler-Peck/Tactile-Necklace
 TO DO: allow pins to be any number 
*/

//Including the libraries used in the code
#include <SoftPWM.h>
#include<Wire.h>
#include <TactNecklace.h>
#define NUMSAMPLES 150
#if defined (ARDUINO_AVR_UNO)|| defined (ARDUINO_AVR_NANO)
//constructor to create a TactNecklace object
void TactNecklace::begin(int* vPins, int numPins, string sensor) {
  // put your setup code here, to run once:
  SoftPWMBegin();
  Wire.begin();
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.println(numPins);
  this->numPins=numPins;
  if (numPins!=4&&numPins!=8){
	 Serial.println("Program not compatible with the number of pins you are currently using. Please use either 4 or 8 pins.");//Error statement if 4 or 8 pins aren't used by the Arduino.
	 //Infinite loop to stop program from running. Works because 1 is always equal to 1, so nothing happens as long as it is true.
	 while (1==1){
	 }
  }
  this->vPins=vPins;
  for(int i=0; i<numPins; i++) {
    pinMode((vPins[i]), OUTPUT);
  }
  getValues(); //get acc values
  oldvalueAccelX = AccX;
  oldvalueAccelY = AccY;
  oldvalueAccelZ = AccZ;
  oldvalueGyroX = GyroX;
  oldvalueGyroY = GyroY;
  oldvalueGyroZ = GyroZ;
  for(int avg = 0;avg < NUMSAMPLES;avg++){
    getValues();
    newvalueAccelX = AccX;
    newvalueAccelY = AccY;
    newvalueAccelZ = AccZ;
    newvalueGyroX = GyroX;
    newvalueGyroY = GyroY;
    newvalueGyroZ = GyroZ;
    oldvalueAccelX = (oldvalueAccelX + newvalueAccelX)/2;
    oldvalueAccelY = (oldvalueAccelY + newvalueAccelY)/2;
    oldvalueAccelZ = (oldvalueAccelZ + newvalueAccelZ)/2;
    oldvalueGyroX = (oldvalueGyroX + newvalueGyroX)/2;
    oldvalueGyroY = (oldvalueGyroY + newvalueGyroY)/2;
    oldvalueGyroZ = (oldvalueGyroZ + newvalueGyroZ)/2;
  }
  zerox=oldvalueAccelX;
  zeroy=oldvalueAccelY;
  delay(500);
  Serial.println("Tactile Necklace Initialized");
}
//turns all vibrators on and then off to simulate a pulsation
void TactNecklace::pulse (){
   for(int i=0; i<numPins; i++){
    SoftPWMSet(vPins[i],255);
   }
  delay(125);
  for(int i=0; i<numPins; i++){
    SoftPWMSet(vPins[i],0);
   }
  delay(125);
}
//turns on each tactor individually then turns that same tactor off so that the vibrators turn on in a circle
void TactNecklace::circle (){
  for(int i=0; i<numPins; i++){
    SoftPWMSet(vPins[i],255);
    delay(125);
    SoftPWMSet(vPins[i],0);
  }
}
//gets acceleration values from the accelerometer
int[] TactNecklace:: readAccel(){
  getValues(); //get acc values
  oldvalueAccelX = AccX;//setting acquired AccX value to oldvalueAccelX to be used in averaging
  oldvalueAccelY = AccY;
  oldvalueAccelZ = AccZ;
  oldvalueGyroX = GyroX;
  oldvalueGyroY = GyroY;
  oldvalueGyroZ = GyroZ;
  for(int avg = 0;avg < 3;avg++){//for loop for averaging (averaged 50 times)
    getValues();//get values again and name them new values to be used in averaging 
    newvalueAccelX = AccX;
    newvalueAccelY = AccY;
    newvalueAccelZ = AccZ;
    newvalueGyroX = GyroX;
    newvalueGyroY = GyroY;
    newvalueGyroZ = GyroZ;
    oldvalueAccelX = (oldvalueAccelX + newvalueAccelX)/2;//averaging old and new values to get a more accurate reading of the accelerometer and gyroscope data
    oldvalueAccelY = (oldvalueAccelY + newvalueAccelY)/2;
    oldvalueAccelZ = (oldvalueAccelZ + newvalueAccelZ)/2;
    oldvalueGyroX = (oldvalueGyroX + newvalueGyroX)/2;
    oldvalueGyroY = (oldvalueGyroY + newvalueGyroY)/2;
    oldvalueGyroZ = (oldvalueGyroZ + newvalueGyroZ)/2;
  }

//for troubleshooting in the serial monitor
  Serial.print("tact #:power");
  for(int i=0;i<numPins;i++){
    Serial.print("\t");
    Serial.print(scaler(myValues[i]));
	Serial.print("\t");
  }
  Serial.print("AccelX:");
  Serial.print(oldvalueAccelX);
  Serial.print("\t");
  Serial.print("ZeroX:");
  Serial.print(zerox);
  Serial.print("\t");
  Serial.print("AccelY:");
  Serial.print(oldvalueAccelY);
  Serial.print("\t");
  Serial.print("ZeroY:");
  Serial.println(zeroy);
}
//sends values to the vibrator pins which determines the strength of the vibration
void TactNecklace::  sendVibration(){  
  if (numPins==8){
	tactValues8a(oldvalueAccelX,oldvalueAccelY, myValues);//sets accelerometer and gyroscope data to pins for 8 vibrators
	}
	//We know that this must be 4 pins because in the initialization, the only possibilities that cause the program to run are 4 pins and 8 pins. And if it's nto 8 pins, then the only remaining option is 4 pins.
	else {
		tactValues4a(oldvalueAccelX,oldvalueAccelY, myValues);//sets accelerometer and gyroscope data to pins for 4 vibrators
	}
  for (int i=0; i<numPins; i++) {//sets each accelerometer value to the designated ping (1-8)
    SoftPWMSet(vPins[i], scaler(myValues[i]));
  }

}
//want your min to be 34 because it is at the point where it first starts to be noticeable, max is lower than 255 because that is the maximum vibration strength we deemed necessary
//  needed to lower vibration strength even lower because voltage was increased from 5V to 7.4, so the new numbers are 69% of original numbers
int TactNecklace::scaler(float input){
  if (input<30){
     return (0);
  }
  else {
    return (50+(130*(input/255)));
  }
}
//Beginning communication with the accelerometer/gyroscopre Arduino "Wire.beginTransmission"
void TactNecklace::getValues(){
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);  
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
  GyroX=Wire.read()<<8|Wire.read();
  GyroY=Wire.read()<<8|Wire.read();
  GyroZ=Wire.read()<<8|Wire.read();
}
//Individual Vibraor Strength from 0-255
void TactNecklace::clearTacts(int*  tactArray) {
  for (int i=0; i<numPins; i++) {
      tactArray[i]=0;
  }
}
//tactValues8=acquiring the vibrator strength values from the accelerometer/gyroscope Arduino with 8 tactors
//tactValues4=acquiring the vibrator strength values from the accelerometer/gyroscope Arduino with 4 tactors
//tactArray=formula for converting Arduino acceleroemter/gyroscope values to output tactor strength values (each tactor has a seperate formula specific to the desired output of each vibrator relative to the orientation of the Arduino)
//"if"/"else if"=if the conditions of the "if" function are met then the code within the function is carried out, if the conditions are not met the next "else if" function is evaluated
void TactNecklace::tactValues8a(float accx, float accy, int* tactArray){
  clearTacts(tactArray);
  if (accy<0 && accx>0){
    tactArray[0]=((abs(accy)-zeroy)/64)+30;
    tactArray[1]=sqrt(pow(accx-zerox,2)+pow(accy+zeroy,2))/64;
    tactArray[2]=(accx-zerox)/64;
	Serial.print("Q1");
	Serial.println("\t");
  }
  else if (accy<0 && accx<0){
    tactArray[2]=(abs(accx)-zerox)/64;
    tactArray[3]=sqrt(pow(accx+zerox,2)+pow(accy+zeroy,2))/64;
	tactArray[4]=((abs(accy)-zeroy)/64)+30;
	Serial.print("Q2");
	Serial.println("\t");
  }
  else if (accy>0 && accx<0){
    tactArray[4]=(abs(accy-zeroy)/64)+30;
    tactArray[5]=sqrt(pow(accx+zerox,2)+pow(accy-zeroy,2))/64;
    tactArray[6]=(abs(accx)-zerox)/64;
	Serial.print("Q3");	
	Serial.println("\t");
  }
  else if (accy>0 && accx>0){
    tactArray[0]=((accy-zeroy)/64)+30;  
    tactArray[6]=(abs(accx)-zerox)/64;
    tactArray[7]=sqrt(pow(accx-zerox,2)+pow(accy-zeroy,2))/64;
	Serial.print("Q4");
	Serial.println("\t");
  }
}
void TactNecklace::tactValues4a(float accx, float accy, int* tactArray){
  clearTacts(tactArray);
  if (accy<0 && accx>0){
	tactArray[0]=((abs(accy)-zeroy)/64)+30;
	Serial.print("Q1");
	Serial.println("\t");
  }
  else if (accy<0 && accx<0){
	tactArray[1]=(abs(accx)-zerox)/64;
	Serial.print("Q2");
	Serial.println("\t");
  }
  else if (accy>0 && accx<0){
	tactArray[2]=((abs(accy)-zeroy)/64)+30;
	Serial.print("Q3");	
	Serial.println("\t");	
  }
  else if (accy>0 && accx>0){
	tactArray[3]=(abs(accx)-zerox)/64;
	Serial.print("Q4");
	Serial.println("\t");
  }
}
#else
#endif