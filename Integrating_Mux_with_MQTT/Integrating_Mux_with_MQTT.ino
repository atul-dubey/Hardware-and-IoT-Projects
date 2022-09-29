/***************************************************
 * Author: Atul Dubey
 * Date: 28/09/2022
 * Description:
 *  This code helps in integrating 4051 Multiplexer
 *  with ESP8266 to add multiple analog sensors.
 *************************************************/
 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int r0 = 0;      //value of select pin at the 4051 (s0)
int r1 = 0;      //value of select pin at the 4051 (s1)
int r2 = 0;      //value of select pin at the 4051 (s2)
int s0 = 15;
int s1 = 2;
int s2 = 0;
int count = 0;   //which y pin we are selecting
const char* ssid = "WIFI_SSID";
const char* password =  "WIFI_PASSWORD";
char* topic = "button/pressed";
char msg[1280];
char msg1[1280];
char received[1280];
const char* mqttServer = "192.168.1.139";
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
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    received[i] = (char)payload[i];
  }
  Serial.println("");
  String comp = String(received);
  Serial.println(comp);
  if (comp == "ONF")
  {
    digitalWrite(13, HIGH);
    Serial.println("LED is ON");
  }
  else if (comp == "OFF")
  {
    digitalWrite(13, LOW);
    Serial.println("LED is OFF");
  }
  Serial.println();
  Serial.println("-----------------------");

}

void loop() {
  int x[2];
  int m1, m11, m2;
  client.loop();
  for (count = 0; count <= 1; count++) {
    // select the bit
    r0 = bitRead(count, 0);   // use this with arduino 0013 (and newer versions)
    r1 = bitRead(count, 1);   // use this with arduino 0013 (and newer versions)
    r2 = bitRead(count, 2);   // use this with arduino 0013 (and newer versions)
    digitalWrite(s0, r0);
    digitalWrite(s1, r1);
    digitalWrite(s2, r2);
    //Either read or write the multiplexed pin here
    x[count] = analogRead(A0);
  }
  m1 = map(x[0], 0, 1010, 0, 2000);
  m2 = map(x[1], 0, 1010, 0, 500);
  String y = String(m1);
  String y1 = String(m2);

  y.toCharArray(msg, 1280);
  y1.toCharArray(msg1, 1280);
  client.publish("Appliance/AirConditioner", msg );
  delay(1000);
  client.publish("Appliance/Television", msg1 );
  delay(1000);

}
