#include <Arduino.h>
#include <SPI.h>
#include "Rtc.h"

RTC rtc;

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  // Serial.begin(9600);
  rtc.RTCSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  rtc.RTCLoop();
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}