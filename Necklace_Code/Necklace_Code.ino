/* Aiden and Jonny (Date started= Summer 2018, Date completed 6/19/19): 
 *  Our tactile necklace was created with the goal of allowing a spatially disoriented wearer to be able to better understand his/her position relative to gravity. 
 *  The necklace operates using an accelerometer and gyroscope Arduino microcontroller which transmits data to an Arduino Nano. 
 *  The Nano then interprets the accelerometer/ gyroscope data and converts it into an output in the form of vibrations in the necklace. 
 *  The necklace consists of eight brush motor vibrators that pulsate independently based on the position of the sensor. 
 *  https://github.com/AidenKunkler-Peck/Tactile-Necklace
*/
//Including the libraries used in the code
#include <SoftPWM.h>
#include<Wire.h>

//establishing Global Variables
float oldvalue;//used later in averaging formula
float newvalue;//used later in averaging formula
float oldvalueAccelX;//used for averaging accelerometer values in the x-axis
float oldvalueAccelY;//used for averaging accelerometer values in the y-axis
float oldvalueAccelZ;//used for averaging accelerometer values in the z-axis
float oldvalueGyroX;//used for averaging gyroscope values in the x-axis
float oldvalueGyroY;//used for averaging gyroscope values in the y-axis
float oldvalueGyroZ;//used for averaging gyroscope values in the z-axis
//float oldvalueTemp;
float newvalueAccelX;
float newvalueAccelY;
float newvalueAccelZ;
float newvalueGyroX; 
float newvalueGyroY;
float newvalueGyroZ;
//float newvalueTemp;
float zerox;
float zeroy;
float avgx;
float avgy;
int vpins[8]={2,3,4,5,6,9,10,11};
float accx=0; //accelerometer data is non integer values, with decimals
float accy=0; //data inputted from sensor has x and y axes
const int maxacc=16500;//Largest input value from accelerometer
const int vMax=255;//max strength of vibrator is 255, exceeding 255 with overflow, the byte will read the 8 rightmost bits

int myValues[8]; //defining a new array

const int MPU6050_addr=0x68;
int16_t AccX,AccY,AccZ,GyroX,GyroY,GyroZ;
//Beginning communication with the accelerometer/gyroscopre Arduino "Wire.beginTransmission"
void getValues(){
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);  
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
//  Temp=Wire.read()<<8|Wire.read();
  GyroX=Wire.read()<<8|Wire.read();
  GyroY=Wire.read()<<8|Wire.read();
  GyroZ=Wire.read()<<8|Wire.read();
}
//Individual Vibraor Strength from 0-255
void clearTacts(int*  tactArray) {
  for (int i=0; i<=7; i++) {
      tactArray[i]=0;
  }
}
//tactValues=acquiring the vibrator strength values from the accelerometer/gyroscope Arduino
//tactArray=formula for converting Arduino acceleroemter/gyroscope values to output tactor strength values (each tactor has a seperate formula specific to the desired output of each vibrator relative to the orientation of the Arduino)
//"if"/"else if"=if the conditions of the "if" function are met then the code within the function is carried out, if the conditions are not met the next "else if" function is evaluated
void tactValues(float accx, float accy, int* tactArray){
  clearTacts(tactArray);
  if (accy<0 && accx>0){
     tactArray[0]=((abs(accy)-zeroy)/64)+30;
     tactArray[1]=sqrt(pow(accx-zerox,2)+pow(accy+zeroy,2))/64;
     tactArray[2]=(accx-zerox)/64;  
  }
  else if (accy<0 && accx<0){
    tactArray[2]=(abs(accx)-zerox)/64;
    tactArray[3]=sqrt(pow(accx+zerox,2)+pow(accy+zeroy,2))/64;
    tactArray[4]=((abs(accy)-zeroy)/64)+30;
  }
  else if (accy>0 && accx<0){
    tactArray[4]=(abs(accy-zeroy)/64)+30;
    tactArray[5]=sqrt(pow(accx+zerox,2)+pow(accy-zeroy,2))/64;
    tactArray[6]=(abs(accx)-zerox)/64;  
  }
  else if (accy>0 && accx>0){
    tactArray[0]=((accy-zeroy)/64)+30;  
    tactArray[6]=(abs(accx)-zerox)/64;
    tactArray[7]=sqrt(pow(accx-zerox,2)+pow(accy-zeroy,2))/64;
  }
}
//turns on each tactor individually then turns that same tactor off so that the vibrators turn on in a circle
void Circle (){
  for(int i=0; i<=7; i++){
    SoftPWMSet(vpins[i],255);
    delay(125);
    SoftPWMSet(vpins[i],0);
  }
}
//turns all vibrators on and then off to simulate a pulsation
void Pulse (){
   for(int i=0; i<8; i++){
    SoftPWMSet(vpins[i],255);
   }
  delay(125);
  for(int i=0; i<8; i++){
    SoftPWMSet(vpins[i],0);
   }
  delay(125);
}
int scaler(float input){
  if (input<30){
     return (0);
  }
  else {
    return (50+(130*(input/255))); //want your min to be 34 because it is at the point where it first starts to be noticeable, max is lower than 255 because that is the maximum vibration strength we deemed necessary
//  needed to lower vibration strength even lower because voltage was increased from 5V to 7.4, so the new numbers are 69% of original numbers
  }
}
void setup() {
  // put your setup code here, to run once:
  SoftPWMBegin();
  Wire.begin();
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
  for(int i=0; i<=7; i++) {
    pinMode(vpins[i], OUTPUT);
  }
  Circle();
  Circle();
  getValues(); //get acc values
  oldvalueAccelX = AccX;
  oldvalueAccelY = AccY;
  oldvalueAccelZ = AccZ;
  oldvalueGyroX = GyroX;
  oldvalueGyroY = GyroY;
  oldvalueGyroZ = GyroZ;
//  oldvalueTemp = Temp;
  for(int avg = 0;avg < 150;avg++){
    getValues();
    newvalueAccelX = AccX;
    newvalueAccelY = AccY;
    newvalueAccelZ = AccZ;
    newvalueGyroX = GyroX;
    newvalueGyroY = GyroY;
    newvalueGyroZ = GyroZ;
//    newvalueTemp = Temp;
    oldvalueAccelX = (oldvalueAccelX + newvalueAccelX)/2;
    oldvalueAccelY = (oldvalueAccelY + newvalueAccelY)/2;
    oldvalueAccelZ = (oldvalueAccelZ + newvalueAccelZ)/2;
    oldvalueGyroX = (oldvalueGyroX + newvalueGyroX)/2;
    oldvalueGyroY = (oldvalueGyroY + newvalueGyroY)/2;
    oldvalueGyroZ = (oldvalueGyroZ + newvalueGyroZ)/2;
//    oldvalueTemp = (oldvalueTemp + newvalueTemp)/2;
  }
  zerox=oldvalueAccelX;
  zeroy=oldvalueAccelY;
delay(500);
Pulse();
Pulse();
}
void loop() {
  getValues(); //get acc values
  oldvalueAccelX = AccX;
  oldvalueAccelY = AccY;
  oldvalueAccelZ = AccZ;
  oldvalueGyroX = GyroX;
  oldvalueGyroY = GyroY;
  oldvalueGyroZ = GyroZ;
//  oldvalueTemp = Temp;
  for(int avg = 0;avg < 50;avg++){
    getValues();
    newvalueAccelX = AccX;
    newvalueAccelY = AccY;
    newvalueAccelZ = AccZ;
    newvalueGyroX = GyroX;
    newvalueGyroY = GyroY;
    newvalueGyroZ = GyroZ;
//    newvalueTemp = Temp;
    oldvalueAccelX = (oldvalueAccelX + newvalueAccelX)/2;
    oldvalueAccelY = (oldvalueAccelY + newvalueAccelY)/2;
    oldvalueAccelZ = (oldvalueAccelZ + newvalueAccelZ)/2;
    oldvalueGyroX = (oldvalueGyroX + newvalueGyroX)/2;
    oldvalueGyroY = (oldvalueGyroY + newvalueGyroY)/2;
    oldvalueGyroZ = (oldvalueGyroZ + newvalueGyroZ)/2;
//    oldvalueTemp = (oldvalueTemp + newvalueTemp)/2;
  }
  tactValues(oldvalueAccelX,oldvalueAccelY, myValues);
  for (int i=0; i<=7; i++) {
    SoftPWMSet(vpins[i], scaler(myValues[i]));
  }

  for(int i=0;i<8;i++){
    Serial.print("tact ");
    Serial.print(i);
    Serial.print("= ");
    Serial.println(scaler(myValues[i]));
  }
}
