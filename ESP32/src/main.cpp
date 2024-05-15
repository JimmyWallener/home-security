#include <Arduino.h>
#include "Numpad.h"
#include "RealTimeClock.h"
#include "ESP32Comm.h"
#include "WiFiConnection.h"
#include "secrets.h"
#include <SPI.h>


RealTimeClock rtc;
ESP32Comm esp32Comm;
WiFiConnection wifi;

void setup() {
    Serial.begin(115200);

    // Initialize components
    
    
    rtc.begin();
    esp32Comm.begin();
    wifi.connect(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
    

    // Get current time from RTC and send to Arduino UNO
    DateTime now = rtc.getCurrentTime();
    esp32Comm.sendRtcData(now);

    delay(1000); // Delay for 1 second
}

