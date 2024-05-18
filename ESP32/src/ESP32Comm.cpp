#include "ESP32Comm.h"
#include "Global.h"

ESP32Comm* ESP32Comm::instance = nullptr;

ESP32Comm::ESP32Comm() {
    this->instance = this;
}

void ESP32Comm::begin() {
    // ESP32 I2C Master initialization
    Wire.begin();
    
    // ESP32 I2C Slave initialization
    // TODO: Need to be tested on receieving data from Arduino UNO. NOT TESTED YET. But atleast it's not breaking the master -> slave communication
    WireSlave.begin(21, 22, ESP32_I2C_ADDRESS); // Join I2C bus with address #8
    
    Wire.onReceive(onReceiveWrapper);
    
}

// This method is a just in case method, it can be removed if not needed
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

// this method is simply for lcd-displaying purposes
void ESP32Comm::sendPinCodeFeedback(bool success, int attemptsLeft) {
    Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
    Wire.write('P');
    Wire.write(success);
    Wire.write(attemptsLeft);
    Wire.endTransmission();
}

// Had a thought about this method, but unsure now. It can be removed if not needed
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