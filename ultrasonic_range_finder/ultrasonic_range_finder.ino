/* Author: Atul Dubey
 *  Date: 06/03/2020
 *  Project: ultrasonic range finder
 */

const int pingpin=7;
int duration;
int inches;

void setup()
{
 //serial communication initialization
 
 Serial.begin(9600);

}

void loop()
{
 pinMode(pingpin,OUTPUT);
 digitalWrite(pingpin,LOW);
 delayMicroseconds(2);
 digitalWrite(pingpin,HIGH);
 delayMicroseconds(5);
 digitalWrite(pingpin,LOW);
 
 pinMode(pingpin,INPUT);
 duration=pulseIn(pingpin,HIGH);
 
 inches=microsecondsToInches(duration);
 cm=microsecondsToCentimeters(duration);
 
 Serial.print(inches);
 Serial.print("in,");
 Serial.print(cm);
 Serial.print("cm");
 Serial.println();
 
 delay(100);
}

long microsecondsToInches()
{
 return microseconds/74/2;
}

long microsecondsToCentimeters()
{
 return microseconds/29/2;
}
