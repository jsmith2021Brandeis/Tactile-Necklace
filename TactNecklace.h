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
#include<Arduino.h>
#ifndef TactNecklace_h
#define TactNecklace_h
//create macro to get length of array
#define ARRAY_SIZE(arr)((sizeof(arr))/sizeof(arr[0]))
class TactNecklace{
	private:
		float oldvalue;//used later in averaging formula
		float newvalue;//used later in averaging formula
		float oldvalueAccelX;//used for averaging accelerometer values in the x-axis (oldvalue and new value averaged to get an approximate accelerometer value) 
		float oldvalueAccelY;//... y-axis
		float oldvalueAccelZ;//... z-axis
		float oldvalueGyroX;//used for averaging gyroscope values in the x-axis (oldvalue and new value averaged to get an approximate gyroscope value) 
		float oldvalueGyroY;//... y-axis
		float oldvalueGyroZ;//... z-axis
		float newvalueAccelX;//used for averaging accelerometer values in the x-axis (oldvalue and new value averaged to get an approximate accelerometer value) 
		float newvalueAccelY;//... y-axis
		float newvalueAccelZ;//... z-axis
		float newvalueGyroX;//used for averaging gyroscope values in the x-axis (oldvalue and new value averaged to get an approximate gyroscope value) 
		float newvalueGyroY;//... y-axis
		float newvalueGyroZ;//... z-axis
		float zerox;//balance point for where the accelerometer is set in the x-axis (where no vibrators are on)
		float zeroy;//... y-axis
		float avgx;//acquire from averaging the old and new values 
		float avgy;//acquire from averaging the old and new values 
		int vpins[8]={2,3,4,5,6,9,10,11};//establishes what ports from the Arduino to use
		float accx=0; //accelerometer data is non integer values, with decimals
		float accy=0; //data inputted from sensor has x and y axes
		const int maxacc=16500;//Largest input value from accelerometer
		const int vMax=255;//max strength of vibrator is 255, exceeding 255 with overflow, the byte will read the 8 rightmost bits
		int numPins;
		int myValues[8]; //defining a new array

		const int MPU6050_addr=0x68;
		int16_t AccX,AccY,AccZ,GyroX,GyroY,GyroZ;
		void clearTacts(int*  tactArray) ;
		void tactValues(float accx, float accy, int* tactArray);
		void getValues(); //get acc values
	public:
		void begin (int vpins[], int numPins); //initializes the function, this can't be in the constructor
		void circle (); //turns on each tactor individually then turns that same tactor off so that the vibrators turn on in a circle
		void pulse (); //turns all vibrators on and then off to simulate a pulsation
		int scaler(float input); //want your min to be 34 because it is at the point where it first starts to be noticeable, max is lower than 255 because that is the maximum vibration strength we deemed necessary
		//  needed to lower vibration strength even lower because voltage was increased from 5V to 7.4, so the new numbers are 69% of original numbers
		//  can adjust the scaler, but not required
		void sendVibration (); //acquires acceleration values and sends it to the vibrator pins which determines the strength of the vibration
};
#endif

 