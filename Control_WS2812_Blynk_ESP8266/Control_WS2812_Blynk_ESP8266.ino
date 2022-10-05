/*******************************************************
 * Author: Atul Dubey
 * Date: 05/10/2022
 * Description:
 *    This code will allow you to control the RGB Strips 
 *    using the Blynk app
 *******************************************************/

//Libraries required for Blynk
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
 #include <Adafruit_NeoPixel.h>

 

const char* ssid = "WIFI SSID";
const char* pass = "WIFI PASSWORD";
const char* auth = "BLYNK AUTH CODE";

uint32_t red_colour;                                                                  // Variables for storing the colour values.
uint32_t green_colour;
uint32_t blue_colour;
uint32_t yellow_colour;
uint32_t magenta_colour;
uint32_t cyan_colour;
uint32_t white_colour;
uint32_t off_colour;
int randcolor;                                                                        // Variables for storing random colour number and random delay
long randdelay;
String received;
int value =0;
int temp =0;

#define PIN 12
 Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, PIN, NEO_GRB + NEO_KHZ800); 

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
    strip.begin();                                                                      // To intialize the Strip.
  strip.show();                                                                       // Function to push colour on to the pixels. Here it is to turn off all pixels initially.

  strip.setBrightness(255);                                                           // Function to set the brightness of the Pixels.
 /* strip.Color returns an integer according to a spcific color set in its arguements */
  red_colour = strip.Color(255, 0, 0);                                                // 255,0,0 for RED
  green_colour = strip.Color(0, 255, 0);
  blue_colour = strip.Color(0, 0, 255);
  yellow_colour = strip.Color(255, 255, 0 );
  magenta_colour = strip.Color(255, 0, 255);
  cyan_colour = strip.Color(0, 255, 255);
  white_colour = strip.Color(255, 255, 255);
  off_colour = strip.Color(0, 0, 0);

  off();                               
}

void loop() {
  Blynk.run();
}

/* Function  
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

/* Function to display Red colour */
void red()
{
  for (uint16_t i = 0; i < strip.numPixels(); i++) {                                  // Starting from the first pixel(RGB LED), loop all 10 LEDs to set the colour.
    strip.setPixelColor(i, red_colour );                                              // Set the pixel colour, (Pixel_number, Pixel_colour).
  }
  strip.show();                                                                       // Display the set colours on the pixels.
}

/* Function to display Green colour */
void green()
{
  for (uint16_t i = 0; i < strip.numPixels(); i++) {                                  // Starting from the first pixel(RGB LED), loop all 10 LEDs to set the colour.
    strip.setPixelColor(i, green_colour );                                            // Set the pixel colour, (Pixel_number, Pixel_colour).
  }
  strip.show();                                                                       // Display the set colours on the pixels.
}

/* Function to display Blue colour */
void blue()
{
  for (uint16_t i = 0; i < strip.numPixels(); i++) {                                  // Starting from the first pixel(RGB LED), loop all 10 LEDs to set the colour.
    strip.setPixelColor(i, blue_colour );                                             // Set the pixel colour, (Pixel_number, Pixel_colour).
  }
  strip.show();                                                                       // Display the set colours on the pixels.
}

/* Function to display Yellow colour */
void yellow()
{
  for (uint16_t i = 0; i < strip.numPixels(); i++) {                                  // Starting from the first pixel(RGB LED), loop all 10 LEDs to set the colour.
    strip.setPixelColor(i, yellow_colour );                                           // Set the pixel colour, (Pixel_number, Pixel_colour).
  }
  strip.show();                                                                       // Display the set colours on the pixels.
}

/* Function to display Yellow colour */
void magenta()
{
  for (uint16_t i = 0; i < strip.numPixels(); i++) {                                  // Starting from the first pixel(RGB LED), loop all 10 LEDs to set the colour.
    strip.setPixelColor(i, magenta_colour );                                          // Set the pixel colour, (Pixel_number, Pixel_colour).
  }
  strip.show();                                                                       // Display the set colours on the pixels.
}

/* Function to display Cyan colour */
void cyan()
{
  for (uint16_t i = 0; i < strip.numPixels(); i++) {                                  // Starting from the first pixel(RGB LED), loop all 10 LEDs to set the colour.
    strip.setPixelColor(i, cyan_colour );                                             // Set the pixel colour, (Pixel_number, Pixel_colour).
  }
  strip.show();                                                                       // Display the set colours on the pixels.
}

/* Function to display White colour */
void white()
{
  for (uint16_t i = 0; i < strip.numPixels(); i++) {                                  // Starting from the first pixel(RGB LED), loop all 10 LEDs to set the colour.
    strip.setPixelColor(i, white_colour );                                            // Set the pixel colour, (Pixel_number, Pixel_colour).
  }
  strip.show();                                                                       // Display the set colours on the pixels.
}

/* Function to turn OFF all LEDs */
void off()
{
  for (uint16_t i = 0; i < strip.numPixels(); i++) {                                  // Starting from the first pixel(RGB LED), loop all 10 LEDs to set the colour.
    strip.setPixelColor(i, off_colour );                                              // Set the pixel colour, (Pixel_number, Pixel_colour).
  }
  strip.show();                                                                       // Display the set colours on the pixels.
}

/* Function to display Dissolve pattern */
/*void dissolve()
{
  while (received == "dissolve")                                           // Continue displaying the dissolve pattern until the argument of incoming request changes.
  {
    uint16_t i, j;                                                                    // Declare variable for parsing the RGB LEDs.
    for (j = 0; j < 256; j++)                                                         // Loop of generating 256 different colours for a smooth transition.
    {
      for (i = 0; i < 10; i++)                                                        // Loop to set colours to each of the 10 pixels.
      {
        strip.setPixelColor(i, Wheel((i + j) & 255));
      }
      strip.show();                                                                   // Display the set colours on the Pixels.
      delay(20);
      Blynk.run();                                                          // Check for any incoming requests.
      if (received != "dissolve")                                          // In case of any new request apart from "dissolve", break out from the loop.
      {
        break;
      }
    }
    Blynk.run();                                                            // Check for any incoming requests.
  }
}

/* Function to display Rainbow pattern */
/*void rainbow()
{
  while (received == "rainbow")                                            // Continue displaying the rainbow pattern until the argument of incoming request changes.
  {
    uint16_t i, j;                                                                    // Declare variable for parsing the RGB LEDs.
    for (j = 0; j < 256 * 5; j++)
    {
      for (i = 0; i < strip.numPixels(); i++)                                         // Loop to setup colour for each pixel
      {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }
      strip.show();                                                                   // Display the set colours on the Pixels.
      delay(20);
      Blynk.run();                                                          // Check for any incoming requests.
      if (received != "rainbow")                                           // In case of any new request apart from "rainbow", break out from the loop.
      {
        break;
      }
    }
    Blynk.run();                                                            // Check for any incoming requests.
  }
}

/* Function to display Dancing Lights pattern 
void dancingLights()
{
  while (received == "dance")                                              // Continue displaying the dancing lights pattern until the argument of incoming request changes.
  {
    for (int j = 0; j < 256; j++)
    {
      for (int q = 0; q < 3; q++)
      {
        for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {                      
          strip.setPixelColor(i + q, Wheel( (i + j) % 255));                          // Set every third pixel ON.
        }
        strip.show();                                                                 // Display the set colours onthe pixels.
        delay(50);
        for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
          strip.setPixelColor(i + q, 0);                                              // Set every third pixel OFF.
        }
        Blynk.run();                                                        // Check for any incoming requests.
        if (received != "dance")                                           // In case of any new request apart from "dance", break out from the loop.
        {
          break;
        }
      }
      Blynk.run();                                                          // Check for any incoming requests.
      if (received != "dance")                                             // In case of any new request apart from "dance", break out from the loop.
      {
        break;
      }
    }
    Blynk.run();                                                            // Check for any incoming requests.
  }
}

/* Function to display Random Colours pattern 
 *  Random Lights turn ON for an interval of 1 second.
 
void randomColour()
{
  while (received == "random") {                                           // Continue displaying random colours until the argument of incoming request changes.
    randcolor = random(0, 7);                                                         // Generate a random number between 0-6 to display a random colour.
    if (randcolor == 0) {                                                             // If generated random number is 0, display Red colour for 1 second.
      red();
      delay(1000);
    }
    else if (randcolor == 1) {                                                        // If generated random number is 1, display Green colour for 1 second.
      green();
      delay(1000);
    }
    else if (randcolor == 2) {                                                        // If generated random number is 2, display Blue colour for 1 second.
      blue();
      delay(1000);
    }
    else if (randcolor == 3) {                                                        // If generated random number is 3, display Magenta colour for 1 second.
      magenta();
      delay(1000);
    }
    else if (randcolor == 4) {                                                        // If generated random number is 4, display Cyan colour for 1 second.
      cyan();
      delay(1000);
    }
    else if (randcolor == 5) {                                                        // If generated random number is 5, display White colour for 1 second.
      white();  
      delay(1000);
    }
    else if (randcolor == 6) {                                                        // If generated random number is 6, display Yellow colour for 1 second.
      yellow();
      delay(1000);
    }
    Blynk.run();                                                            // Check for any incoming requests for other patterns or colours.
  }
  Blynk.run();                                                              // Check for any incoming requests for other patterns or colours.
}

/* Function to display Party Lights pattern
 *  Display random lights with random ON times.
 
void party()
{
  while (received == "party") {                                            // Continue displaying Party lights pattern until the argument of incoming request changes.
    randcolor = random(0, 7);                                                         // Generate a random number between 0 and 6 to display a random colour.
    randdelay = random(20, 100);                                                      // Generate a random number between 20 and 99 to generate a random turn ON time.
    if (randcolor == 0) {                                                             // If generated random number is 0, display Red colour.
      red();
      delay(randdelay);                                                               // Display the colour for a random time which is generated above.
    }
    else if (randcolor == 1) {                                                        // If generated random number is 1, display Green colour.
      green();
      delay(randdelay);                                                               // Display the colour for a random time which is generated above.
    }
    else if (randcolor == 2) {                                                        // If generated random number is 2, display Blue colour.
      blue();
      delay(randdelay);                                                               // Display the colour for a random time which is generated above.
    }
    else if (randcolor == 3) {                                                        // If generated random number is 3, display Magenta colour.
      magenta();
      delay(randdelay);                                                               // Display the colour for a random time which is generated above.
    }
    else if (randcolor == 4) {                                                        // If generated random number is 4, display Cyan colour.
      cyan();
      delay(randdelay);                                                               // Display the colour for a random time which is generated above.
    }
    else if (randcolor == 5) {                                                        // If generated random number is 5, display White colour.
      white();
      delay(randdelay);                                                               // Display the colour for a random time which is generated above.
    }
    else if (randcolor == 6) {                                                        // If generated random number is 6, display Yellow colour.
      yellow();
      delay(randdelay);                                                               // Display the colour for a random time which is generated above.
    }
    Blynk.run();                                                            // Check for any incoming requests for other patterns or colours.
  }
  Blynk.run();                                                              // Check for any incoming requests for other patterns or colours.
}
*/
BLYNK_WRITE(V1){
   value = param.asInt();
  Serial.println(value);
  strip.setBrightness(value);  
  strip.show();
}
BLYNK_WRITE(V2){
  received = param.asStr();
  Serial.println(received);
  light();
}

BLYNK_WRITE(V3){
  received = param.asStr();
  Serial.println(received);
  
  if(received == "on")
  {
    temp++;
    if (temp%2 != 0)
  {
    white();
  }
   else
  off();
    
    }
  
  else if(received == "increase")
  {
    value +=10;
    strip.setBrightness(value);
    strip.show();
  }
   else if(received == "decrease")
  {
    value -=10;
    strip.setBrightness(value);
    strip.show();
  }
}
void light(){
    if (received == "red" || received == "Red") {                                                 // If the argument of incoming request is Red, turn on Red colour.
    red();                                                                            // Function call to turn ON Red light.
  }

  else if (received == "green" || received == "Green") {                                                 // If the argument of incoming request is Red, turn on Red colour.
   green();                                                                            // Function call to turn ON Red light.
  }
  else if (received == "blue" || received == "Blue") {                                                 // If the argument of incoming request is Red, turn on Red colour.
    blue();                                                                            // Function call to turn ON Red light.
  }
  else if (received == "yellow" || received == "Yellow") {                                                 // If the argument of incoming request is Red, turn on Red colour.
    yellow();                                                                            // Function call to turn ON Red light.
  }
  else if (received == "magenta" || received == "Magenta") {                                                 // If the argument of incoming request is Red, turn on Red colour.
    magenta();                                                                            // Function call to turn ON Red light.
  }
  else if (received == "cyan" || received == "Cyan") {                                                 // If the argument of incoming request is Red, turn on Red colour.
    cyan();                                                                            // Function call to turn ON Red light.
  }
  else if (received == "white" || received == "White") {                                                 // If the argument of incoming request is Red, turn on Red colour.
    cyan();                                                                            // Function call to turn ON Red light.
  }
 /*  if (received == "dissolve") {                                                 // If the argument of incoming request is Red, turn on Red colour.
    dissolve();                                                                            // Function call to turn ON Red light.
  }
   if (received == "rainbow") {                                                 // If the argument of incoming request is Red, turn on Red colour.
    rainbow();                                                                            // Function call to turn ON Red light.
  }
   if (received == "dance") {                                                 // If the argument of incoming request is Red, turn on Red colour.
    dancingLights();                                                                            // Function call to turn ON Red light.
  }
   if (received == "random") {                                                 // If the argument of incoming request is Red, turn on Red colour.
    randomColour();                                                                            // Function call to turn ON Red light.
  }
   if (received == "party") {                                                 // If the argument of incoming request is Red, turn on Red colour.
    party();                                                                            // Function call to turn ON Red light.
  }*/
  else if (received == "off") {                                                 // If the argument of incoming request is Red, turn on Red colour.
    off();                                                                            // Function call to turn ON Red light.
  }
  
}
