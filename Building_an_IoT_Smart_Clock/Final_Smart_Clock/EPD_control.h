#ifndef EPD_CONTROL_H
#define EPD_CONTROL_H
#include <Arduino.h>

enum alignment {LEFT, RIGHT, CENTER}; // Define new data type for text alignment

#include "epd_driver.h" //e-paper display driver
#include "time.h"

// Include the font header files
#include "time24.h"
#include "time72.h"
#include "opensans18b.h"

// Include the image to be displayed
#include "dnd.h"
#include "image.h"
#include "graphic2.h"
#include "hum.h"
#include "pres.h"
#include "temp.h"

//Function prototypes
void EPD_setup();
boolean getLocalTime();
void displayDateTime();
void displayDND();
void displayGraphic();
void displayWelcomeImage();
void drawString(const GFXfont *font, int x, int y, String text, alignment align);


#endif
