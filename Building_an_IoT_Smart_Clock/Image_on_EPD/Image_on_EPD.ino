/*********************************************************************
   Author: Atul Dubey
   Created on: 23rd Feb 2022
   Summary: This code displays images on the E-Paper Display
 *********************************************************************/

// Check if the PSRAM is available and enabled
#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM !!!"
#endif

// Include the required libraries
#include "epd_driver.h"

// Include the image to be displayed
#include "image.h"
#include "graphic2.h"
#include "graphic3.h"
#include "graphic4.h"

// Declare the required global variables
uint8_t *framebuffer;

void setup()
{
  Serial.begin(115200);
  EPD_setup();
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
void displayImage() {
  // Set the position and area of the image
  Rect_t area = {
    .x = 100,
    .y = 200,
    .width = image_width,
    .height = image_height,
  };
  Rect_t area1 = {
    .x = 51,
    .y = 45,
    .width = graphic2_width,
    .height = graphic2_height,
  };
  epd_draw_grayscale_image(area, (uint8_t *)image_data); // Display image
  delay(3000);

  epd_clear();
  delay(500);

  epd_draw_grayscale_image(area1, (uint8_t *)graphic2_data); // Display image
  delay(3000);

  epd_clear();
  delay(500);

  epd_draw_grayscale_image(area1, (uint8_t *)graphic3_data); // Display image
  delay(3000);

  epd_clear();
  delay(500);

  epd_draw_grayscale_image(area1, (uint8_t *)graphic4_data); // Display image
  delay(3000);
}
