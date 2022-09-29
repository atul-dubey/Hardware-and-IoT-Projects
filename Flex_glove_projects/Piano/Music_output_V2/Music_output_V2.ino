 #include "pitches.h"
#include <SoftwareSerial.h>

//#include <Adafruit_NeoPixel.h>

//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(10, 12, NEO_GRB + NEO_KHZ800);
SoftwareSerial BTserial(4, 5); // RX | TX
// Connect the HC-05 TX to Arduino pin 2 RX. 
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.
 
char c = ' ';
char temp = 'o';
int speakerpin = 14;
int note1= 440;
int note2= 494;
int note3= 523;
int note4= 587;
int noteDuration= 120;
 
void setup() 
{
    // start the serial communication with the host computer
    Serial.begin(115200);
    Serial.println("Arduino with HC-05 is ready");
 
    // start communication with the HC-05 using 9600
    BTserial.begin(115200);  
    Serial.println("BTserial started at 115200");
//     pixels.begin();

 // myservo1.attach(14);
 // myservo2.attach(15);
  //myservo3.attach(12);
 // myservo4.attach(13);
}
 
void loop()
{
 
     // Keep reading from HC-05 and send to Arduino Serial Monitor
    if (BTserial.available())
    {  
        c = BTserial.read();
        Serial.print(c);
        //delay(10);
    }
 if(c=='1' || temp== '1')
{ melody1();
}
  if(c=='2' || temp== '2')
 {melody2();
 }
  if(c=='3' || temp== '3')
 { melody3();
 }
  if(c=='4' || temp== '4')
 { melody4();
 }
 if(c=='5')
 {
  melody5();
 }
 if(c=='6')
 {
  //myservo1.write(150);
  //myservo2.write(150);
  //myservo3.write(150);
  //myservo4.write(150);
  //temp='o';
 }
}
void melody1()
{
  int Pirates_note[] = {
NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,
NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, 
NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,  
NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4, 
NOTE_F4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_C4, NOTE_D4,
REST, NOTE_A3, NOTE_C4, NOTE_B3, NOTE_D4, NOTE_B3, NOTE_E4, NOTE_F4,
NOTE_F4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4,
NOTE_D4, REST, REST, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_F4,
NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_G4,
NOTE_A4, NOTE_D4, REST, NOTE_D4, NOTE_E3, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_A4, 
NOTE_D4, REST, NOTE_D4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_F4, NOTE_D4
};
int Pirates_duration[] = {
2,4,2,4,2,4,4,4,4,2,4,2,4,2,4,4,4,4,2,4,2,4,
2,4,4,4,4,2,2,4,4,2,2,4,4,2,2,4,4,
4,2,4,4,4,2,2,4,4,2,2,4,4,2,2,4,2,
2,4,4,4,4,2,2,4,4,2,2,4,4,2,2,4,4,
4,2,4,4,4,2,2,2,4,2,4,4,4,2,2,4,4
};

 for (int thisNote = 0; thisNote < 91; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 500 / Pirates_duration[thisNote];
    tone(speakerpin, Pirates_note[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speakerpin);
  }
}

void melody2()
{
  int underworld_melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, REST,
  REST,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, REST,
  REST,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, REST,
  REST,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, REST,
  REST, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  REST, REST, REST
};

int underworld_Notes[] ={
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};
  
  const int num_elements_in_arr = sizeof(underworld_Notes)/sizeof(underworld_Notes[0]);
 for (int thisNote = 0; thisNote < num_elements_in_arr; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 500/underworld_Notes[thisNote];
    tone(speakerpin, underworld_melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speakerpin);
  }
}

void melody3()
{
  int Mario_melody[] = {
  NOTE_E4, NOTE_E4, REST, NOTE_E4, 
  REST, NOTE_C4, NOTE_E4, REST,
  NOTE_G4, REST, REST, NOTE_G3, REST,
  
  NOTE_C4, REST, REST, NOTE_G3,
  REST, NOTE_E3, REST,
  REST, NOTE_A3, REST, NOTE_B3,   
  REST, NOTE_AS3, NOTE_A3, REST,
  
  NOTE_G3, NOTE_E4, NOTE_G4,
  NOTE_A4, REST, NOTE_F4, NOTE_G4, 
  REST, NOTE_E4, REST, NOTE_C4, 
  NOTE_D4, NOTE_B3, REST
};

int Mario_Notes[] = {
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 2, 4, 2, 2,
  
  4, 4, 4, 4,
  2, 4, 4,
  4, 4, 4, 4,  
  4, 4, 4, 4,
  
  4, 2, 4,
  4, 4, 4, 4,
  4, 4, 4, 4, 
  4, 4, 2
  };
  
  const int num_elements_in_arr = sizeof(Mario_Notes)/sizeof(Mario_Notes[0]);
 for (int thisNote = 0; thisNote < num_elements_in_arr; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 500/Mario_Notes[thisNote];
    tone(speakerpin, Mario_melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speakerpin);
  }
}

void melody4()
{
  int melody[] = {
  NOTE_C2, NOTE_F3, NOTE_C3, NOTE_A2,
  NOTE_C3, NOTE_F3, NOTE_C3,
  NOTE_C3, NOTE_F3, NOTE_C3, NOTE_F3,
  NOTE_AS3, NOTE_G3, NOTE_F3, NOTE_E3, NOTE_D3, NOTE_CS3,
  NOTE_C3, NOTE_F3, NOTE_C3, NOTE_A2, // the same again
  NOTE_C3, NOTE_F3, NOTE_C3,
  NOTE_AS3, REST, NOTE_G3, NOTE_F3,
  NOTE_E3, NOTE_D3, NOTE_CS3, NOTE_C3};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4,    4,    4,    4,
  4,    4,          2,
  4,    4,    4,    4,
  3,   8, 8, 8, 8, 8,
  4,    4,    4,    4, // the same again
  4,    4,          2,
  4, 8, 8,    4,    4,
  4,    4,    4,    4,
  0};

 for (int thisNote = 0; noteDurations[thisNote] != 0; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(speakerpin, melody[thisNote],noteDuration * 0.9);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    //int pauseBetweenNotes = noteDuration * 1.30;
    //delay(pauseBetweenNotes);
  delay(noteDuration);
  }
}

void melody5()
{
  int melody[] = {
NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_A4,
NOTE_G4, NOTE_C5, NOTE_AS4, NOTE_A4,                   
NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
NOTE_C4, NOTE_D4,REST,                                 

NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_A4,
NOTE_G4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4,      //29               //8
NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
NOTE_C4, NOTE_D4,REST,                                       

NOTE_D4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_DS5, NOTE_D5,
NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5,
NOTE_C4, NOTE_D4, NOTE_DS4, NOTE_FS4, NOTE_D5, NOTE_C5,
NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_AS4,             //58

NOTE_D4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_DS5, NOTE_D5,
NOTE_C5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_G4,
NOTE_A4, REST, NOTE_AS4, NOTE_A4, REST, NOTE_G4,
NOTE_G4, NOTE_A4, NOTE_G4, NOTE_FS4, REST,

NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_DS4,
NOTE_C4, NOTE_D4, REST,
NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_DS4,
NOTE_C4, NOTE_D4

};

// note durations: 8 = quarter note, 4 = 8th note, etc.
int noteDurations[] = {       //duration of the notes
8,4,8,4,
4,4,4,12,
4,4,4,4,4,4,
4,16,4,

8,4,8,4,
4,2,1,1,2,1,1,12,
4,4,4,4,4,4,
4,16,4,

4,4,4,4,4,4,
4,4,4,12,
4,4,4,4,4,4,
4,4,4,12,

4,4,4,4,4,4,
2,1,1,2,1,1,4,8,4,
2,6,4,2,6,4,
2,1,1,16,4,

4,8,4,4,4,
4,16,4,
4,8,4,4,4,
4,20,0
};

for (int thisNote = 0; noteDurations[thisNote] != 0; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 90*noteDurations[thisNote];
    tone(speakerpin, melody[thisNote],noteDuration * 0.95);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    //int pauseBetweenNotes = noteDuration * 1.30;
    //delay(pauseBetweenNotes);
  delay(noteDuration);
  //noTone(3);
  }
}

