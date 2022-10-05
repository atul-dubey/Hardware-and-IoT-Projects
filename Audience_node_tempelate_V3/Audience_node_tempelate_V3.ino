/**********************************************************
   Author: Atul Dubey
   Date: 05/10/2022
   Description:
      Tempelate for Audience Demo
      Change the following parts in code to customize:
        -Publish topic
        -Cient name
 *********************************************************/

#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define button 4
#define led 13
// Update these with values suitable for your network.

const char* ssid = "WIFI SSID";
const char* password = "WIFI PASSWORD";
const char* mqtt_server = "MQTT BROKER IP ADDRESS";

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
int count = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, 12, NEO_GRB + NEO_KHZ800);

WiFiClient espClient;
PubSubClient client(espClient);

String value = "";

void setup() {
  strip.begin();
  strip.show();
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
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char received[100];
  for (int i = 0; i < 3; i++) {
    received[i] = (char)payload[i];
  }

  Serial.println();
  value = String(received);
  Serial.println(value);
  String Prev_value = value;
  light();
  Prev_value = value;
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("node4")) {
      Serial.println("connected");
      client.subscribe("mood");
      client.subscribe("node4");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (digitalRead(button) == HIGH)
  {

    String msg = "1";
    char Y[10];
    msg.toCharArray(Y, 10);
    client.publish("vote", Y);
    Serial.println(Y);
    green();
    delay(200);
    off();
    delay(300);
    //count++;
  }
  String msg = String(analogRead(A0));
  char Y[10];
  msg.toCharArray(Y, 10);
  client.publish("ldr", Y);
  delay(100);
}

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

void smile()
{
  for (uint16_t i = 0; i < 2; i++) {                                  // Starting from the first pixel(RGB LED), loop all 10 LEDs to set the colour.
    strip.setPixelColor(i, green_colour );                                            // Set the pixel colour, (Pixel_number, Pixel_colour).
  }
  for (uint16_t i = 3; i < 4; i++) {                                  // Starting from the first pixel(RGB LED), loop all 10 LEDs to set the colour.
    strip.setPixelColor(i, green_colour );                                            // Set the pixel colour, (Pixel_number, Pixel_colour).
  }
  for (uint16_t i = 6; i < 7; i++) {                                  // Starting from the first pixel(RGB LED), loop all 10 LEDs to set the colour.
    strip.setPixelColor(i, green_colour );                                            // Set the pixel colour, (Pixel_number, Pixel_colour).
  }
  for (uint16_t i = 8; i < 10; i++) {                                  // Starting from the first pixel(RGB LED), loop all 10 LEDs to set the colour.
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
  while (value == "dis")                                           // Continue displaying the dissolve pattern until the argument of incoming request changes.
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
      client.loop();                                                         // Check for any incoming requests.
      if (value != "dis")                                          // In case of any new request apart from "dissolve", break out from the loop.
      {
        break;
      }
    }
    client.loop();                                                           // Check for any incoming requests.
  }
}

/* Function to display Rainbow pattern */
void rainbow()
{
  while (value == "rai")                                            // Continue displaying the rainbow pattern until the argument of incoming request changes.
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
      client.loop();                                                          // Check for any incoming requests.
      if (value != "rai")                                           // In case of any new request apart from "rainbow", break out from the loop.
      {
        break;
      }
    }
    client.loop();                                                            // Check for any incoming requests.
  }
}

/* Function to display Dancing Lights pattern */
void dancingLights()
{
  while (value == "dan")                                              // Continue displaying the dancing lights pattern until the argument of incoming request changes.
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
        client.loop();                                                        // Check for any incoming requests.
        if (value != "dan")                                           // In case of any new request apart from "dance", break out from the loop.
        {
          break;
        }
      }
      client.loop();                                                          // Check for any incoming requests.
      if (value != "dan")                                             // In case of any new request apart from "dance", break out from the loop.
      {
        break;
      }
    }
    client.loop();                                                            // Check for any incoming requests.
  }
}

/* Function to display Random Colours pattern
    Random Lights turn ON for an interval of 1 second.
*/
void randomColour()
{
  while (value == "ran") {                                           // Continue displaying random colours until the argument of incoming request changes.
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
    client.loop();                                                            // Check for any incoming requests for other patterns or colours.
  }
  client.loop();                                                              // Check for any incoming requests for other patterns or colours.
}

/* Function to display Party Lights pattern
    Display random lights with random ON times.
*/
void party()
{
  while (value == "par") {                                            // Continue displaying Party lights pattern until the argument of incoming request changes.
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
    client.loop();                                                            // Check for any incoming requests for other patterns or colours.
  }
  client.loop();                                                              // Check for any incoming requests for other patterns or colours.
}

void light() {
  if (value == "red") {
    red();
  }
  else if (value == "gre") {
    green();
  }
  else if (value == "blu") {
    blue();
  }
  else if (value == "yel") {
    yellow();
  }
  else if (value == "mag") {
    magenta();
  }
  else if (value == "cya") {
    cyan();
  }
  else if (value == "whi") {
    white();
  }
  else if (value == "dis") {
    dissolve();
  }
  else if (value == "rai") {
    rainbow();
  }
  else if (value == "dan") {
    dancingLights();
  }
  else if (value == "ran") {
    randomColour();
  }
  else if (value == "par") {
    party();
  }
  else if (value == "off") {
    off();
  }
  else {
  }
  delay(10);
}
