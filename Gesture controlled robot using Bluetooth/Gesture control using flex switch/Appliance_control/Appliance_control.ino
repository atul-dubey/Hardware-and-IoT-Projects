/****************************************************
   Author: Atul Dubey
   Date: 05/10/2022
   Description:
      This code will help you in controlling GPIOs by
      sending commands from Serial monitor
 ****************************************************/

int fan = 13;
int bulb = 14;
int state;
int flag = 0;

void setup() {
  Serial.begin(115200);
  pinMode(fan, OUTPUT);
  pinMode(bulb, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    state = Serial.read();
    flag = 0;
  }

  if (state == '1') {
    Serial.println(state);
    digitalWrite(fan, HIGH);
    if (flag == 0) {
      Serial.println("Go Forward!");
      flag = 1;
    }
  }

  else if (state == '2') {
    Serial.println(state);
    if (flag == 0) {
      Serial.println("Go Forward!");
      flag = 1;
    }
  }

  else if (state == '3') {
    Serial.println(state);
    if (flag == 0) {
      Serial.println("Go Forward!");
      flag = 1;
    }
  }

  else if (state == '4') {
    Serial.println(state);
    if (flag == 0) {
      Serial.println("Go Forward!");
      flag = 1;
    }
  }

  else if (state == '5') {
    Serial.println(state);
    if (flag == 0) {
      Serial.println("Go Forward!");
      flag = 1;
    }
  }
}
