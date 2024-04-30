#include <Arduino.h>
#include <SPI.h>
#include "Rtc.h"

RTC rtc;

void setup() {
  // put your setup code here, to run once:
  while (!Serial);
  rtc.RTCInitialize();
}

void loop() {
  // put your main code here, to run repeatedly:
  rtc.RTCUpdate();
}