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

int lastMinute = -1; // Last minute that was sent to Arduino UNO

void setup() {
    Serial.begin(115200);

    // Initialize components
    
    
    rtc.begin();
    esp32Comm.begin();
    wifi.connect(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
    // Get current time from RTC and send it to Arduino UNO every minute
    DateTime now = rtc.getCurrentTime();
    int currentMinute = now.minute();
    if (currentMinute != lastMinute) {
        lastMinute = currentMinute;
        esp32Comm.sendRtcData(now);
    }

    
    delay(1000);
}


