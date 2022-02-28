/*********************************************************************
   Author: Atul Dubey
   Created on: 23rd Feb 2022
   Summary: This code displays weather data on the E-Paper Display
 *********************************************************************/

// Check if the PSRAM is available and enabled
#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM !!!"
#endif

// Include the required libraries
#include "epd_driver.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>        // https://github.com/bblanchon/ArduinoJson

// Include the font header
#include "opensans18b.h"
#include "time24.h"

// Include the images
#include "temp.h"
#include "hum.h"
#include "pres.h"

// set location and API key
String Location = "Indore";
String API_Key = "2e98d6d804cccc57c78dc6c8b03db205";

// Declare the required global variables
uint8_t *framebuffer;
float main_temp = 0;
float main_pressure = 0;
int main_humidity = 0;

enum alignment {LEFT, RIGHT, CENTER}; // Creates a new data type (alignment) for the text alignement

void setup()
{
  Serial.begin(115200);
  
  wifiSetup();
  EPD_setup();
  displayWeather();
  epd_poweroff();
}

void loop()
{
  // Do nothing in the loop
}

/*********************************************************************
   Function for Wifi setup
 ********************************************************************/
void wifiSetup() {
  const char* ssid = "JioFiber101-2.4";
  const char* password = "22011995";

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
}

/*********************************************************************
  Function to set buffer memory and initialize E-Paper Display
 ********************************************************************/
void EPD_setup() {
  epd_init();    // Initialize the e-paper display

  // Set framebuffer size and allocate memory for the same
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
   Function to show the the image on E-Paper display
 *********************************************************************/
void displayWeather() {
  getWeatherData(); // Get the current weather information

    // Set the position and area of the image
  Rect_t area1 = {
    .x = 500,
    .y = 230,
    .width = temp_width,
    .height = temp_height,
  };

    // Set the position and area of the image
  Rect_t area2 = {
    .x = 500,
    .y = 330,
    .width = hum_width,
    .height = hum_height,
  };

    // Set the position and area of the image
  Rect_t area3 = {
    .x = 500,
    .y = 430,
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
  cursor_x = 580;
  cursor_y = 240;
  drawString((GFXfont *)&OpenSans18B, cursor_x, cursor_y, temp, LEFT);

  cursor_x = 580;
  cursor_y += 100;
  drawString((GFXfont *)&OpenSans18B, cursor_x, cursor_y, hum, LEFT);

  cursor_x = 580;
  cursor_y += 100;
  drawString((GFXfont *)&OpenSans18B, cursor_x, cursor_y, pres, LEFT);
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
    Function to convert string to an array of charecters and
    set the alignment of text
 ********************************************************************/
void drawString(const GFXfont *font, int x, int y, String text, alignment align) {
  char * Data  = const_cast<char*>(text.c_str());
  int  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  int w, h;
  int xx = x, yy = y;
  get_text_bounds((GFXfont *)font, Data, &xx, &yy, &x1, &y1, &w, &h, NULL);
  if (align == RIGHT)  x = x - w;
  if (align == CENTER) x = x - w / 2;
  int cursor_y = y + h;
  writeln((GFXfont *)font, Data, &x, &cursor_y, NULL);
}
