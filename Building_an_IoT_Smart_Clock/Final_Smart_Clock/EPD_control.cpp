#include "EPD_control.h"

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
   Function to get the time/date data and store it in a global variable
 ********************************************************************/
boolean getLocalTime()
{
  struct tm timeinfo;
  char   time_output[30], day_output[30], update_time[30];

  const char* ntpServer = "pool.ntp.org";
  const long  gmtOffset_sec = 19800;  //Offset for IST
  const int   daylightOffset_sec = 0; //No Day Light saving

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  strftime(day_output, sizeof(day_output), "%a %b-%d-%Y", &timeinfo); // Creates  'Sat May-31-2019'
  strftime(update_time, sizeof(update_time), "%r", &timeinfo);        // Creates: '@ 02:05:49pm'
  sprintf(time_output, "%s", update_time);
  Date_str = day_output;
  Time_str = time_output;
  return true;
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
void displayDateTime() {
  getLocalTime();

  //Seperating time and phase
  String Time = Time_str.substring(0, 5);
  String Phase = Time_str.substring(8);

  // Define the cursor variable and default location
  int cursor_x = 500;
  int cursor_y = 40;
  drawString((GFXfont *)&time72, cursor_x, cursor_y, Time, LEFT);

  cursor_x = 790;
  cursor_y = 100;
  drawString((GFXfont *)&time24, cursor_x, cursor_y, Phase, LEFT);
  
  cursor_x = 500;
  cursor_y = 180;
  drawString((GFXfont *)&time24, cursor_x, cursor_y, Date_str, LEFT);
}

/*********************************************************************
   Function to show the the image on E-Paper display
 *********************************************************************/
void displayWeather() {
  getWeatherData(); // Get the current weather information
  
  // Define the cursor variable and default location
  int cursor_x = 0;
  int cursor_y = 0;

  String temp = "Temperature : " + String((main_temp - 273.15)) + " °C";
  String hum = "Humidity : " + String(main_humidity) + " %";
  String pres = "Pressure : " + String(main_pressure) + " hPa";

  // Display text on the E-Paper Display
  cursor_x = 200;
  cursor_y = 100;
  drawString((GFXfont *)&OpenSans24B, cursor_x, cursor_y, temp, LEFT);

  cursor_x = 200;
  cursor_y += 100;
  drawString((GFXfont *)&OpenSans24B, cursor_x, cursor_y, hum, LEFT);

  cursor_x = 200;
  cursor_y += 100;
  drawString((GFXfont *)&OpenSans24B, cursor_x, cursor_y, pres, LEFT);
}

/*********************************************************************
   Function to show the the image on E-Paper display
 *********************************************************************/
void displayDND() {
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

/*********************************************************************
   Function to show the the image on E-Paper display
 *********************************************************************/
void displayGraphic() {
  // Set the position and area of the image
  Rect_t area = {
    .x = 50,
    .y = 30,
    .width = graphic2_width,
    .height = graphic2_height,
  };

  epd_draw_grayscale_image(area, (uint8_t *)graphic2_data); //Draw DND image
  delay(100);
}

/*********************************************************************
   Function to show the the image on E-Paper display
 *********************************************************************/
void displayWelcomeImage() {
  // Set the position and area of the image
  Rect_t area = {
    .x = 100,
    .y = 200,
    .width = image_width,
    .height = image_height,
  };

  epd_draw_grayscale_image(area, (uint8_t *)image_data); //Draw DND image
  delay(100);
}

/*********************************************************************
    Function to cnvert string to an array of charecters and
    set the alignment of text
 ********************************************************************/
void drawString(const GFXfont *font, int x, int y, String text, alignment align) {
  char * Data  = const_cast<char*>(text.c_str());
  int  x1, y1; // The bounds of x,y and w and h of the variable 'text' in pixels.
  int w, h;
  int xx = x, yy = y;
  get_text_bounds((GFXfont *)font, Data, &xx, &yy, &x1, &y1, &w, &h, NULL);
  if (align == RIGHT)  x = x - w;
  if (align == CENTER) x = x - w / 2;
  int cursor_y = y + h;
  writeln((GFXfont *)font, Data, &x, &cursor_y, NULL);
}
