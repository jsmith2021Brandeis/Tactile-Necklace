/*Jonathan Lam
   https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
*/
// defines pins numbers
const int trigPin = 10;
const int echoPin = 11;
// defines variables
long duration;
int distance;
int tactArray[4];
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.print ("tact 1: ");
  Serial.println(tactArray[0]);
  Serial.print ("tact 2: ");
  Serial.println(tactArray[1]);
  Serial.print ("tact 3: ");
  Serial.println(tactArray[2]);
  Serial.print ("tact 4: ");
  Serial.println(tactArray[3]);
  tactvalues4u(distance, tactArray);
}
void clearTacts(int*  tactArray) {
  for (int i = 0; i < 4; i++) {
    tactArray[i] = 0;
  }
}
void tactvalues4u(int distance, int* tactArray) {
  clearTacts(tactArray);
  if (75 <= distance && distance <= 100) {
    tactArray[0] = ((9 * (100 - distance)) + 30);
  }
  if (50 <= distance && distance <= 75) {
    tactArray[0]=255;
    tactArray[1] = (9 * (75 - distance) + 30);
    tactArray[2] = (9 * (75 - distance) + 30);
  }
  if (25 <= distance && distance <= 50) {
    for (int i = 0; i < 3; i++) {
    tactArray[i] = 255;
    }
    tactArray[3] = (9 * (50 - distance) + 30);
  }
  if (distance <= 25) {
    for (int i = 0; i < 4; i++) {
    tactArray[i] = 255;
    }
  }
}