// #include "WiFiConnection.h"
#include <Arduino.h>
#include <Keypad.h>
#include <Wire.h>
#include "../include/AlarmSystem.h"
#include "../include/Numpad.h"
// This is a MASTER unit

// WiFiConnection wifi;

AlarmSystem alarmsystem;
Keypad numpad = initializeKeypad();

void setup() {
  Serial.begin(115200);
  Wire.begin();
  //wifi.initWiFi();
}

// Detta kan också snyggas till senare.
void loop() {
  if(numpad.getKey() == '#') {
    Serial.println("Enter Password: "); // This will be on the lcd display later when connected. 
    alarmsystem.getPassword(numpad);
    alarmsystem.changeAlarmState();
  }
  if(alarmsystem.getState() == true) {
    Wire.beginTransmission(4);
    Wire.write(1);
    Wire.endTransmission();
  }
  if(alarmsystem.getState() == false) {
    Wire.beginTransmission(4);
    Wire.write(0);
    Wire.endTransmission();
  }
}