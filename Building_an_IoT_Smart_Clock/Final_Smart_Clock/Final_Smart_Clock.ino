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
#include <HTTPClient.h>
#include <ArduinoJson.h>        // https://github.com/bblanchon/ArduinoJson
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#include "credentials.h"
#include "EPD_control.h"

WiFiClient client;  // Create an ESP8266 WiFiClient class to connect to the MQTT server.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);  // Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Subscribe voiceTrigger = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/dnd"); // Define the topic to be subscribed

float main_temp = 0;
float main_pressure = 0;
int main_humidity = 0;

void setup() {
  Serial.begin(115200);
  EPD_setup();
  displayWelcomeImage();
  wifiSetup();
  mqtt.subscribe(&voiceTrigger);
  epd_clear();
  displayGraphic();
  displayDateTime();
  displayWeather();
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

      else ESP.restart();

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

/*********************************************************************
    Function to get the weather data
 ********************************************************************/
void getWeatherData() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    String payload;

    // specify request destination
    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://api.openweathermap.org/data/2.5/weather?q=" + Location + "&APPID=" + API_Key))    // HTTP
    {

      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) // check the returning code
      {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
          payload = http.getString();
          Serial.println(payload);
        }
      }
      else
      {
        Serial.printf("[HTTP] GET... failed, error: %s\n",
                      http.errorToString(httpCode).c_str());
        return;
      }

      StaticJsonDocument<1024> doc;

      DeserializationError error = deserializeJson(doc, payload);

      if (error)
      {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      JsonObject main = doc["main"];
      main_temp = main["temp"]; // 301.45
      main_pressure = main["pressure"]; // 1004
      main_humidity = main["humidity"]; // 62

      // print data
      Serial.printf("Temperature = %.2f°C\r\n", main_temp);
      Serial.printf("Humidity = %d %%\r\n", main_humidity);
      Serial.printf("Pressure = %.3f bar\r\n", main_pressure);
    }
    else
    {
      Serial.println("[HTTP] .begin() failed");
    }

    http.end(); //Close connection
  }
}

/*********************************************************************
   Function to show the the image on E-Paper display
 *********************************************************************/
void displayWeather() {
  getWeatherData();
  // Set the position and area of the image
  Rect_t area1 = {
    .x = 550,
    .y = 280,
    .width = temp_width,
    .height = temp_height,
  };

  // Set the position and area of the image
  Rect_t area2 = {
    .x = 550,
    .y = 360,
    .width = hum_width,
    .height = hum_height,
  };

  // Set the position and area of the image
  Rect_t area3 = {
    .x = 550,
    .y = 440,
    .width = pres_width,
    .height = pres_height,
  };

  epd_draw_grayscale_image(area1, (uint8_t *)temp_data); //Draw DND image
  epd_draw_grayscale_image(area2, (uint8_t *)hum_data); //Draw DND image
  epd_draw_grayscale_image(area3, (uint8_t *)pres_data); //Draw DND image


  // Define the cursor variable and default location
  int cursor_x = 0;
  int cursor_y = 0;

  String temp = String((main_temp - 273.15)) + " °C";
  String hum = String(main_humidity) + " %";
  String pres = String(main_pressure) + " hPa";

  // Display text on the E-Paper Display
  cursor_x = 630;
  cursor_y = 290;
  drawString((GFXfont *)&OpenSans18B, cursor_x, cursor_y, temp, LEFT);

  cursor_x = 630;
  cursor_y += 80;
  drawString((GFXfont *)&OpenSans18B, cursor_x, cursor_y, hum, LEFT);

  cursor_x = 630;
  cursor_y += 80;
  drawString((GFXfont *)&OpenSans18B, cursor_x, cursor_y, pres, LEFT);

}
