/*********************************************************************
   Author: Atul Dubey
   Created on: 23rd Feb 2022
   Summary: This code displays date and time on the E-Paper Display
 *********************************************************************/

// Check if the PSRAM is available and enabled
#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM !!!"
#endif

// Include the required libraries
#include "epd_driver.h"
#include <WiFi.h>
#include "time.h"

// Include the font header
#include "opensans24b.h"
#include "time24.h"
#include "time72.h"

// Declare the required global variables
uint8_t *framebuffer;
String  Time_str = "--:--:--";
String  Date_str = "-- --- ----";

enum alignment {LEFT, RIGHT, CENTER}; // Define new data type for text alignment

void setup()
{
  Serial.begin(115200);
  wifiSetup();
  EPD_setup();
  displayDateTime();
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
  const char* ssid       = "JioFiber101-2.4";
  const char* password   = "22011995";

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
