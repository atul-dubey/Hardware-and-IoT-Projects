
/* Include libraries */
#include <Adafruit_NeoPixel.h>                                                        // Library for working with RGB LEDs
#include <ESP8266WiFi.h>                                                              // Library for connecting HDK to the WiFi network.
#include <ESP8266WebServer.h>                                                         // Library that helps the HDK to act as a web server.

/* Define Macros */
const char* ssid = "WIFI SSID";                                                      // WiFi credentials of the network.
const char* password = "WIFI PASSWORD";

/* Define pins */
#define PIN 12

/* Declare instances */
ESP8266WebServer server(80);                                                          // Create a Web Server instance with HTTP port number 80.

/* Define global variables & constants */
uint32_t color0;
uint32_t color1; 
uint32_t color2;
uint32_t color3;
uint32_t color4;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, PIN, NEO_GRB + NEO_KHZ800);           // Defining RGB LED strip.

/* Function to display different patterns and colors */
void forward()
{
 for(int i=0;i<10;i++){
    strip.setPixelColor(i, color1); // set the color of strip
    strip.show(); // This sends the updated pixel color to the hardware.
  }
  delay(50);
  for(int i=0;i<10;i++){
    strip.setPixelColor(i, color0); 
    strip.show(); // This sends the updated pixel color to the hardware.
  }
  delay(50);
}

void backward()
{
 for(int i=0;i<10;i++){
    strip.setPixelColor(i, color2); 
    strip.show(); // This sends the updated pixel color to the hardware.
  }  
  delay(50);
  for(int i=0;i<10;i++){
    strip.setPixelColor(i, color0); 
    strip.show(); // This sends the updated pixel color to the hardware.
  }  
  delay(50);
}

void left()
{
  for(int i=0;i<10;i++){
    strip.setPixelColor(i, color3); 
    strip.show(); // This sends the updated pixel color to the hardware.
  } 
   delay(50);
  for(int i=0;i<10;i++){
    strip.setPixelColor(i, color0); 
    strip.show(); // This sends the updated pixel color to the hardware.
  }
  delay(50);
}
void right()
{
  for(int i=10;i>0;i--){
    strip.setPixelColor(i, color4); 
    strip.show(); // This sends the updated pixel color to the hardware.
  } 
  delay(50);
  for(int i=0;i<10;i++){
    strip.setPixelColor(i, color0); 
    strip.show(); // This sends the updated pixel color to the hardware.
  }
  delay(50);
}


/* Function to handle undefined requests */
void handleNotFound() {
  server.send(200, "text/plain", "Not Found");                                        // Send a message "Not Found" in case of undefined requests.
}

/* Function to handle the "/light" request and turn ON the appropriate light or pattern */
void movement()
{
  if (server.arg("state") == "forward") {                                           // If the argument of incoming request isforward, then display respective pattern.
    Serial.println(F("Request is for forward "));                                  // Indication in Serial Monitor.
    server.send(200, "text/plain", "move forward");                               // Message to the Client that the light is ON.
    forward();                                                                   // Function call to show the pattern.
  }
  else if (server.arg("state") == "backward") {
    Serial.println(F("Request is for backward "));
    server.send(200, "text/plain", "move backward");
    backward();
  }
   else if (server.arg("state") == "left") {
    Serial.println(F("Request is for left "));
    server.send(200, "text/plain", "move left");
    left();
  }
   else if (server.arg("state") == "right") {
    Serial.println(F("Request is for right "));
    server.send(200, "text/plain", "move right");
    right();
  }
}

/* Function to connect to a WiFi network */
void setup_wifi() {
  delay(10);
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
  Serial.print(WiFi.localIP());                                                       // Display the IP address on the Serial Monitor to access the Lights.
  Serial.println("/");
}

/* Setup code - Runs once */
void setup() {
  Serial.begin(115200);
  strip.begin();                                                                      // To intialize the Strip.
  strip.show();                                                                       // Function to push colour on to the pixels. Here it is to turn off all pixels initially.

  strip.setBrightness(50);                                                           // Function to set the brightness of the Pixels.

  /* strip.Color returns an integer according to a spcific color set in its arguements */
  color0 = strip.Color(0, 0, 0);
  color1 = strip.Color(255, 0, 0);
  color2 = strip.Color(0, 255, 0);
  color3 = strip.Color(0, 0, 255);
  color4 = strip.Color(255, 0, 255);
  server.begin();                                                                     // Start the Web Server on the HDK.
  Serial.println("Server started");

  setup_wifi();
  
  server.on("/movement", movement);                                                    // Method to execute/process client requests with the URL "/light"
  server.onNotFound(handleNotFound);                                                  // Method that executes for undefined requests.
}

void loop() {
  server.handleClient();                                                              // Method to listen to client requests
}
