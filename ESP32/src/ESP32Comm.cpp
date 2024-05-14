#include "ESP32Comm.h"

ESP32Comm::ESP32Comm(int sdaPin, int sclPin) : sdaPin(sdaPin), sclPin(sclPin) {}

void ESP32Comm::begin() {
    Wire.begin(sdaPin, sclPin);
}

void ESP32Comm::sendJsonData(const JsonDocument &doc) {
    String jsonString;
    serializeJson(doc, jsonString);
    Wire.beginTransmission(8); // Assuming Arduino UNO is at address 8
    Wire.write(jsonString.c_str());
    Wire.endTransmission();
}

void ESP32Comm::sendRtcData(const DateTime &now) {
    Wire.beginTransmission(8); 
    Wire.write(now.year() - 2000); // year
    Wire.write(now.month());       // month
    Wire.write(now.day());         // day
    Wire.write(now.hour());        // hours
    Wire.write(now.minute());      // minutes
    Wire.write(now.second());      // seconds
    Wire.endTransmission();
}
