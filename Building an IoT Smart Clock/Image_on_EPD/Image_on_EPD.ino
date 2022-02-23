/*********************************************************************
   Author: Atul Dubey
   Created on: 23rd Feb 2022
   Summary: This code displays image on the E-Paper Display
 *********************************************************************/

// Check if the PSRAM is available and enabled
#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM !!!"
#endif

//Include the required libraries
#include "epd_driver.h"
#include "opensans18b.h"
#include "image.h"

//Declare the required global variables
uint8_t *framebuffer;

void setup()
{
  Serial.begin(115200);
  EDP_setup();
  displayImage();
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
   Function to show the the image on E-Paper display
 *********************************************************************/
void displayImage() {
  // Set the position and area of the image
  Rect_t area = {
    .x = 100,
    .y = 200,
    .width = image_width,
    .height = image_height,
  };

  epd_draw_grayscale_image(area, (uint8_t *)image_data); //Draw logo
  vTaskDelay(100);
}
