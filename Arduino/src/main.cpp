#include <Arduino.h>
#include "Rtc.h"
#include <SPI.h>

RTC rtc;

void setup() {
  // put your setup code here, to run once:
  while (!Serial);
  rtc.RTCSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  rtc.RTCLoop();
}