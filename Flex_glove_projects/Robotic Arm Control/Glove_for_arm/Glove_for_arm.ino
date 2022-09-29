/*************************************************
   Author: Atul Dubey
   Date: 28/09/2022
   Description:
    This code allows you to build a flex gove to control
    RObotic arm
 *************************************************/
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
  //Serial.println(d1);
  //Serial.println(d2);
  //Serial.println(d3);
  //Serial.println(d4);
  //Serial.println(d5);
  if (d1 > 700)
  {
    newtemp = 1;
    // Serial.print('1');
  }
  else if (d2 > 800)
  {
    newtemp = 2;
    //Serial.print('2');
  }
  else if (d3 > 800)
  {
    newtemp = 3;
    //Serial.print('3');
  }
  else if (d4 > 700)
  {
    newtemp = 4;
    //Serial.print('4');
  }
  else if (d5 == 1)
  {
    newtemp = 5;
    // Serial.print('5');
  }
  else
  {
    newtemp = 6;
  }
  delay(100);
  if (newtemp != oldtemp)
  {
    if (newtemp == 1)
    {
      Serial.print('1');

    }
    if (newtemp == 2)
    {

      Serial.print('2');
    }

    if (newtemp == 3)
    {
      Serial.print('3');

    }

    if (newtemp == 4)
    {
      Serial.print('4');

    }

    if (newtemp == 5)
    {
      Serial.print('5');

    }
    if (newtemp == 6)
    {
      Serial.print('6');

    }
    oldtemp = newtemp;
    //Serial.println(newtemp);
  }
}
