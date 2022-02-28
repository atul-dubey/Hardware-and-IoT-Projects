/*********************************************************************
   Author: Atul Dubey
   Created on: 25th Feb 2022
   Summary: This code allows you set DND mode using voice command from
            you android phone
 *********************************************************************/

// Check if the PSRAM is available and enabled
#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM !!!"
#endif

// Include the required libraries
#include "epd_driver.h"
#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// Include the image to be displayed
#include "dnd.h"

// Declare the required global variables
uint8_t *framebuffer;

// Wifi and server credentials
#define WLAN_SSID       "JioFiber101-2.4"
#define WLAN_PASS       "22011995"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "ad101"
#define AIO_KEY         "aio_zPvi77mHzVa3kAC1i0SIeeOzEYDo"

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
        displayImage();

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
  Function to set buffer memory and initialize E-Paper Display
 ********************************************************************/
void EPD_setup() {
  epd_init();    // Initialize the e-paper display

  // Set framebuffer size
  framebuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);

  // Check if the buffer memory is allocted successfully
  if (!framebuffer) {
    Serial.println("alloc memory failed !!!");
    while (1);
  }
  memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);    // Fill the buffer with white pixels
  epd_poweron();    // Turn the E-Paper display on
  epd_clear();    // clear the content on the display
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

/*********************************************************************
   Function to show the the image on E-Paper display
 *********************************************************************/
void displayImage() {
  // Set the position and area of the image
  Rect_t area = {
    .x = 50,
    .y = 30,
    .width = dnd_width,
    .height = dnd_height,
  };

  epd_draw_grayscale_image(area, (uint8_t *)dnd_data); //Draw DND image
  delay(100);
}
