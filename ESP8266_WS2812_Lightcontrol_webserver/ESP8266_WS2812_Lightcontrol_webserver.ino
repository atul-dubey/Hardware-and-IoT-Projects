/**************************************************************************
   Author: Atul Dubey
   Date: 29 september 2022
   Description:
           This code allows you to control a WS2812 strip from a web page.
           There are multiple patterns to play around with the RGB strip:
            - Basic colors: Red, Green, Blue, White, yellow, magenta, Cyan
            - Dissolve
            - Rainblow
            - Dancing Lights
            - Random
            - Party
    Requirements:
      Hardware: ESP8266/NodeMCU board, WS2812 RGB Strip
      Libraries: Adafruit Neopixel, ESP8266WiFi, ESP8266WebServer
 **************************************************************************/

#include <Adafruit_NeoPixel.h>                                                        // Library for working with RGB LEDs
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

#define PIN 12

//Uncommment below lines if you want to setup a static IP
//IPAddress ip(192,168,2,118);
//IPAddress gateway(192,168,2,1);
//IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

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
Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, PIN, NEO_GRB + NEO_KHZ800);           // Defining RGB LED strip.

/* Function  */
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
void dissolve()
{
  while (server.arg("value") == "dissolve")                                           // Continue displaying the dissolve pattern until the argument of incoming request changes.
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
      server.handleClient();                                                          // Check for any incoming requests.
      if (server.arg("value") != "dissolve")                                          // In case of any new request apart from "dissolve", break out from the loop.
      {
        break;
      }
    }
    server.handleClient();                                                            // Check for any incoming requests.
  }
}

/* Function to display Rainbow pattern */
void rainbow()
{
  while (server.arg("value") == "rainbow")                                            // Continue displaying the rainbow pattern until the argument of incoming request changes.
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
      server.handleClient();                                                          // Check for any incoming requests.
      if (server.arg("value") != "rainbow")                                           // In case of any new request apart from "rainbow", break out from the loop.
      {
        break;
      }
    }
    server.handleClient();                                                            // Check for any incoming requests.
  }
}

/* Function to display Dancing Lights pattern */
void dancingLights()
{
  while (server.arg("value") == "dance")                                              // Continue displaying the dancing lights pattern until the argument of incoming request changes.
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
        server.handleClient();                                                        // Check for any incoming requests.
        if (server.arg("value") != "dance")                                           // In case of any new request apart from "dance", break out from the loop.
        {
          break;
        }
      }
      server.handleClient();                                                          // Check for any incoming requests.
      if (server.arg("value") != "dance")                                             // In case of any new request apart from "dance", break out from the loop.
      {
        break;
      }
    }
    server.handleClient();                                                            // Check for any incoming requests.
  }
}

/* Function to display Random Colours pattern
    Random Lights turn ON for an interval of 1 second.
*/
void randomColour()
{
  while (server.arg("value") == "random") {                                           // Continue displaying random colours until the argument of incoming request changes.
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
    server.handleClient();                                                            // Check for any incoming requests for other patterns or colours.
  }
  server.handleClient();                                                              // Check for any incoming requests for other patterns or colours.
}

/* Function to display Party Lights pattern
    Display random lights with random ON times.
*/
void party()
{
  while (server.arg("value") == "party") {                                            // Continue displaying Party lights pattern until the argument of incoming request changes.
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
    server.handleClient();                                                            // Check for any incoming requests for other patterns or colours.
  }
  server.handleClient();                                                              // Check for any incoming requests for other patterns or colours.
}

/* Function to handle undefined requests */
void handleNotFound() {
  server.send(200, "text/plain", "Not Found");                                        // Send a message "Not Found" in case of undefined requests.
}

/* Function to handle the "/light" request and turn ON the appropriate light or pattern */
void light()
{
  if (server.arg("value") == "red") {                                                 // If the argument of incoming request is Red, turn on Red colour.
    Serial.println(F("Request is for Red colour "));                                  // Indication in Serial Monitor.
    server.send(200, "text/plain", "Red Light turned ON");                            // Message to the Client that the light is ON.
    red();                                                                            // Function call to turn ON Red light.
  }
  else if (server.arg("value") == "green") {
    Serial.println(F("Request is for Green colour "));
    server.send(200, "text/plain", "Green Light turned ON");
    green();
  }
  else if (server.arg("value") == "blue") {
    Serial.println(F("Request is for Blue colour "));
    server.send(200, "text/plain", "Blue Light turned ON");
    blue();
  }
  else if (server.arg("value") == "yellow") {
    Serial.println(F("Request is for Yellow colour "));
    server.send(200, "text/plain", "Yellow Light turned ON");
    yellow();
  }
  else if (server.arg("value") == "magenta") {
    Serial.println(F("Request is for Magenta colour "));
    server.send(200, "text/plain", "Magenta Light turned ON");
    magenta();
  }
  else if (server.arg("value") == "cyan") {
    Serial.println(F("Request is for Cyan colour "));
    server.send(200, "text/plain", "Cyan Light turned ON");
    cyan();
  }
  else if (server.arg("value") == "white") {
    Serial.println(F("Request is for White colour "));
    server.send(200, "text/plain", "White Light turned ON");
    white();
  }
  else if (server.arg("value") == "dissolve") {                                       // If the argument of incoming request is "dissolve", turn on Dissolve pattern.
    Serial.println(F("Request is for Dissolve pattern "));                            // Indication on Serial Monitor.
    dissolve();                                                                       // Function call to start displaying the Dissolve pattern.
    server.send(200, "text/plain", "Dissolve pattern started");                       // Message to the Client that the pattern is started.
  }
  else if (server.arg("value") == "rainbow") {                                        // If the argument of incoming request is "rainbow", turn on Rainbow pattern.
    Serial.println(F("Request is for Rainbow pattern "));                             // Indication on Serial Monitor.
    rainbow();                                                                        // Function call to start displaying the Rainbow pattern.
    server.send(200, "text/plain", "Rainbow pattern started");                        // Message to the Client that the pattern is started.
  }
  else if (server.arg("value") == "dance") {                                          // If the argument of incoming request is "dance", turn on Dancing Lights pattern.
    Serial.println(F("Request is for Dancing Lights pattern "));                      // Indication on Serial Monitor.
    dancingLights();                                                                  // Function call to start displaying the Dancing Lights pattern.
    server.send(200, "text/plain", "Dancing Lights pattern started");                 // Message to the Client that the pattern is started.
  }
  else if (server.arg("value") == "random") {                                         // If the argument of incoming request is "random", turn on Random Lights pattern.
    Serial.println(F("Request is for Random pattern"));                               // Indication on Serial Monitor.
    randomColour();                                                                   // Function call to start displaying the Random Lights pattern.
    server.send(200, "text/plain", "Random lights pattern started");                  // Message to the Client that the pattern is started.
  }
  else if (server.arg("value") == "party") {                                          // If the argument of incoming request is "party", turn on Party Lights pattern.
    Serial.println(F("Request is for Party Lights pattern"));                         // Indication on Serial Monitor.
    party();                                                                          // Function call to start displaying the Party Lights pattern.
    server.send(200, "text/plain", "Party Lights pattern started");                   // Message to the Client that the pattern is started.
  }
  else if (server.arg("value") == "off") {
    Serial.println(F("Request is to turn off the light"));
    server.send(200, "text/plain", "Light turned OFF");
    off();
  }
  else {                                                                              // Code to handle an invalid argument in the request.
    Serial.println(F("Invalid Request!"));
    off();
    server.send(404, "text/plain", "No such colour or pattern is defined!");          // Send an error message to the client.
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);                           // Start connecting to the WiFi network.

  while (WiFi.status() != WL_CONNECTED) {               // Wait until the connection is successful.
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  // WiFi.config(ip, gateway,subnet);
  Serial.println("WiFi connected");
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());                       // Print the IP Address of the device
  Serial.println("/");
}

void lighton() {
  // digitalWrite(5,LOW);
  white();
  Serial.println("on");
  server.send(200, "text/plain", "LIght ON");
}

void lightoff() {
  //digitalWrite(5,HIGH);
  off();
  Serial.println("off");
  server.send(200, "text/plain", "Light OFF");
}

void handleNotFound() {
  server.send(200, "text/plain", "Not Found");                                        // Send a message "Not Found" in case of undefined requests.
}

void setup() {
  pinMode(12, OUTPUT);
  Serial.begin(115200);
  setup_wifi();

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

  off();                                                                              // Initiate all Pixels to 0 --> OFF.


  //  WiFi.config(ip, gateway,subnet);
  server.begin();
  Serial.println("Server started");
  server.on("/light", light);                                                         // Method to execute/process client requests with the URL "/light"
  server.on("/lighton", lighton);                                                         // Method to execute/process client requests with the URL "/lighton"
  server.on("/lightoff", lightoff);                                                        // Method to execute/process client requests with the URL "/lightoff"
  server.onNotFound(handleNotFound);
}

void loop()
{
  server.handleClient();
}
