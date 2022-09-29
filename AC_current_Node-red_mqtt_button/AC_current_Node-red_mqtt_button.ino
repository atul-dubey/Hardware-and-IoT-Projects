/*=======================================================
  Author: Atul Dubey
  Date: 29/09/2022
  Description: Track the current and power consumed by DC load 
  using ACS712 Current sensor and node red.
  Sensitivity of Acs712 current sensor is 185mv/A
  =======================================================*/
/* Include libraries */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


/* Defining global variables and constants */
char ssid[] = "YouKnowMe";                                                   // Variables to store network credentials
char pass[] = "IKnowYou";
char msg[50];                                                             // Varaible for storing final message to be published
const char* mqttServer = "192.168.43.91";                                  // Address of the computer running MQTT broker

int relayPin = 4;                                                         // Variable for relay output pin
double sensor_Vol = 0;                                                    // Variable for storing voltage on ADC pin duue to sensor
double Vrms = 0;                                                          // Variable for storing RMS value of the voltage
double AmpsRMS = 0;                                                       // Variale for storing final RMS value of current
float initialError;                                                       // Variable for storing output value of sensor at no load
int Sensitivity = 185;                                                    // use 100 for 20A Module and 66 for 30A Module
uint32_t start_time;                                                      // Variable for saving start time of the loop

WiFiClient espClient;                                                       // Creating a Wi-Fi client
PubSubClient client(espClient);                                             // Creating instance of PubSubClient library

/* Function for making WiFi connection */
void WiFi_connect() {
  WiFi.begin(ssid, pass);                                                   // Initializing WiFi connection
  while (WiFi.status() != WL_CONNECTED) {                                   // Checking for connection untill connected
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to the WiFi network");
}
void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  Serial.begin(115200);                                                     // Initializing serial monitor
  WiFi_connect();                                                           // Calling function for WiFi connection
  client.setServer(mqttServer, 1883);                                       // Setting MQTT broker and port number
  client.setCallback(callback);

}

/* Function for re-establishing MQTT connection with broker */
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("h1")) {                                  // Attempt to connect
      Serial.println("connected");
      client.subscribe("button1");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/* Function to receive values from subscribed topic */
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < 1; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(relayPin, HIGH);   // Turn the LED on (Note that LOW is the voltage level

  } else if ((char)payload[0] == '0')
  {
    digitalWrite(relayPin, LOW);  // Turn the LED off by making the voltage HIGH
  }
}

/* Function for getting peak to peak value of ADC value of sensor */
float getVPP()
{
  float result;
  int sensor_ADC;                                                           //value read from the sensor
  int maxValue = 0;                                                         // store max value here
  int minValue = 1024;                                                      // store min value here

  uint32_t start_time1 = millis();
  while ((millis() - start_time1) < 1000) //sample for 1 Sec
  {
    sensor_ADC = analogRead(A0);

    if (sensor_ADC > maxValue)                                              // Chicking for new maxValue
    {
      /*record the maximum sensor value*/
      maxValue = sensor_ADC;
    }
    if (sensor_ADC < minValue)
    {
      /*record the minimum sensor value*/
      minValue = sensor_ADC;
    }
  }
  result = ((maxValue - minValue) * 3.34) / 1024.0;                         // Subtract min from max

  return result;
}
void loop() {
  if (!client.connected()) {                                                // Calling reconnect function when the connection breaks
    reconnect();
  }
  client.loop();                                                            // Keeping MQTT connection alive

  sensor_Vol = getVPP();                                                    // Getting peak to peak value of voltage from sensor
  Vrms = (sensor_Vol / 2.0) * 0.707;                                        // Calculating RMS value of sensor voltage
  AmpsRMS = ((Vrms * 1000) / Sensitivity);                                  // Calculating RMS value of load current

  Serial.print(AmpsRMS);
  Serial.println(" Amps");

  String y = String(AmpsRMS);                                               // Converting Current value to string
  y.toCharArray(msg, 50);                                                   // Converting the string to character array
  client.publish("h1", msg);                                                // Publishing the current value to a topic
  delay(200);
}
