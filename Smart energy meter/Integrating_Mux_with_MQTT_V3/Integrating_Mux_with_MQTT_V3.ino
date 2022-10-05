/**********************************************************
   Author: Atul Dubey
   Date: 05/10/2022
   Description:
      This code allows you to get Analog values using the
      Analog multiplexer and then publish them over MQTT
 *********************************************************/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int r0 = 0;      //value of select pin at the 4051 (s0)
int r1 = 0;      //value of select pin at the 4051 (s1)
int r2 = 0;      //value of select pin at the 4051 (s2)
int s0 = 14;
int s1 = 5;
int s2 = 4;
int count = 0;   //which y pin we are selecting
const char* ssid = "WIFI SSID";
const char* password =  "WIFI PASSWORD";
char* topic = "button/pressed";
char msg[1280];
char msg1[1280];
char msg2[1280];
char received[1280];
const char* mqttServer = "MQTT BROKER ADDRESS";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(13, OUTPUT);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  client.setServer(mqttServer, mqttPort);

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
}

void loop() {
  int x[3];
  int m1, m11, m2, m3;
  client.loop();
  for (count = 0; count <= 2; count++) {
    // select the bit
    r0 = bitRead(count, 0);   // use this with arduino 0013 (and newer versions)
    r1 = bitRead(count, 1);   // use this with arduino 0013 (and newer versions)
    r2 = bitRead(count, 2);   // use this with arduino 0013 (and newer versions)
    digitalWrite(s0, r0);
    digitalWrite(s1, r1);
    digitalWrite(s2, r2);
    //Either read or write the multiplexed pin here
    x[count] = analogRead(A0);
    Serial.println(analogRead(A0));
  }
  m1 = map(x[0], 0, 1010, 0, 3000);
  m2 = map(x[1], 0, 1010, 0, 1500);
  m3 = map(x[2], 0, 1010, 0, 500);

  String y = String(m1);
  String y1 = String(m2);
  String y2 = String(m3);
  y.toCharArray(msg, 1280);
  y1.toCharArray(msg1, 1280);
  y2.toCharArray(msg2, 1280);

  client.publish("Appliance/TV", msg2 );
  client.publish("Appliance/AC", msg1 );
  client.publish("Appliance/GEN", msg );
  delay(1000);
}
