/*********************************************************************
   Author: Atul Dubey
   Created on: 25th Feb 2022
   Summary: This code displays a Smart clock on E-Paper Display
 *********************************************************************/

// Check if the PSRAM is available and enabled
#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM !!!"
#endif

// Include the required libraries
#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>        // https://github.com/bblanchon/ArduinoJson

#include "credentials.h"
#include "EPD_control.h"

WiFiClient client;  // Create an ESP8266 WiFiClient class to connect to the MQTT server.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);  // Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Subscribe voiceTrigger = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/dnd"); // Define the topic to be subscribed

void setup() {
  Serial.begin(115200);
  EPD_setup();
  wifiSetup();
  mqtt.subscribe(&voiceTrigger);
}

void loop() {
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;

  /*********************************************************************
    Check the string received from the MQTT server on the subscribed topic
    and perform action accordingly
   ********************************************************************/
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &voiceTrigger) {
      Serial.print(F("Got: "));
      Serial.println((char *)voiceTrigger.lastread);
      if (!strcmp((char*) voiceTrigger.lastread, "dnd"))
        displayDND();

      else epd_clear();

    }
  }
}

/*********************************************************************
   Function for Wifi setup
 ********************************************************************/
void wifiSetup() {
  Serial.printf("Connecting to %s ", WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
}

/*********************************************************************
   Function for connecting to MQTT Server
 ********************************************************************/
void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
