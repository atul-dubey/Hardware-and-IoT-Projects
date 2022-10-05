#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(-1);

const int up = 5;  // This is the Move Up Button
const int down = 4; // move down Button;
const int buzzer = 2; // Buzzer(makes the sound)
int upRead;
int downRead;
int positon = 0;  // first position for the racket
int positon2 = 2; // second positon for the racket.
int score = 0;
bool goBack = false;
int x = 0;  // ball x positon
long y = 2; // ball y positon
bool yChange = true;
int goStraightRandom = 1;
bool goStraight = true;

void setup()   {
  pinMode(buzzer, OUTPUT);
  noTone(buzzer);
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(7,10);
  display.println("PONG V1.5");
  display.display();
  noTone(buzzer);
  display.setTextSize(1);
  delay(2000);
  display.clearDisplay();
  tone(buzzer, 800);
  delay(30);
  noTone(buzzer);
}


void loop() {

  int upRead = digitalRead(up);
  int downRead = digitalRead(down);

  goStraight = goStraightRandom == 1 || goStraightRandom == 4;
  
  if(y == 0 && goStraight == false)
  {
    yChange = true;
    tone(buzzer, 800);
    delay(5);
    noTone(buzzer);
  }
  if(y == 23 && goStraight == false)
  {
    yChange = false;
    tone(buzzer, 800);
    delay(5);
    noTone(buzzer);
  }
  if(yChange == true && goStraight == false)
  {
    y++;
  }
  if(yChange == false && goStraight == false)
  {
    y--;
  }
  
  display.clearDisplay();
  if(upRead == HIGH)
  {
    positon = positon+1;
    positon2 = positon2+1;
  }
  if(downRead == HIGH)
  {
    positon = positon - 1;
    positon2 = positon2-1;
  }
  if(positon > 23)
  {
    positon = 23;
    positon2 = 25;
  }
    if(positon < 0)
  {
    positon = 0;
    positon2 = 2;
  }
  display.setCursor(1,positon);
  display.println(".");
  display.display();
    display.setCursor(1,positon2);
  display.println(".");
  display.display();
  
  if(goBack == false)
  {
    goBack = false;
    display.setCursor(x,y);
    display.println(".");
    display.display();
    delay(5);
    if(x == 118)
    {
      goBack = true;
      tone(buzzer, 800);
      delay(5);
      noTone(buzzer);
      goStraightRandom = random(5);
    }
    x = x+2;
  }
  if(goBack == true)
  {
    goBack = true;
    display.setCursor(x,y);
    display.println(".");
    display.display();
    delay(5);  
    x = x-2;

    if(x == 0)
    {
      goStraightRandom = random(5);
      goBack = false;
      tone(buzzer, 800);
      delay(5);
      noTone(buzzer);
    }
  }
  if(x == 0 && ((y!=positon) && (y!=positon2)&& (y!=positon-1) && (y!=positon2-1) && (y!=positon+1) && (y!=positon2+1))) // checks if the player has missed the ball
  {
       display.setCursor(35,6);
       display.println("Game over");
       display.setCursor(35,16);
       display.print("Score: ");
       display.println(score);
       display.display();
       score = 0;
       delay(3000);
  }
  if(x == 0 && ((y==positon) || (y==positon2) || (y==positon-1) || (y==positon2-1) || (y==positon+1) || (y==positon2+1)))
  {
    score ++;
    tone(buzzer, 800);
    delay(20);
    noTone(buzzer);
  }
  Serial.println(positon);
}
