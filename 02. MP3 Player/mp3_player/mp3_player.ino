/*
 * vol --- A3
 * prev -- D7
 * paus -- D6
 * next -- D5
 * VCC --- 5V
 * RX  --- D10
 * TX  --- D11
 * GND --- GND
 */

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(11, 10); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int vol;
int previ;
int pause;
int next;
bool change = true;

void setup()
{
  pinMode(7, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(A3, INPUT);
  mySoftwareSerial.begin(9600);
  //Serial.begin(9600);
  //Serial.println();
  //Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    //Serial.println(F("Unable to begin:"));
    //Serial.println(F("1.Please recheck the connection!"));
    //Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  //Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(0);
  myDFPlayer.enableLoopAll();
  vol = map(analogRead(A3), 0, 1023, 0, 20);
  myDFPlayer.volume(vol);  //Set volume value. From 0 to 30
  myDFPlayer.start();  //Play the first mp3
}

void loop()
{
  previ = digitalRead(7);
  pause = digitalRead(6);
  next = digitalRead(5);
  vol = map(analogRead(A3), 0, 1023, 0, 20);
  if (next == 0) {
    while ((next = digitalRead(5)) == 0) {}
    myDFPlayer.next();
  }
  if (previ == 0) {
    while ((previ = digitalRead(7)) == 0) {}
    myDFPlayer.previous();
  }
  if (pause == 0) {
    while ((pause = digitalRead(6)) == 0) {}
    if (change) {
      myDFPlayer.pause();
    }
    else {
      myDFPlayer.start();
    }
    change = !change;
  }
  /*
  Serial.print(previ);
  Serial.print(", ");
  Serial.print(pause);
  Serial.print(", ");
  Serial.print(next);
  Serial.print(", ");
  Serial.println(vol);
  */
  myDFPlayer.volume(vol);
  delay(100);
}
