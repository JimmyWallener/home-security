#include <Arduino.h>
#include "Numpad.h"
#include "RealTimeClock.h"
#include "ESP32Comm.h"
#include "WiFiConnection.h"
#include "secrets.h"
#include <SPI.h>



RealTimeClock realTimeClock;
ESP32Comm esp32Comm;
WiFiConnection wifi;

int lastMinute = -1; // Last minute that was sent to Arduino UNO
int sendCounter = 0;

void setup() {
    Serial.begin(115200);

    // Initialize components
    
    
    realTimeClock.begin();
    esp32Comm.begin();
    wifi.connect(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
    // Get current time from RTC and send it to Arduino UNO every minute
    DateTime now = realTimeClock.getCurrentTime();
    int currentMinute = now.minute();
    if (currentMinute != lastMinute) {
        lastMinute = currentMinute;
        esp32Comm.sendRtcData(now);
    }
    // For testing if payload is sent and displayed on the LCD
    if(sendCounter < 10){
        esp32Comm.sendAlarmActivation();
        sendCounter++;
    }
    
    
    delay(1000);
}


