/***********************************************
   Author: Atul Dubey
   Date: 28/09/2022
   Description:
    This code received the HTTP requests sent from
    the glove code and controlls the Robot accordingly.
 **********************************************/

/* Include libraries */
#include <Adafruit_NeoPixel.h>                                                        // Library for working with RGB LEDs
#include <ESP8266WiFi.h>                                                              // Library for connecting HDK to the WiFi network.
#include <ESP8266WebServer.h>                                                         // Library that helps the HDK to act as a web server.

/* Define Macros */
const char* ssid = "iBHubs 2";                                                      // WiFi credentials of the network.
const char* password = "";

IPAddress ip(192, 168, 2, 150);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);

/* Define pins */
#define PIN 12
const int m1_1 = 13;
const int m1_2 = 14;
const int m2_1 =  4;
const int m2_2 =  5;

/* Declare instances */
ESP8266WebServer server(80);                                                          // Create a Web Server instance with HTTP port number 80.

/* Define global variables & constants */
uint32_t color0;
uint32_t color1;
uint32_t color2;
uint32_t color3;
uint32_t color4;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, PIN, NEO_GRB + NEO_KHZ800);           // Defining RGB LED strip.

/* Functions to move the robot with led indications */
void forward()
{

  for (uint16_t i = 0; i < 10; i++) {
    strip.setPixelColor(i, color0);
    strip.show(); // This sends the updated pixel color to the hardware.
    //delay(10);
  }
  for (uint16_t j = 4; j < 6; j++) {
    strip.setPixelColor(j, color3); // set the color of strip
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(10);
  }
  /*************Drive Forward*************/
  digitalWrite(m1_1, HIGH);
  digitalWrite(m1_2, LOW);
  digitalWrite(m2_1, HIGH);
  digitalWrite(m2_2, LOW);
}

void backward()
{
  for (uint16_t i = 0; i < 10; i++) {
    strip.setPixelColor(i, color0);
    strip.show();
    delay(10);// This sends the updated pixel color to the hardware.
  }
  for (uint16_t i = 4; i < 6; i++) {
    strip.setPixelColor(i, color2);
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(10);
  }
  /*************Drive Backward*************/
  digitalWrite(m1_1, LOW);
  digitalWrite(m1_2, HIGH);
  digitalWrite(m2_1, LOW);
  digitalWrite(m2_2, HIGH);
}

void right()
{
  for (uint16_t i = 0; i < 10; i++) {
    strip.setPixelColor(i, color0);
    strip.show(); // This sends the updated pixel color to the hardware.
  }

  for (uint16_t i = 7; i < 8; i++) {
    strip.setPixelColor(i, color4);
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(10);
  }
  /*************Drive Left*************/
  digitalWrite(m1_1, LOW);
  digitalWrite(m1_2, HIGH);
  digitalWrite(m2_1, HIGH);
  digitalWrite(m2_2, LOW);
}

void left()
{
  for (uint16_t i = 0; i < 10; i++) {
    strip.setPixelColor(i, color0);
    strip.show(); // This sends the updated pixel color to the hardware.
  }
  for (uint16_t i = 2; i < 3; i++) {
    strip.setPixelColor(i, color4);
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(10);
  }

  /*************Drive right*************/
  digitalWrite(m1_1, HIGH);
  digitalWrite(m1_2, LOW);
  digitalWrite(m2_1, LOW);
  digitalWrite(m2_2, HIGH);
}

void Stop()
{
  for (uint16_t i = 0; i < 10; i++) {
    strip.setPixelColor(i, color0);
    strip.show(); // This sends the updated pixel color to the hardware.
  }
  for (uint16_t i = 4; i < 6; i++) {
    strip.setPixelColor(i, color1);
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(10);
  }
  /*************STOP*************/
  digitalWrite(m1_1, LOW);
  digitalWrite(m1_2, LOW);
  digitalWrite(m2_1, LOW);
  digitalWrite(m2_2, LOW);
}

/* Function to handle undefined requests */
void handleNotFound() {
  server.send(200, "text/plain", "Not Found");                                        // Send a message "Not Found" in case of undefined requests.
}

/* Function to handle the "/light" request and turn ON the appropriate light or pattern */
void movement()
{
  if (server.arg("state") == "forward") {  // If the argument of incoming request isforward, then display respective pattern.
    forward();
    Serial.println(F("Request is for forward "));                                  // Indication in Serial Monitor.

    server.send(200, "text/plain", "move forward");                               // Message to the Client that the light is ON.
    // Function call to show the pattern.
  }
  else if (server.arg("state") == "backward") {
    backward();
    Serial.println(F("Request is for backward "));
    server.send(200, "text/plain", "move backward");

  }
  else if (server.arg("state") == "left") {
    left();
    Serial.println(F("Request is for left "));
    server.send(200, "text/plain", "move left");

  }
  else if (server.arg("state") == "right") {
    right();
    Serial.println(F("Request is for right "));
    server.send(200, "text/plain", "move right");

  }
  else if (server.arg("state") == "stop") {
    Stop();
    Serial.println(F("Request is for stop "));
    server.send(200, "text/plain", "STOP");

  }
}

/* Function to connect to a WiFi network */
void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);                                                         // Start connecting to the WiFi network.

  while (WiFi.status() != WL_CONNECTED) {                                             // Wait until the connection is successful.
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("Use this URL to connect to this server: ");
  Serial.print("http://");
  WiFi.config(ip, gateway, subnet);
  Serial.print(WiFi.localIP());                                                       // Display the IP address on the Serial Monitor to access the Lights.
  Serial.println("/");
}

/* Setup code - Runs once */
void setup() {
  /**********defining motor pins**********/
  pinMode(m1_1, OUTPUT);
  pinMode(m1_2, OUTPUT);
  pinMode(m2_1, OUTPUT);
  pinMode(m2_2, OUTPUT);

  /********Inetializing motor pins***********/
  digitalWrite(m1_1, LOW);
  digitalWrite(m1_2, LOW);
  digitalWrite(m2_1, LOW);
  digitalWrite(m2_2, LOW);

  Serial.begin(115200);                                                               // To intialize the Serial communication.
  strip.begin();                                                                      //To inetialize the strip
  strip.show();                                                                       // To push the color into hardware

  strip.setBrightness(20);                                                           // Function to set the brightness of the Pixels.

  /* strip.Color returns an integer according to a spcific color set in its arguements */
  color0 = strip.Color(0, 0, 0);
  color1 = strip.Color(255, 0, 0);
  color2 = strip.Color(100, 0, 55);
  color3 = strip.Color(0, 0, 255);
  color4 = strip.Color(20, 100, 160);

  setup_wifi();

  server.begin();                                                                     // Start the Web Server on the HDK.
  Serial.println("Server started");
  server.on("/movement", movement);                                                    // Method to execute/process client requests with the URL "/light"
  server.onNotFound(handleNotFound);                                                  // Method that executes for undefined requests.
}

void loop() {
  if (WiFi.status() != WL_CONNECTED)
    Serial.print("Connection lost");
  server.handleClient();                                                              // Method to listen to client requests
}
