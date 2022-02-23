/*********************************************************************
   Author: Atul Dubey
   Created on: 23rd Feb 2022
   Summary: This code displays text on the E-Paper Display
 *********************************************************************/

// Check if the PSRAM is available and enabled
#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM !!!"
#endif

//Include required libraries
#include "epd_driver.h"
#include "opensans18b.h"

//Declare the required global variables
uint8_t *framebuffer;

void setup()
{
  Serial.begin(115200);
  EDP_setup();
  displayMessage();
  epd_poweroff();
}

void loop()
{
  // Do nothing in the loop
}

/*********************************************************************
  Function to set buffer memory and initialize E-Paper Display
 ********************************************************************/
void EDP_setup() {
  char buf[128];    // Define buffer with size
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
   Function to show the welcome message
 *********************************************************************/
void displayMessage() {

  // Define the cursor variable and default location
  int cursor_x = 0;
  int cursor_y = 0;

  // Define the text to be displayed
  char *txt1 = "Welcome\n";
  char *txt2 = "to\n";
  char *txt3 = "Silken Automation\n";

  // Display text on the E-Paper Display
  cursor_x = 398;
  cursor_y = 160;
  writeln((GFXfont *)&OpenSans18B, txt1, &cursor_x, &cursor_y, NULL);
  vTaskDelay(100);

  cursor_x = 460;
  cursor_y += 50;
  writeln((GFXfont *)&OpenSans18B, txt2, &cursor_x, &cursor_y, NULL);
  vTaskDelay(100);

  cursor_x = 300;
  cursor_y += 50;
  writeln((GFXfont *)&OpenSans18B, txt3, &cursor_x, &cursor_y, NULL);
  vTaskDelay(100);
}
