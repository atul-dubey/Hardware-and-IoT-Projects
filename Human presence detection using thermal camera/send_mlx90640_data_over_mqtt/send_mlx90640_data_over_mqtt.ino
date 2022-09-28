/********************************************
  Author: Atul Dubey
  Date: 28 september 2022
  Description: This code allows us to read the 
  MLX90640 data, put that data into a JSON object,
  serialize the data, and then send it over MQTT
 *******************************************/


/********************************************
   Include the required libraries
 *******************************************/
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

/********************************************
   Define the valriable and required values
 *******************************************/
#define MQTT_MAX_PACKET_SIZE 1000
const byte MLX90640_address = 0x33; //Default 7-bit unshifted address of the MLX90640
#define TA_SHIFT 8 //Default shift for MLX90640 in open air
float mlx90640To[768];
paramsMLX90640 mlx90640;
#define cap 2560  // This the maximum buffer size for storing sensor data

const char* ssid = "YOUR_WIFI_SSID";
const char* password =  "YOUR_WIFI_PASSWORD";
const char* mqttServer = "YOUR_MQTT_BROKER_IP_ADDRESS";
const int mqttPort = 1883;  // Standard MQTT port
const byte led_gpio = 2; // We can use the onboard LED for showing data transfer

/********************************************
   If you are using password authentication
   for MQTT broker, add them below
 *******************************************/
//const char* mqttUser = "******";
//const char* mqttPassword = "12345";

/********************************************
   Declare required instances
 *******************************************/
WiFiClient espClient;
PubSubClient client(espClient);

/********************************************
   All the one time code goes here
 *******************************************/
void setup()
{
  Wire.begin();
  Wire.setClock(400000); //Increase I2C clock speed to 400kHz
  Serial.begin(115200); //Fast serial as possible

  pinMode(led_gpio, OUTPUT);  // Set the GPIO to output

  setup_wifi(); // Connect to the WiFi

  if (client.setBufferSize(2560) == true) // Set the size of data that can be sent over MQTT
  {
    Serial.print("buffer size set 2560");
  }
  else Serial.print("buffer size setting failed");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  if (isConnected() == false) //Check if the sensor is still connected
  {
    Serial.println("MLX90640 not detected at default I2C address. Please check wiring. Freezing.");
    while (1);
  }
  else Serial.println("MLX90640 online!");

  // Get device parameters - We only have to do this once
  int status;
  uint16_t eeMLX90640[832];
  status = MLX90640_DumpEE(MLX90640_address, eeMLX90640);
  if (status != 0)
    Serial.println("Failed to load system parameters");

  status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
  if (status != 0)
    Serial.println("Parameter extraction failed");

  // Once params are extracted, we can release eeMLX90640 array
  MLX90640_SetRefreshRate(MLX90640_address, 0x03); //Set rate to 4Hz
}

/********************************************
   Function for setting up WiFi
 *******************************************/
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

/********************************************
   Function to receive the data received over MQTT
 *******************************************/
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}

/********************************************
   Function for connecting to MQTT broker
 *******************************************/
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("\n Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
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

void loop()
{
  client.loop();

  for (byte x = 0 ; x < 2 ; x++)
  {
    uint16_t mlx90640Frame[834];
    int status = MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);

    float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
    float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);

    float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
    float emissivity = 0.95;

    MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);
    Serial.println(" ");
  }

  /*******************DynamicJsonDocument*****************************/
  DynamicJsonDocument doc(13000);
  doc["device"] = "ESP32";
  doc["sensorType"] = "T-Scanner";
  char arr[786];
  for (int x = 0 ; x < 768 ; x++)   //768 384
  {
    Serial.print(mlx90640To[x], 0); //value from sensor print on com port
    doc["values"][x] = (uint8_t)mlx90640To[x];   // AARAY value of 786 is store in values
    Serial.print(",");
  }

  digitalWrite(led_gpio, HIGH);
  Serial.println("\nSending message to MQTT topic..");
  char JSONmessageBuffer[cap];                            // JSON MESSAGE CHAR BUFFER
  Serial.println(serializeJson(doc, Serial));
  size_t n = serializeJson(doc, JSONmessageBuffer);     // formation of serialization of data
  size_t sizeq = sizeof(JSONmessageBuffer) / sizeof(JSONmessageBuffer[0]);
  Serial.print("  JSONmessageBuffer_size: ");
  Serial.println(sizeq);
  Serial.print("\nJSONmessageBuffer: ");
  Serial.println(JSONmessageBuffer);

  //*************************Data send to PUBLISH TO MQTT TOPIC******************************/
  if (client.publish("ev8051/esp32/temperature", JSONmessageBuffer, n) == true)
  {
    Serial.println("\nSuccess sending message");
  } else {
    Serial.println("\nError sending message");
  }
  digitalWrite(led_gpio, LOW);

  if (!client.connected()) {
    reconnect();
  }

  client.loop();
  Serial.println("-------------");

  delay(5000);
}

/********************************************
   Function to check if the sensor is connected
 *******************************************/
boolean isConnected()
{
  Wire.beginTransmission((uint8_t)MLX90640_address);
  if (Wire.endTransmission() != 0)
    return (false); //Sensor did not ACK
  return (true);
}
