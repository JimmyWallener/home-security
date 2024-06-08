#include <Arduino.h>
#include "RealTimeClock.h"
#include "ESP32Comm.h"
#include "WifiManager.h"
#include "secrets.h"
#include <SPI.h>
#include "constants.h"
#include <Keypad.h>
#include <HttpsRequest.h>
#include "AccessLog.h"


using namespace constants;

// classes
RealTimeClock realTimeClock;
ESP32Comm esp32Comm;
WifiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
HttpsRequest httpsRequest(&wifiManager);

// Keypad
Keypad keypad{Keypad(makeKeymap(KEYPAD_KEYS), KEYPAD_ROW_PINS, KEYPAD_COLS_PINS, KEYPAD_ROWS, KEYPAD_COLS)};

int lastMinute{-1}; // Last minute that was sent to Arduino UNO
int loginAttempts{3};
char pinCode[4]{'\0', '\0', '\0', '\0'};
bool alarmActive{false};
char initKey{'#'};

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
    httpsRequest.syncTime();
    Serial.println("############### Time is synced ###############");
    delay(5000);

    

    Serial.println("############### Setup is completed ###############");
    delay(5000);
    keypad.setHoldTime(500);
}

void loop() {
    char key = keypad.getKey();
    updateRealTimeClock();
   // esp32Comm.requestDataFromPeripheral(httpsRequest);

    if (key == initKey) {
        handleKeypad();
        while(loginAttempts > 0){
        
        // check if pincode has been entered
        if(pinCode[0] != '\0' && pinCode[1] != '\0' && pinCode[2] != '\0' && pinCode[3] != '\0'){
            bool isValid = httpsRequest.isPinCodeValid(String(pinCode));
            if(isValid) {
                esp32Comm.sendPinCodeFeedback(true, loginAttempts);
                esp32Comm.sendAlarmActivationChange(alarmActive);
                alarmActive = !alarmActive;
                loginAttempts = 3;
                for (int i = 0; i < 4; i++) {
                pinCode[i] = '\0';
            }
            } else {
            esp32Comm.sendPinCodeFeedback(false, loginAttempts);
            loginAttempts--;
            if(loginAttempts == 1) {
                esp32Comm.sendTriggerEvent("Alarm");
            }
            for (int i = 0; i < 4; i++) {
                pinCode[i] = '\0';
            }
    }
    
    }
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
    
    while(pinCode[0] == '\0' || pinCode[1] == '\0' || pinCode[2] == '\0' || pinCode[3] == '\0') {
        char key = keypad.getKey();
        if (key) {
            if(key == 'D'){
                esp32Comm.sendKeypadData(key);
                // delete last entry in char array
                for (int i = 3; i >= 0; i--) {
                    if (pinCode[i] != '\0') {
                        pinCode[i] = '\0';
                        break;
                    }
                }
                break;
            }
            if(key == 'C'){
                esp32Comm.sendKeypadData(key);
                // delete all entries in char array
                for (int i = 0; i < 4; i++) {
                    pinCode[i] = '\0';
                }
                break;
            }
            esp32Comm.sendKeypadData(key);
            for (int i = 0; i < 4; i++) {
                if (pinCode[i] == '\0') {
                    pinCode[i] = key;
                    break;
                }
            }
        }
    }
}
