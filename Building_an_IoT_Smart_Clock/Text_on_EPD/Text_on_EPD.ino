/*********************************************************************
   Author: Atul Dubey
   Created on: 23rd Feb 2022
   Summary: This code displays text on the E-Paper Display
 *********************************************************************/

// Check if the PSRAM is available and enabled
#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM !!!"
#endif

// Include required libraries
#include "epd_driver.h"

// Include the fond header files
#include "opensans18b.h"
#include "time24.h"
#include "time72.h"

//Declare the required global variables
uint8_t *framebuffer;

enum alignment {LEFT, RIGHT, CENTER}; // Define a new data type for Text Alignment

void setup()
{
  Serial.begin(115200);
  EPD_setup();
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
  cursor_x = 200;
  cursor_y = 100;
  drawString((GFXfont *)&OpenSans18B, cursor_x, cursor_y, txt1, LEFT);

  cursor_x = 200;
  cursor_y += 100;
  drawString((GFXfont *)&time72, cursor_x, cursor_y, txt2, LEFT);

  cursor_x = 200;
  cursor_y += 150;
  drawString((GFXfont *)&time24, cursor_x, cursor_y, txt3, LEFT);
  vTaskDelay(100);
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
