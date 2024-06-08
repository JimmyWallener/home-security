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
int loginAttempts{2};
char pinCode[5] {0}; // Make sure it's 5 to accommodate the null terminator
int pinIndex{0};

bool alarmActive{false};
bool isEnteringPin{false};


void updateRealTimeClock();
void handleKeypad(char key);

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
    esp32Comm.sendRtcData(realTimeClock);
    

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

    if (key == '#') {
        isEnteringPin = true;
        pinIndex = 0;
        memset(pinCode, '\0', sizeof(pinCode)); // Clear the pin code buffer
        Serial.println("Enter PIN:");
    }

    if (isEnteringPin) {
        handleKeypad(key);
    }
}

void updateRealTimeClock() {
    DateTime now = realTimeClock.getCurrentTime();
    int currentMinute = now.minute();
    if (currentMinute != lastMinute) {
        lastMinute = currentMinute;
        esp32Comm.sendRtcData(realTimeClock);
        Serial.println("############ Sent RTC data ####################");
    }
}

unsigned long lastKeyPressTime = 0; // Last key press time
const unsigned long timeoutDuration = 5000; // Timeout duration in milliseconds

void handleKeypad(char key) {
    unsigned long currentTime = millis(); // get the current time

    // check if the timeout has occurred
    if (currentTime - lastKeyPressTime >= timeoutDuration && pinIndex > 0) {
        // Timeout has occurred so reset the pin code input
        memset(pinCode, '\0', sizeof(pinCode)); // Clear the pin code buffer
        pinIndex = 0;
        isEnteringPin = false;
        Serial.println("Timeout! Pin code input reset.");
        return;
    }

    if (key) {
        lastKeyPressTime = currentTime; // Update the last key press time
        esp32Comm.sendKeypadData(key);

        if (key == 'D') {
            if (pinIndex > 0) {
                pinCode[--pinIndex] = '\0'; // Remove the last digit
            }
            Serial.print("Pin after delete: ");
            Serial.println(pinCode);
        } else if (key == 'C') {
            memset(pinCode, '\0', sizeof(pinCode)); // Clear the pin code buffer
            pinIndex = 0;
            Serial.println("Pin code cleared.");
        } else if (pinIndex < 4 && isdigit(key)) { // Allowing only digits
            pinCode[pinIndex++] = key;
            Serial.print("Pin so far: ");
            Serial.println(pinCode);

            if (pinIndex == 4) {
                // Check the pin code
                Serial.print("Checking pin code: ");
                Serial.println(pinCode);

                bool isValid = httpsRequest.isPinCodeValid(String(pinCode));
                if (isValid) {
                    esp32Comm.sendPinCodeFeedback(true, loginAttempts);
                    alarmActive = !alarmActive;
                    esp32Comm.sendAlarmActivationChange(alarmActive);
                    Serial.println("Alarm state changed.");
                } else {
                    esp32Comm.sendPinCodeFeedback(false, loginAttempts);
                    loginAttempts--;

                    if (loginAttempts == 0) {
                        Serial.println("No attempts left, resetting.");
                        loginAttempts = 2;
                    }
                }
                memset(pinCode, '\0', sizeof(pinCode)); // Clear the pin code buffer
                pinIndex = 0;
                isEnteringPin = false;
            }
        }
    }
}
