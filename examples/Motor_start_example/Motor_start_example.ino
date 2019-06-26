/*Jonathan Lam [Date Started=June 26, 2019]
 * This is a simple sketch to test whether the hardware component of a vibrator system is working properly.
*/
int vpins[4] = {3, 5, 6, 9}; //The outputs for the tactor system, the pin numbers.
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Initializes communication with the serial monitor.
  //Sends message to the Arduino that the pins in the array above are set to output information
  for (int i = 0; i < 3; i++) {
    pinMode(vpins[i], OUTPUT);
  }
}
//runs forever
void loop() {
  test();//Tests vibrators to make sure they are wired properly, truning them on for half a second and off for half a second
}
void test() {
  for (int i = 0; i < 4; i++) {
    analogWrite(vpins[i], 255); //Sets the vibrator strength to full for each vibrator, which is 255, for half a second
  }
  delay(500);
  for (int i = 0; i < 4; i++) {
    analogWrite(vpins[i], 0); //Sets the vibrator strength to off for each vibrator, which is 0, for half a second
  }
  delay(500);
}
