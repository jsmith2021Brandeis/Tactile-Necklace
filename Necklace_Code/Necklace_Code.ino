#include "SoftPWM.h"
#include<Wire.h>

//Global Variables
float oldvalue;
float newvalue;
float oldvalueAccelX;
float oldvalueAccelY;
float oldvalueAccelZ;
float oldvalueGyroX;
float oldvalueGyroY;
float oldvalueGyroZ;
float oldvalueTemp;
float newvalueAccelX;
float newvalueAccelY;
float newvalueAccelZ;
float newvalueGyroX; 
float newvalueGyroY;
float newvalueGyroZ;
float newvalueTemp;
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
int16_t AccX,AccY,AccZ,Temp,GyroX,GyroY,GyroZ;

void getValues(){
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);  
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
  Temp=Wire.read()<<8|Wire.read();
  GyroX=Wire.read()<<8|Wire.read();
  GyroY=Wire.read()<<8|Wire.read();
  GyroZ=Wire.read()<<8|Wire.read();
}
//Individual Vibraor Strength from 0-255
void tactValues(float accx, float accy, int* tactArray){
  if (accy<0 && accx>0){
     tactArray[0]=((abs(accy)-zeroy)/64)+30;
     tactArray[1]=sqrt(pow(accx-zerox,2)+pow(accy+zeroy,2))/64;
     tactArray[2]=(accx-zerox)/64;
     tactArray[3]=0;
     tactArray[4]=0;
     tactArray[5]=0;
     tactArray[6]=0;
     tactArray[7]=0;  
  }
  else if (accy<0 && accx<0){
    tactArray[0]=0;
    tactArray[1]=0;
    tactArray[2]=(abs(accx)-zerox)/64;
    tactArray[3]=sqrt(pow(accx+zerox,2)+pow(accy+zeroy,2))/64;
    tactArray[4]=((abs(accy)-zeroy)/64)+30;
    tactArray[5]=0;
    tactArray[6]=0;
    tactArray[7]=0;
  }
  else if (accy>0 && accx<0){
    tactArray[0]=0;
    tactArray[1]=0;
    tactArray[2]=0;
    tactArray[3]=0;
    tactArray[4]=(abs(accy-zeroy)/64)+30;
    tactArray[5]=sqrt(pow(accx+zerox,2)+pow(accy-zeroy,2))/64;
    tactArray[6]=(abs(accx)-zerox)/64;
    tactArray[7]=0;  
  }
  else if (accy>0 && accx>0){
    tactArray[0]=((accy-zeroy)/64)+30;  
    tactArray[1]=0;
    tactArray[2]=0;
    tactArray[3]=0;
    tactArray[4]=0;
    tactArray[5]=0;
    tactArray[6]=(abs(accx)-zerox)/64;
    tactArray[7]=sqrt(pow(accx-zerox,2)+pow(accy-zeroy,2))/64;
  }
  else{
    Serial.println("uh oh! something failed!");
    for (int i=0; i<=7; i++) {
      tactArray[i]=0;  
    }
    tactArray[0]=0;
    tactArray[1]=0;
    tactArray[2]=0;
    tactArray[3]=0;
    tactArray[4]=0;
    tactArray[5]=0;
    tactArray[6]=0;
    tactArray[7]=0;  
  }
  //return(tactArray[0,1,2,3,4,5,6,7]);
}

void Circle (){
  for(int i=0; i<=7; i++){
    SoftPWMSet(vpins[i],255);
    delay(125);
    SoftPWMSet(vpins[i],0);
  }
}
void Pulse (){
  SoftPWMSet(vpins[0],255);
  SoftPWMSet(vpins[1],255);
  SoftPWMSet(vpins[2],255);
  SoftPWMSet(vpins[3],255);
  SoftPWMSet(vpins[4],255);
  SoftPWMSet(vpins[5],255);
  SoftPWMSet(vpins[6],255);
  SoftPWMSet(vpins[7],255);
  delay(125);
  SoftPWMSet(vpins[0],0);
  SoftPWMSet(vpins[1],0);
  SoftPWMSet(vpins[2],0);
  SoftPWMSet(vpins[3],0);
  SoftPWMSet(vpins[4],0);
  SoftPWMSet(vpins[5],0);
  SoftPWMSet(vpins[6],0);
  SoftPWMSet(vpins[7],0);
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
  oldvalueTemp = Temp;
  for(int avg = 0;avg < 150;avg++){
    getValues();
    newvalueAccelX = AccX;
    newvalueAccelY = AccY;
    newvalueAccelZ = AccZ;
    newvalueGyroX = GyroX;
    newvalueGyroY = GyroY;
    newvalueGyroZ = GyroZ;
    newvalueTemp = Temp;
    oldvalueAccelX = (oldvalueAccelX + newvalueAccelX)/2;
    oldvalueAccelY = (oldvalueAccelY + newvalueAccelY)/2;
    oldvalueAccelZ = (oldvalueAccelZ + newvalueAccelZ)/2;
    oldvalueGyroX = (oldvalueGyroX + newvalueGyroX)/2;
    oldvalueGyroY = (oldvalueGyroY + newvalueGyroY)/2;
    oldvalueGyroZ = (oldvalueGyroZ + newvalueGyroZ)/2;
    oldvalueTemp = (oldvalueTemp + newvalueTemp)/2;
  }
  zerox=oldvalueAccelX;
  zeroy=oldvalueAccelY;
delay(500);
Pulse();
Pulse();
}


void loop() {
  // put your main code here, to run repeatedly:
  getValues(); //get acc values
  oldvalueAccelX = AccX;
  oldvalueAccelY = AccY;
  oldvalueAccelZ = AccZ;
  oldvalueGyroX = GyroX;
  oldvalueGyroY = GyroY;
  oldvalueGyroZ = GyroZ;
  oldvalueTemp = Temp;
  for(int avg = 0;avg < 50;avg++){
    getValues();
    newvalueAccelX = AccX;
    newvalueAccelY = AccY;
    newvalueAccelZ = AccZ;
    newvalueGyroX = GyroX;
    newvalueGyroY = GyroY;
    newvalueGyroZ = GyroZ;
    newvalueTemp = Temp;
    oldvalueAccelX = (oldvalueAccelX + newvalueAccelX)/2;
    oldvalueAccelY = (oldvalueAccelY + newvalueAccelY)/2;
    oldvalueAccelZ = (oldvalueAccelZ + newvalueAccelZ)/2;
    oldvalueGyroX = (oldvalueGyroX + newvalueGyroX)/2;
    oldvalueGyroY = (oldvalueGyroY + newvalueGyroY)/2;
    oldvalueGyroZ = (oldvalueGyroZ + newvalueGyroZ)/2;
    oldvalueTemp = (oldvalueTemp + newvalueTemp)/2;
  }
  tactValues(oldvalueAccelX,oldvalueAccelY, myValues);
  for (int i=0; i<=7; i++) {
    SoftPWMSet(vpins[i], scaler(myValues[i]));
  }
  Serial.print("tact 0= ");
  Serial.println(scaler(myValues[0]));
  Serial.print("tact 1= ");
  Serial.println(scaler(myValues[1]));
  Serial.print("tact 2= ");
  Serial.println(scaler(myValues[2]));
  Serial.print("tact 3= ");
  Serial.println(scaler(myValues[3]));
  Serial.print("tact 4= ");
  Serial.println(scaler(myValues[4]));
  Serial.print("tact 5= ");
  Serial.println(scaler(myValues[5]));
  Serial.print("tact 6= ");
  Serial.println(scaler(myValues[6]));
  Serial.print("tact 7= ");
  Serial.println(scaler(myValues[7]));
}
