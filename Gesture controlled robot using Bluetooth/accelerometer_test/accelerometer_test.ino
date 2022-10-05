/****************************************************
   Author: Atul Dubey
   Date: 05/10/2022
   Description:
      This code will help you in reading ADXL345 sensor data
 ****************************************************/

#include<Wire.h>

#define Addr 0x53   // ADXL345 I2C address is 0x53(83)

void setup() {
  Wire.begin(); // Initialise I2C communication as MASTER
  Serial.begin(9600); // Initialise serial communication, set baud rate = 9600

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

void loop() {
  unsigned int data[6];

  for (int i = 0; i < 6; i++) {
    Wire.beginTransmission(Addr); // Start I2C Transmission
    Wire.write((50 + i)); // Select data register
    Wire.endTransmission(); // Stop I2C transmission
    Wire.requestFrom(Addr, 1);  // Request 1 byte of data

    // Read 6 bytes of data
    // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb

    if (Wire.available() == 1) {
      data[i] = Wire.read();
    }
  }

  /*********** Convert the data to 10-bits ***********/

  int xAccl = (((data[1] & 0x03) * 256) + data[0]);

  if (xAccl > 511) {
    xAccl -= 1024;
  }

  int yAccl = (((data[3] & 0x03) * 256) + data[2]);
  if (yAccl > 511) {
    yAccl -= 1024;
  }

  int zAccl = (((data[5] & 0x03) * 256) + data[4]);

  if (zAccl > 511) {
    zAccl -= 1024;
  }

  /*************** Output data to serial monitor ************/

  //Serial.print("x : ");

  Serial.println(xAccl);
  Serial.print("\t");
  Serial.println(yAccl);
  //Serial.print("\t");
  //Serial.println(zAccl);
  delay(300);
}
