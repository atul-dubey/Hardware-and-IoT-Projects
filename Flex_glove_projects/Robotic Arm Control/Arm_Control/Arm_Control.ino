/**************************************************
    Author: Atul Dubey
    Date: 28/09/2022
    Description:
      This code helps in controlling a 4DOF arm over
      Bluetooth.
 **************************************************/

#include <SoftwareSerial.h>
//#include <Adafruit_NeoPixel.h>
#include <Servo.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;// create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;

//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(10, 12, NEO_GRB + NEO_KHZ800);
SoftwareSerial BTserial(4, 5); // RX | TX
// Connect the HC-05 TX to Arduino pin 2 RX.
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.

char c = ' ';
char temp = 'o';

void setup()
{
  // start the serial communication with the host computer
  Serial.begin(115200);
  Serial.println("Arduino with HC-05 is ready");

  // start communication with the HC-05 using 9600
  BTserial.begin(115200);
  Serial.println("BTserial started at 115200");
  //     pixels.begin();

  myservo1.attach(14);
  myservo2.attach(15);
  myservo3.attach(12);
  myservo4.attach(13);
}

void loop()
{

  // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (BTserial.available())
  {
    c = BTserial.read();
    Serial.print(c);
    //delay(10);
  }
  if (c == '1' || temp == '1')
  { myservo1.write(10);
    //delay(1000);
    temp = c;
  }
  if (c == '2' || temp == '2')
  { myservo2.write(10);
    //delay(2000);
    temp = c;
  }
  if (c == '3' || temp == '3')
  { myservo3.write(10);
    //delay(2000);
    temp = c;
  }
  if (c == '4' || temp == '4')
  { myservo4.write(10);
    //delay(2000);
    temp = c;
  }
  if (c == '6')
  {
    myservo1.write(150);
    myservo2.write(150);
    myservo3.write(150);
    myservo4.write(150);
    temp = 'o';
  }
}
