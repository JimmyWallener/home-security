#include "ESP32Comm.h"
#include "constants.h"

using namespace constants;

ESP32Comm::ESP32Comm(){};

void ESP32Comm::begin() {
  // ESP32 I2C Master initialization
  Wire.begin();
}

void ESP32Comm::sendRtcData(RealTimeClock &realTimeClock) {
  Serial.println("RealTimeClock transmission started");
  Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
  Wire.write('R');
  Wire.write(realTimeClock.getFormattedTime().c_str());
  byte status = Wire.endTransmission();

  if (status == 0) {
    Serial.println("RealTimeClock transmission successful");
  } else {
    Serial.println("RealTimeClock transmission failed");
  }
}

void ESP32Comm::sendTriggerEvent(const char *sensor) {
  Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
  Wire.write('T');
  Wire.write(sensor);
  Wire.endTransmission();
}

void ESP32Comm::sendAlarmActivationChange(bool isActive) {
  Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
  Wire.write((isActive ? 'A' : 'D'));
  Wire.endTransmission();
}

// this method is simply for lcd-displaying purposes
void ESP32Comm::sendPinCodeFeedback(bool success, int attemptsLeft) {
  Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
  Wire.write('P');
  Wire.write((success ? 1 : 0));
  Wire.write(attemptsLeft);
  Wire.endTransmission();
}

void ESP32Comm::sendKeypadData(char key) {
  Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
  Wire.write('K');
  Wire.write(key);
  Wire.endTransmission();
}

JsonDocument ESP32Comm::requestDataFromPeripheral() {

  String message{};
  JsonDocument doc;

  Wire.requestFrom(ARDUINO_I2C_ADDRESS, (uint8_t)16);
  while (Wire.available()) {
    char c = Wire.read();
    message += c;
  }
  deserializeJson(doc, message);
  return doc;
}
