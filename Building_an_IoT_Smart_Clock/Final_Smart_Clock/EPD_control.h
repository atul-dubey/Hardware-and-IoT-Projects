#ifndef EPD_CONTROL_H
#define EPD_CONTROL_H
#include <Arduino.h>

// Declare the required global variables
uint8_t *framebuffer;
String  Time_str = "--:--:--";
String  Date_str = "-- --- ----";
float main_temp = 0;
float main_pressure = 0;
int main_humidity = 0;

enum alignment {LEFT, RIGHT, CENTER}; // Define new data type for text alignment

#include "epd_driver.h" //e-paper display driver
#include "time.h"

// Include the font header files
#include "time24.h"
#include "time72.h"

// Include the image to be displayed
#include "dnd.h"
#include "image.h"
#include "graphic2.h"
#include "hum.h"
#include "pres.h"
#include "temp.h"

#endif
