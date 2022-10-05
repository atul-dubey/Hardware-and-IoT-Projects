/********************************************************
   Author: Atul Dubey
   Date: 05/10/2022
   Description:
      Code for building device to control PC UI
 ********************************************************/

/* Include libraries */
#include <ESP8266WiFi.h>                                                              // Library for connecting HDK to the WiFi network.
#include <ESP8266WebServer.h>                                                         // Library that helps the HDK to act as a web server.

/* Define Macros */
const char* ssid = "WIFI SSID";                                                      // WiFi credentials of the network.
const char* password = "WIFI PASSWORD";

IPAddress ip(192, 168, 2, 150);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);

/* Declare instances */
ESP8266WebServer server(80);                                                          // Create a Web Server instance with HTTP port number 80.

/* Function to handle undefined requests */
void handleNotFound() {
  server.send(200, "text/plain", "Not Found");                                        // Send a message "Not Found" in case of undefined requests.
}

/* Function to handle the "/light" request and turn ON the appropriate light or pattern */
void movement()
{
  if (server.arg("state") == "forward") {  // If the argument of incoming request isforward, then display respective pattern.
    Serial.println(F("1"));                                  // Indication in Serial Monitor.
    server.send(200, "text/plain", "move forward");                               // Message to the Client that the light is ON.

  }
  else if (server.arg("state") == "backward") {
    Serial.println(F("2"));
    server.send(200, "text/plain", "move backward");

  }
  else if (server.arg("state") == "left") {
    Serial.println(F("3"));
    server.send(200, "text/plain", "move left");

  }
  else if (server.arg("state") == "right") {
    Serial.println(F("4"));
    server.send(200, "text/plain", "move right");

  }
  else if (server.arg("state") == "stop") {
    Serial.println(F("5"));
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

  Serial.begin(115200);                                                               // To intialize the Serial communication.
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
