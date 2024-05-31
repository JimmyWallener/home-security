#include <Arduino.h>
#include "RealTimeClock.h"
#include "ESP32Comm.h"
#include "WifiManager.h"
#include "secrets.h"
#include <SPI.h>
#include "constants.h"
#include <Keypad.h>
#include <Http.h>

using namespace constants;

RealTimeClock realTimeClock;
ESP32Comm esp32Comm;
WifiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
Http http(&wifiManager);

Keypad keypad{Keypad(makeKeymap(KEYPAD_KEYS), KEYPAD_ROW_PINS, KEYPAD_COLS_PINS, KEYPAD_ROWS, KEYPAD_COLS)};


int lastMinute{-1}; // Last minute that was sent to Arduino UNO
int sendCounter{0};

void updateRealTimeClock();
void handleKeypad();

void setup() {
     Serial.begin(115200);
    Serial.println("################# Setup started #################");
    delay(5000);
    
    Serial.println("########### Initializing RealTimeClock ###########");
    realTimeClock.begin();
    Serial.println("##### RealTimeClock successfully initialized #####");
    delay(5000);

    Serial.println("############# Initializing ESP32Comm #############");
    esp32Comm.begin();
    Serial.println("####### ESP32Comm successfully initialized #######");
    delay(5000);

    Serial.println("########## Establishing WIFI connection ##########");
    wifiManager.connect();
    Serial.println("############### Connected to WiFi ###############");
    delay(5000);

    Serial.println("############### Syncing time with NTP ###############");
    http.syncTime();
    Serial.println("############### Time is synced ###############");
    delay(5000);

    Serial.println("############### Setup is completed ###############");
    delay(5000);
    keypad.setHoldTime(500);
}

void loop() {

    updateRealTimeClock();
    handleKeypad();
    while(sendCounter < 2){
        bool isValid = http.isPinCodeValid("1232131");
        if(isValid) {
            Serial.println("Pin code is valid");
    } else {
        Serial.println("Pin code is not valid");
    }
        sendCounter++;
        delay(1000);
    }
   
}




void updateRealTimeClock() {
    // Get current time from RTC and send it to Arduino UNO every minute
    
    DateTime now = realTimeClock.getCurrentTime();
    int currentMinute = now.minute();
    if (currentMinute != lastMinute) {
        lastMinute = currentMinute;
        esp32Comm.sendRtcData(realTimeClock);
        Serial.println("############ Sent RTC data ####################");
    }
}

void handleKeypad() {
    
    char key = keypad.getKey();
    if (key) {
        Serial.println(key);
        esp32Comm.sendKeypadData(key);
    }
}
