/****************************************************
   Author: Atul Dubey
   Date: 05/10/2022
   Description:
      Thi code will help you get the analog readings
      from the drum pads and then send corresponding
      signals over MQTT
 ****************************************************/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "WIFI SSID";
const char* password = "WIFI PASSWORD";
const char* mqtt_server = "MQTT BROKER ADDRESS";
int threshold = 500;
WiFiClient espClient;
PubSubClient client(espClient);

char msg[50];
int value = 0;

int r0 = 0;      //value of select pin at the 4051 (s0)
int r1 = 0;      //value of select pin at the 4051 (s1)

int s0 = 4;
int s1 = 5;
int d1, d2, d3, d4;
int oldtemp = 0;
int newtemp = 0;
int count = 0;   //which y pin we are selecting

void setup() {

  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
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
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Clien")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
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

  int x[8];
  for (count = 0; count <= 3; count++) {
    // select the bit
    r0 = bitRead(count, 0);   // use this with arduino 0013 (and newer versions)
    r1 = bitRead(count, 1);   // use this with arduino 0013 (and newer versions)

    digitalWrite(s0, r0);
    digitalWrite(s1, r1);

    //Either read or write the multiplexed pin here
    x[count] = analogRead(A0);
    //Serial.println(analogRead(A0));
  }
  d1 = x[0];
  d2 = x[1];
  d3 = x[2];
  d4 = x[3];
  if (d1 > threshold)
  {
    newtemp = 1;
    // Serial.print('1');
  }
  else if (d2 > threshold)
  {
    newtemp = 2;
    //Serial.print('2');
  }
  else if (d3 > threshold)
  {
    newtemp = 3;
    //Serial.print('3');
  }
  else if (d4 > threshold)
  {
    newtemp = 4;
    //Serial.print('4');
  }
  else
  {
    newtemp = 5;
  }
  delay(10);
  if (newtemp != oldtemp)
  {
    if (newtemp == 1)
    {
      String msg = "1";
      char Y[10];
      msg.toCharArray(Y, 10);
      client.publish("drum", Y);
      Serial.println(Y);
    }
    if (newtemp == 2)
    {
      String msg = "2";
      char Y[10];
      msg.toCharArray(Y, 10);
      client.publish("drum", Y);
      Serial.println(Y);
    }
    if (newtemp == 3)
    {
      String msg = "3";
      char Y[10];
      msg.toCharArray(Y, 10);
      client.publish("drum", Y);
      Serial.println(Y);
    }

    if (newtemp == 4)
    {
      String msg = "4";
      char Y[10];
      msg.toCharArray(Y, 10);
      client.publish("drum", Y);
      Serial.println(Y);

    }

    if (newtemp == 5)
    {
      String msg = "5";
      char Y[10];
      msg.toCharArray(Y, 10);
      //client.publish("", Y);
      Serial.println(Y);

    }

    oldtemp = newtemp;
    //Serial.println(newtemp);
  }

}
