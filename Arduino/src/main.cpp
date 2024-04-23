#include <Arduino.h>
#include "../include/PIRSensor.h"

PIRSensor pirSensor(3, 1000);


void setup() {
  Serial.begin(9600);

 
}

void loop() {
  pirSensor.update();
  if (pirSensor.isTriggered()) {
    Serial.println(F("PIR Triggered!"));
  } else {
    Serial.println(F("PIR Not Triggered"));
  }
  delay(100);
}

