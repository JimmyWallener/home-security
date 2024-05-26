#include <Arduino.h>
#include "Numpad.h"
#include "RealTimeClock.h"
#include "ESP32Comm.h"
#include "WiFiConnection.h"
#include "secrets.h"
#include <SPI.h>
#include "constants.h"

using namespace constants;

RealTimeClock realTimeClock;
ESP32Comm esp32Comm;
WiFiConnection wifi(WIFI_SSID, WIFI_PASSWORD);

int lastMinute = -1; // Last minute that was sent to Arduino UNO
int sendCounter = 0;

void updateRealTimeClock();



void setup() {
     Serial.begin(115200);
    Serial.println("Setup started.");
    delay(5000);
    
    Serial.println("Initializing RealTimeClock");
    realTimeClock.begin();
    Serial.println("RealTimeClock initialized");
    delay(5000);

    Serial.println("Initializing ESP32Comm");
    esp32Comm.begin();
    Serial.println("ESP32Comm initialized");
    delay(5000);

    Serial.println("Connecting to WiFi...");
    wifi.connect();
    Serial.println("Connected to WiFi");
    delay(5000);

    Serial.println("Setup completed.");
    delay(5000);
}

void loop() {

    updateRealTimeClock();

    delay(1000);
}




void updateRealTimeClock() {
    // Get current time from RTC and send it to Arduino UNO every minute
    DateTime now = realTimeClock.getCurrentTime();
    int currentMinute = now.minute();
    if (currentMinute != lastMinute) {
        lastMinute = currentMinute;
        esp32Comm.sendRtcData(realTimeClock);
        Serial.println("Sent RTC data");
    }
}



 
