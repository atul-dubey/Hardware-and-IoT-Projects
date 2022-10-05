/********************************************************
   Author: Atul Dubey
   Date: 05/10/2022
   Description:
      Code for building gesture glove to control PC UI
 ********************************************************/

#include<Wire.h> //library for I2C communication

#include <ESP8266WiFi.h>  //library for connecting to WiFi
#include <ESP8266HTTPClient.h> //library for making a web client

#define Addr 0x53   // ADXL345 I2C address is 0x53(83)

const char* ip_address = "IP ADDRESS OF DEVICE CONNECTED TO PC";
const char* f = "http://" + ip_address + "/movement?state=forward"; //http://192.168.43.175
const char* b = "http://" + ip_address + "/movement?state=backward";
const char* l = "http://" + ip_address + "/movement?state=left";
const char* r = "http://" + ip_address + "/movement?state=right";
const char* s = "http://" + ip_address + "/movement?state=stop";

const char* ssid = "WIFI SSID";          // WiFi credentials of the network.
const char* password = "WIFI PASSWORD";
int oldtemp = 0;
int newtemp = 0;
int th_p = 100;
int th_n = -100;
int wifi_led = 10;
//unsigned long copymillis;

HTTPClient http;
void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);       // Start connecting to the WiFi network.

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200); // Wait until the connection is successful.
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print(WiFi.localIP());         // Display the IP address on the Serial Monitor to access the Lights.
  Serial.println("/");
}
void setup()
{
  Wire.begin(); // Initialise I2C communication as MASTER
  Serial.begin(115200); // Initialise serial communication, set baud rate = 9600
  setup_wifi(); //calling function for setting up WiFi
  pinMode(wifi_led, OUTPUT);
  /***********************Inetializing ADXL345 **************************/
  Wire.beginTransmission(Addr); // Start I2C Transmission
  Wire.write(0x2C); // Select bandwidth rate register
  Wire.write(0x0A); // Normal mode, Output data rate = 100 Hz
  Wire.endTransmission(); // Stop I2C transmission
  Wire.beginTransmission(Addr); // Start I2C Transmission
  Wire.write(0x2D); // Select power control register
  Wire.write(0x08); // Auto-sleep disable
  Wire.endTransmission(); // Stop I2C transmission
  Wire.beginTransmission(Addr); // Start I2C Transmission
  Wire.write(0x31); // Select data format register
  Wire.write(0x08); // Self test disabled, 4-wire interface, Full resolution, Range = +/-2g
  Wire.endTransmission(); // Stop I2C transmission

  delay(300);
}

void loop()
{
  //if(WiFi.status() != WL_CONNECTED)
  //{
  //  Serial.print("Connection lost");
  //  digitalWrite(wifi_led,LOW);
  //}
  //else if(WiFi.status() == WL_CONNECTED)
  //digitalWrite(wifi_led,HIGH);
  unsigned int data[6];

  for (int i = 0; i < 6; i++)

  {
    Wire.beginTransmission(Addr); // Start I2C Transmission
    Wire.write((50 + i)); // Select data register
    Wire.endTransmission(); // Stop I2C transmission
    Wire.requestFrom(Addr, 1);  // Request 1 byte of data

    // Read 6 bytes of data
    // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
    if (Wire.available() == 1)
    {
      data[i] = Wire.read();
    }
  }
  delay(100);
  /*********** Convert the data to 10-bits ***********/
  //copymillis = millis();
  int xAccl = (((data[1] & 0x03) * 256) + data[0]);
  if (xAccl > 511)
  {
    xAccl -= 1024;
  }

  int yAccl = (((data[3] & 0x03) * 256) + data[2]);
  if (yAccl > 511)
  {
    yAccl -= 1024;
  }

  int zAccl = (((data[5] & 0x03) * 256) + data[4]);
  if (zAccl > 511)
  {
    zAccl -= 1024;
  }

  /*************** Output data to serial monitor ************/

  //Serial.print("x : ");

  //Serial.println(xAccl);
  //Serial.print("\t");
  //Serial.print("y : ");
  //Serial.println(yAccl);
  //Serial.print("\t");
  //Serial.println(zAccl);
  //delay(300);

  if (xAccl > th_p)
  {
    // move_forward();  //calling function to move the robot in forward direction
    newtemp = 1;
  }

  else if (xAccl < th_n)
  {
    // move_backward();
    newtemp = 2;
  }

  else if (yAccl > th_p)
  {
    //move_right();
    newtemp = 3;
  }

  else if (yAccl < th_n)
  {
    newtemp = 4;
    //move_left();
  }
  else if (xAccl > th_n && xAccl < th_p && yAccl > th_n && yAccl < th_p)
  {
    newtemp = 5;
    //Stop();
  }

  delay(100);
  if (newtemp != oldtemp)
  {
    if (newtemp == 1)
    {
      move_forward();
      //oldtemp=newtemp;
    }
    if (newtemp == 2)
    {
      //oldtemp=newtemp;
      move_backward();
    }

    if (newtemp == 3)
    {
      move_right();
      // oldtemp=newtemp;
    }

    if (newtemp == 4)
    {
      move_left();
      // oldtemp=newtemp;
    }

    if (newtemp == 5)
    {
      Stop();
      //oldtemp=newtemp;
    }
    oldtemp = newtemp;
    Serial.println(newtemp);
  }

}

void move_forward()
{
  http.begin(f);
  int httpCode = http.GET();   // start connection and send HTTP header

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
    else
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  }
  http.end();
  delay(100);
}
void move_backward()
{
  http.begin(b);
  //Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
    else
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  }
  http.end();
  delay(100);
}

void move_left()
{
  http.begin(l);
  //Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
    else
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  }
  http.end();
  delay(100);
}

void move_right()
{
  http.begin(r);
  //Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
    else
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  }
  http.end();
  delay(100);
}

void Stop()
{
  http.begin(s);
  //Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
    else
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  }
  http.end();
  delay(100);
}
