/**************************************************************
   Author: Atul Dubey
   Date: 05/10/2022
   Description:
      Project: Wifi controlled LED matrix display
      NodeMCU pins    -> EasyMatrix pins
      MOSI-D7-GPIO13  -> DIN
      CLK-D5-GPIO14   -> Clk
      GPIO0-D3        -> LOAD
***************************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

#define SSID "WIFI SSID"                      // insert your SSID
#define PASS "WIFI PASSWORD"                    // insert your password
// ******************* String form to sent to the client-browser ************************************
String form =
  "<p>"
  "<center>"
  "<h1>ESP8266 Web Server</h1>"
  "<form action='msg'><p>Type your message <input type='text' name='msg' size=100 autofocus> <input type='submit' value='Submit'></form>"
  "<button action='button'>Direct<type='scrolling' value='direct'></button>"
  "<button action='button'>Speed1<type='scrolling' value='speed1'></button>"
  "</center>";

ESP8266WebServer server(80);                             // HTTP server will listen at port 80
long period;
int offset = 1, refresh = 0;
int pinCS = 15; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 5;
int numberOfVerticalDisplays = 1;
String decodedMsg;
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

String tape = "Arduino";
int wait = 100; // In milliseconds
int spacer = 2;
int width = 5 + spacer; // The font width is 5 pixels

/*
  handles the messages coming from the webbrowser, restores a few special characters and
  constructs the strings that can be sent to the oled display
*/
void handle_msg() {

  matrix.fillScreen(LOW);
  server.send(200, "text/html", form);    // Send same page so they can send another msg
  refresh = 1;
  // Display msg on Oled
  String msg = server.arg("msg");
  Serial.println(msg);
  decodedMsg = msg;
  // Restore special characters that are misformed to %char by the client browser
  decodedMsg.replace("+", " ");
  decodedMsg.replace("%21", "!");
  decodedMsg.replace("%22", "");
  decodedMsg.replace("%23", "#");
  decodedMsg.replace("%24", "$");
  decodedMsg.replace("%25", "%");
  decodedMsg.replace("%26", "&");
  decodedMsg.replace("%27", "'");
  decodedMsg.replace("%28", "(");
  decodedMsg.replace("%29", ")");
  decodedMsg.replace("%2A", "*");
  decodedMsg.replace("%2B", "+");
  decodedMsg.replace("%2C", ",");
  decodedMsg.replace("%2F", "/");
  decodedMsg.replace("%3A", ":");
  decodedMsg.replace("%3B", ";");
  decodedMsg.replace("%3C", "<");
  decodedMsg.replace("%3D", "=");
  decodedMsg.replace("%3E", ">");
  decodedMsg.replace("%3F", "?");
  decodedMsg.replace("%40", "@");
  //Serial.println(decodedMsg);                   // print original string to monitor
  //Serial.println(' ');                          // new line in monitor
}

void setup(void) {
  matrix.setIntensity(15); // Use a value between 0 and 15 for brightness

  // matrix.setPosition(6, 0, 0); // The first display is at <0, 0>
  // matrix.setPosition(5, 1, 0); // The second display is at <1, 0>
  matrix.setPosition(4, 0, 0); // The third display is at <2, 0>
  matrix.setPosition(3, 1, 0); // And the last display is at <3, 0>
  matrix.setPosition(2, 2, 0); // And the last display is at <3, 0>
  matrix.setPosition(1, 3, 0); // And the last display is at <3, 0>
  matrix.setPosition(0, 4, 0); // And the last display is at <3, 0>

  //ESP.wdtDisable();                               // used to debug, disable wachdog timer,
  Serial.begin(115200);                           // full speed to monitor

  WiFi.begin(SSID, PASS);                         // Connect to WiFi network
  while (WiFi.status() != WL_CONNECTED) {         // Wait for connection
    delay(500);
    Serial.print(".");
  }

  // Set up the endpoints for HTTP server,  Endpoints can be written as inline functions:
  server.on("/", []() {
    server.send(200, "text/html", form);
  });
  server.on("/msg", handle_msg);                  // And as regular external functions:
  server.begin();                                 // Start the server


  Serial.print("SSID : ");                        // prints SSID in monitor
  Serial.println(SSID);                           // to monitor

  char result[16];
  sprintf(result, "%3d.%3d.%2d.%2d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  Serial.println();
  Serial.println(result);
  decodedMsg = result;
  Serial.println("WebServer ready!   ");

  Serial.println(WiFi.localIP());                 // Serial monitor prints localIP
  Serial.print(analogRead(A0));
}


void loop(void) {
  if (decodedMsg == "50")
  { wait = 50;
  }
  if (decodedMsg == "100")
  { wait = 100;
  }
  if (decodedMsg == "200")
  { wait = 200;
  }

  for ( int i = 0 ; i < width * decodedMsg.length() + matrix.width() - 1 - spacer; i++ ) {
    server.handleClient();                        // checks for incoming messages
    if (refresh == 1) i = 0;
    refresh = 0;
    matrix.fillScreen(LOW);

    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < decodedMsg.length() ) {
        matrix.drawChar(x, y, decodedMsg[letter], HIGH, LOW, 1);
      }

      letter--;
      x -= width;
    }

    matrix.write(); // Send bitmap to display

    delay(wait);
  }
}
