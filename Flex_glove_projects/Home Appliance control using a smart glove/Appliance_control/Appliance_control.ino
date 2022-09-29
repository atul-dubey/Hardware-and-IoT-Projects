/*************************************************
    Author: Atul Dubey
    Date: 28/09/2022
    Description:
    This code will help in controlling different Appliances
    using a Bluetooth connected Flex glove
 *************************************************/

#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(10, 12, NEO_GRB + NEO_KHZ800);
SoftwareSerial BTserial(4, 5); // RX | TX
// Connect the HC-05 TX to Arduino pin 2 RX.
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.

char c = ' ';

void setup()
{
  // start the serial communication with the host computer
  Serial.begin(115200);
  Serial.println("Arduino with HC-05 is ready");

  // start communication with the HC-05 using 9600
  BTserial.begin(115200);
  Serial.println("BTserial started at 115200");
  pixels.begin();
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
  if (c == '1')
  { green();
    //delay(1000);
  }
  if (c == '2')
  { blue();
    //delay(2000);
  }
  if (c == '3')
  { yellow();
    //delay(2000);
  }
  if (c == '4')
  { purple();
    //delay(2000);
  }
  if (c == '5')
  { off();
    // delay(2000);
  }

}
void red()
{
  for (uint16_t i = 0; i < 10; i++) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(150, 0, 0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
  delay(10);
}

void green()
{
  for (uint16_t i = 0; i < 10; i++) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.
  }
  delay(10);
}

void blue()
{
  for (uint16_t i = 0; i < 10; i++) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, 0, 150)); // Moderately bright green color.
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(10);
}

void yellow()
{
  for (uint16_t i = 0; i < 10; i++) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(150, 150, 0)); // Moderately bright green color.
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(10);
}

void purple()
{
  for (uint16_t i = 0; i < 10; i++) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(244, 66, 164)); // Moderately bright green color.
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(10);
}

void off()
{
  for (uint16_t i = 0; i < 10; i++) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Moderately bright green color.
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(10);
}
