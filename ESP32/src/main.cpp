#include "WiFiConnection.h"
#include <Arduino.h>

WiFiConnection wifi;

void setup() {
  Serial.begin(115200);
  wifi.initWiFi();

}

void loop() {
  
}

