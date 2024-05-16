#include "ESP32Comm.h"

ESP32Comm::ESP32Comm() {}

void ESP32Comm::begin() {
    Wire.begin();
}

void ESP32Comm::sendJsonData(const JsonDocument &doc) {
    String jsonString;
    serializeJson(doc, jsonString);
    Wire.beginTransmission(8); // Assuming Arduino UNO is at address 8
    Wire.write('J');
    Wire.write(jsonString.c_str());
    Wire.endTransmission();
}

void ESP32Comm::sendRtcData(const DateTime &now) {
    Wire.beginTransmission(8);
    Wire.write('R');
    Wire.write(now.year() - 2000); // year
    Wire.write(now.month());       // month
    Wire.write(now.day());         // day
    Wire.write(now.hour());        // hours
    Wire.write(now.minute());      // minutes
    Wire.write(now.second());      // seconds
    Wire.endTransmission();
}

void ESP32Comm::sendTriggerEvent(const char *sensor) {
    Wire.beginTransmission(8);
    Wire.write('T');
    Wire.write(sensor);
    Wire.endTransmission();
}

void ESP32Comm::sendAlarmActivation() {
    Wire.beginTransmission(8);
    Wire.write('A');
    Wire.endTransmission();
}

void ESP32Comm::sendAlarmDeactivation() {
    Wire.beginTransmission(8);
    Wire.write('D');
    Wire.endTransmission();
}

void ESP32Comm::sendPinCodeFeedback(bool success, int attemptsLeft) {
    Wire.beginTransmission(8);
    Wire.write('P');
    Wire.write(success);
    Wire.write(attemptsLeft);
    Wire.endTransmission();
}

void ESP32Comm::sendAlarmStatusRequest() {
    Wire.beginTransmission(8);
    Wire.write('S');
    Wire.endTransmission();
}