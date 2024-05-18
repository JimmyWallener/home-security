#include "ESP32Comm.h"
#include "Global.h"

ESP32Comm* ESP32Comm::instance = nullptr;

ESP32Comm::ESP32Comm() {
    this->instance = this;
}

void ESP32Comm::begin() {
    Wire.begin();
    Wire.onReceive(onReceiveWrapper);
}

void ESP32Comm::sendJsonData(const JsonDocument &doc) {
    String jsonString;
    serializeJson(doc, jsonString);
    Wire.beginTransmission(ARDUINO_I2C_ADDRESS); // Assuming Arduino UNO is at address 8
    Wire.write('J');
    Wire.write(jsonString.c_str());
    Wire.endTransmission();
}

void ESP32Comm::sendRtcData(const DateTime &now) {
    Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
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
    Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
    Wire.write('T');
    Wire.write(sensor);
    Wire.endTransmission();
}

void ESP32Comm::sendAlarmActivation() {
    Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
    Wire.write('A');
    Wire.endTransmission();
}

void ESP32Comm::sendAlarmDeactivation() {
    Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
    Wire.write('D');
    Wire.endTransmission();
}

void ESP32Comm::sendPinCodeFeedback(bool success, int attemptsLeft) {
    Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
    Wire.write('P');
    Wire.write(success);
    Wire.write(attemptsLeft);
    Wire.endTransmission();
}

void ESP32Comm::sendAlarmStatusRequest() {
    Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
    Wire.write('S');
    Wire.endTransmission();
}

void ESP32Comm::onReceive(int numBytes) {
    // TODO: Implement onReceive method for Arduino UNO to ESP32 communication

}

void ESP32Comm::onReceiveWrapper(int numBytes) {
    if (instance != nullptr) {
        instance->onReceive(numBytes);
    }
}