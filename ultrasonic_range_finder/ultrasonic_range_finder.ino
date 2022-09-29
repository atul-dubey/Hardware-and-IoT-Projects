/* Author: Atul Dubey
    Date: 06/03/2020
    Project: ultrasonic range finder
*/

const int pingpin = 7;        // Pin to send the pulse and receive the Echo
int duration;                 // Variable to store the time taken by pulse
int inches, cm;                   // Variable to store the calculated distance in inches

void setup()
{
  Serial.begin(9600);          //serial communication initialization

}

void loop()
{
  // Sending the pulse
  pinMode(pingpin, OUTPUT);
  digitalWrite(pingpin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingpin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingpin, LOW);

  // Receiving the echo
  pinMode(pingpin, INPUT);
  duration = pulseIn(pingpin, HIGH); //Calculating the duration

  inches = microsecondsToInches(duration); //Converting the duration into inches
  cm = microsecondsToCentimeters(duration); //From inches to cm

  //printing the distance on the serial monitor
  Serial.print(inches);
  Serial.print("in,");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(100);            // wait for 100ms
}

// Function to convert ms to inches
long microsecondsToInches()
{
  return microseconds / 74 / 2;
}

// Function to convert ms to cm
long microsecondsToCentimeters()
{
  return microseconds / 29 / 2;
}
