/***************************************************
   Author: Atul Dubey
   Date: 28/09/2022
   Description:
      This code creates a client device which will send
      request to another server device to turn on Light on
      Door opening.
   Server code: 
     YOu can use the ESP8266_WS2812_Lightcontrol_webserver
     code to setup a webserver that will request to the request
     sent.  
 ***************************************************/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";

#define server "IP_ADDRESS_OF_SERVER_DEVICE"
#define port 80

//IPAddress ip(192,168,43,118);
// gateway(192,168,43,1);
//IPAddress subnet(255,255,255,0);

int switch_pin = 4;
int flag = 0;
int led = 13;

HTTPClient http;

/* Function to connect to a WiFi network */
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
  //   WiFi.config(ip,gateway,subnet);
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());                       // Print the IP Address of the device
}



void send_http_get() {
  Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();
  if (httpCode) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    // Serial.print("[HTTP] GET... failed, no connection or no HTTP server\n");
  }
}

void setup() {

  pinMode(switch_pin, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
}

void loop() {
  if ((digitalRead(switch_pin) == LOW) && flag == 0)
  {
    delay(20);
    if ((digitalRead(switch_pin) == LOW) && flag == 0)
    {
      digitalWrite(led, HIGH);
      flag = 1;
      http.begin(server, port, "/lighton");
      Serial.println("Door opened");
      send_http_get();
      http.end();
    }
  }
  else if (digitalRead(switch_pin) == HIGH && flag == 1)
  {
    delay(20);
    if (digitalRead(switch_pin) == HIGH && flag == 1) {
      digitalWrite(led, LOW);
      flag = 0;
      http.begin(server, port, "/lightoff");
      Serial.println("Door closed");
      send_http_get();
      http.end();
    }
  }
  delay(100);
}
