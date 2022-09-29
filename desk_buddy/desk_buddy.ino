/*********************************************************
   Author: Atul Dubey
   Date: 28/09/2022
   Description:
       This code helps you create a smart Desk buddy
       which tells you traffic condition on the way to your home, and
       reflects that on a RGB LED.
 *********************************************************/
 
#include <ESP8266WiFi.h>          //wifi library
#include <GoogleMapsApi.h> // For accessing Google Maps Api
#include <ArduinoJson.h>    // For parsing the response from google maps and for the config file
#include <WiFiClientSecure.h>           //for secure requesting for API
#include <Adafruit_NeoPixel.h>           // Library for working with RGB LEDs
#include<Wire.h>                                  //for I2C communication
#include<Adafruit_SSD1306.h>           //library for OLED
#include<Adafruit_GFX.h>                 //library for animations on OLED

#define MEDIUM_TRAFFIC_THRESHOLD 60       //medium when delay>1min
#define BAD_TRAFFIC_THRESHOLD 300           //heavy when delay >5min 
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(-1);                              //instance for OLED library

char ssid[] = "WIFI_SSID";
char pass[] = "WIFI_PASSWORD";
String ori = "ORIGIN_LOCATION";                                   //for serial monitor use
String dest = "DESTINATION_LOCATION";
String origin = "ORIGIN_LONGITUDE, ORIGIN_LATTITUDE";            //origin coordinates
String destination = "DESTINATION_LONGITUDE, DESTINATION_LATTITUDE";      //destination coordinates
String responseString;
int button_num = 0;                                      //to know which button is pressed

char apiKey[45] = "GOOGLE MAPS API KEY";                  //Google Maps API Key

Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, 12, NEO_GRB + NEO_KHZ800);                                                  // Defining RGB LED strip.
WiFiClientSecure client;                                    //instance for client library
GoogleMapsApi *mapsApi;                           //instance for maps API

void red()                                                      //Function to Lit up RGBring in red
{
  for (uint16_t i = 0; i < 10; i++) {                                  // Starting from the first pixel(RGB LED), loop all 10 LEDs to set the colour.
    strip.setPixelColor(i, 255, 0, 0);                                            // Set the pixel
    //  colour, (Pixel_number, Pixel_colour).
  }
  strip.show();                                                                       // Display the set
  //     colours on the pixels.
}


void yellow()                                     //Function to Lit up RGBring in yellow
{
  for (uint16_t i = 0; i < 10; i++) {                                  // Starting from the first
    //  pixel(RGB LED), loop all 10 LEDs to set the colour.
    strip.setPixelColor(i, 255, 147, 0);                                            // Set the pixel
    //   colour, (Pixel_number, Pixel_colour).
  }
  strip.show();                                                                       // Display the set
  // colours on the pixels.
}


void off()                                                        //Function to turn RGB off
{
  for (uint16_t i = 0; i < 10; i++) {                                  // Starting from the first
    //  pixel(RGB LED), loop all 10 LEDs to set the colour.
    strip.setPixelColor(i, 0, 0, 0);                                            // Set the pixel
    // colour, (Pixel_number, Pixel_colour).
  }
  strip.show();                                                                       // Display the set
  //  colours on the pixels.
}


void green()                                        //Function to Lit up RGBring in green
{
  for (uint16_t i = 0; i < 10; i++) {                                  // Starting from the first
    //pixel(RGB LED), loop all 10 LEDs to set the colour.
    strip.setPixelColor(i, 0, 255, 0);                                            // Set the pixel colour,
    // (Pixel_number, Pixel_colour).
  }
  strip.show();
  // Display the set colours on the pixels.
}

void clearthedisplay()                         //Function for clearing the OLED screen
{
  display.clearDisplay();
  display.display();

}
void format_text()                              //function to give text formatting
{
  display.setTextSize(2);
  display.setTextColor(WHITE);
}
void checkGoogleMaps() {                            //function for sending and evaluating
  // the traffic request
  int durationInTrafficInSeconds;                //to capture time in  traffic

  if (button_num == 1)                                 //when button 1 pressed
  { Serial.println("Getting traffic for " + dest + " to " + ori);
    responseString = mapsApi->distanceMatrix(destination, origin, "now", "best_guess");
  }

  if (button_num == 2)                              //when button 2 is pressed
  { Serial.println("Getting traffic for " + ori + " to " + dest);
    responseString = mapsApi->distanceMatrix(origin, destination, "now", "best_guess");
  }

  Serial.println(responseString);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& response = jsonBuffer.parseObject(responseString);
  if (response.success()) {

    if (response.containsKey("rows")) {

      JsonObject& element = response["rows"][0]["elements"][0];
      String status = element["status"];
      Serial.println(status);
      Serial.println("Checking status");
      if (status == "OK") {
        Serial.println("Enter loop 1");
        int durationInSeconds = element["duration"]["value"];
        durationInTrafficInSeconds = element["duration_in_traffic"]["value"];
        Serial.print("Duration In Traffic:  ");
        Serial.println(durationInTrafficInSeconds);


        int difference = durationInTrafficInSeconds - durationInSeconds;
        //to know delay time
        Serial.print(difference);
        Serial.println("seconds");

        if ( difference > BAD_TRAFFIC_THRESHOLD )  //heavy traffic condition
        {
          Serial.println("Setting Colour to Red --- Bad Traffic");
          red();                                                      //to turn LED red
          clearthedisplay();                                  //clearing the display
          format_text();                                      //formatting the text
          display.setCursor(0, 0);                      //location to start printing from
          display.print("Heavy     Traffic");
          delay(2000);
          clearthedisplay();
          format_text();
          display.setCursor(0, 0);
          display.print(" ETA");
          format_text();
          display.setCursor(10, 20);                 //to print in different line
          display.print(durationInTrafficInSeconds / 60); // to display in mins
          format_text();
          display.setCursor(10, 40);                       //to print in different line
          display.print("Mins");
          delay(2000);
          clearthedisplay();
          format_text();
          display.setCursor(0, 0);
          display.print("Delay By");
          format_text();
          display.setCursor(10, 20);
          display.print(difference / 60);
          format_text();
          display.setCursor(10, 40);
          display.print("Mins");
          delay(2000);
          clearthedisplay();
          off();                                                  //to off the RGB ring
        }
        else if (difference > MEDIUM_TRAFFIC_THRESHOLD)
          //condition for medium traffic
        {
          Serial.println("Setting Colour to yellow -- Medium Traffic");
          yellow();
          clearthedisplay();
          format_text();
          display.setCursor(0, 0);
          display.print("Medium    Traffic");
          delay(2000);
          clearthedisplay();
          format_text();
          display.setCursor(0, 0);
          display.print(" ETA");
          format_text();
          display.setCursor(10, 20);
          display.print(durationInTrafficInSeconds / 60);
          format_text();
          display.setCursor(10, 40);
          display.print("Mins");
          delay(2000);
          clearthedisplay();
          format_text();
          display.setCursor(0, 0);
          display.print("Delay By");
          format_text();
          display.setCursor(10, 20);
          display.print(difference / 60);
          format_text();

          display.setCursor(10, 40);
          display.print("Mins");
          delay(2000);
          clearthedisplay();
          off();
        }

        else                                            //condition for lite traffic and no delay
        {
          Serial.println("Setting Colour to Green");
          green();
          clearthedisplay();
          format_text();
          display.setCursor(0, 0);
          display.print("Lite      Traffic");
          delay(2000);
          clearthedisplay();
          format_text();
          display.setCursor(0, 0);
          display.print(" ETA");
          format_text();
          display.setCursor(10, 20);
          display.print(durationInTrafficInSeconds / 60);
          format_text();
          display.setCursor(10, 40);
          display.print("Mins");
          delay(2000);
          clearthedisplay();
          format_text();
          display.setCursor(0, 0);
          display.print("Delay By");
          format_text();
          display.setCursor(10, 20);
          display.print(difference / 60);
          format_text();
          display.setCursor(10, 40);
          display.print("Mins");
          delay(2000);
          clearthedisplay();
          off();
        }
      }
      else {                                //to capture and display errors from API
        Serial.println("Entered 2");
        Serial.println("Got an error status: " + status);

      }
    }
    else
    { // no row error from API
      Serial.println("Entered 3");
      Serial.println("Reponse did not contain rows");

    }
  }
  else                                            //time out or no network error
  {
    Serial.println("Entered 4");
    if (responseString == "") {
      Serial.println("No response, probably timed out");
      clearthedisplay();
      format_text();
      display.setCursor(0, 0);
      display.print("Check  Network");

    } else {                                                   //failed json responce
      Serial.println("Failed to parse Json. Response:");
      Serial.println(responseString);
    }
  }
}



void setup() {

  Serial.begin(115200);                       //to starting Serial Monitor
  // Wire.begin();                                   //to start I2C communication
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  //starting display
  clearthedisplay();                          //to clear the screen
  WiFi.begin(ssid, pass);                              //to start wifi connection
  format_text();
  display.setCursor(0, 0);
  display.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)     //produce animation until connected
  {
    Serial.println(".");
    delay(100);

  }
  pinMode(14, INPUT);                            //buttons as input
  pinMode(13, INPUT);
  strip.begin();                                         //to start RGB ring
  mapsApi = new GoogleMapsApi(apiKey, client);   //configuring  API
  Serial.println("");
  clearthedisplay();
  format_text();;
  display.setCursor(0, 0);
  display.print("WiFi Connected");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());                           //printing IP iddress of Octabrix
  off();

}

void loop() {
  if (digitalRead(14) == HIGH)       //to know if 1 button pressed
  { clearthedisplay();
    Serial.println("Work      to        Home");
    format_text();
    display.setCursor(0, 0);
    display.print("Work      to        Home");       //printing on OLED
    button_num = 1;                                         //assigning as button 1 is pressed
    checkGoogleMaps();                          //calling maps function

  }

  if (digitalRead(13) == HIGH)       //to know if 2 button pressed
  { clearthedisplay();
    Serial.print("Home      to        Work");
    format_text();
    display.setCursor(0, 0);
    display.print("Home      to        Work");     //printing on OLED
    button_num = 2;                                       //assigning as button 1 is pressed
    checkGoogleMaps();                              //calling maps function

  }

}
