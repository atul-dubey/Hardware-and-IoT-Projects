/**********************************************************
   Author: Atul Dubey
   Date: 05/10/2022
   Description:
      This code allows you to get Analog values using the
      Analog multiplexer and then publish them over MQTT and also
      control a Bulb connected the device
 *********************************************************/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "WIFI SSID";
const char* password =  "WIFI PASSWORD";
char* topic = "button/pressed";
char msg3[1280];
char received[1280];
const char* mqttServer = "MQTT BROKER ADDRESS";
const int mqttPort = 1883;
int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module

double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  client.setCallback(callback);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266Client")) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }
  client.subscribe("button/pressed");

}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < 3; i++) {
    Serial.print((char)payload[i]);
    received[i] = (char)payload[i];
  }
  Serial.println("");
  String comp = String(received);
  Serial.println(comp);
  if (comp == "ONF")
  {
    digitalWrite(13, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    Serial.println("LED is ON");
  }
  else if (comp == "OFF")
  {
    digitalWrite(13, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    Serial.println("LED is OFF");
  }
  Serial.println();
  Serial.println("-----------------------");

}

void loop() {
  client.loop();
}
