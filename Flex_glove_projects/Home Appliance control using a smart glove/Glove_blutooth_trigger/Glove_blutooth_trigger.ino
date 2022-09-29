/************************************************
   Author: Atul Dubey
   Date: 28/09/2022
   Description:
    This code will help you send the trigger over Bluetooth
    to control the appliances
 ***********************************************/

int flex1 = A1;
int flex2 = A2;
int flex3 = A3;
int flex4 = A4;
int button = 10;
int d1, d2, d3, d4, d5;
int oldtemp = 0;
int newtemp = 0;

void setup() {
  pinMode(flex1, INPUT);
  pinMode(flex2, INPUT);
  pinMode(flex3, INPUT);
  pinMode(flex4, INPUT);
  pinMode(button, INPUT);
  Serial.begin(115200);
}

void loop() {
  d1 = analogRead(flex1); // threshold 900
  d2 = analogRead(flex2); //------
  d3 = analogRead(flex3); //--------
  d4 = analogRead(flex4); //-----------
  d5 = digitalRead(button); //----0 or 1------

  if (d1 > 900)
  {
    Serial.print('1');
  }
  else if (d2 > 900)
  {
    Serial.print('2');
  }
  else if (d3 > 900)
  {
    Serial.print('3');
  }
  else if (d4 > 900)
  {
    Serial.print('4');
  }
  else if (d5 == 1)
  {
    Serial.print('5');
  }
}
