#include <Arduino.h>
#include <LiquidCrystal.h>
#include "Rtc.h"
#include <RTClib.h>
#include <SPI.h>

RTC rtc;

void setup() {
  // put your setup code here, to run once:
  rtc.RTCSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  rtc.RTCLoop();
}