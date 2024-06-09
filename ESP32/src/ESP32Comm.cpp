#include "ESP32Comm.h"
#include "constants.h"

using namespace constants;

ESP32Comm::ESP32Comm(){};

void ESP32Comm::begin() {
  // ESP32 I2C Master initialization
  Wire.begin();
}

/// @brief 
/// @param realTimeClock 
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

/// @brief 
/// @param sensor 
void ESP32Comm::sendTriggerEvent(const char *sensor) {
  Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
  Wire.write('T');
  Wire.write(sensor);
  Wire.endTransmission();
}

/// @brief 
/// @param isActive 
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

struct SensorData {
  String id;
  String type;
  String timestamp;
  String sensorType;
  String sensorId;
  bool sensorValue;

   String toJson() const {
    JsonDocument doc;
    doc["id"] = generateUUID();
    doc["type"] = "sensor_log";
    doc["timestamp"] = timestamp;
    doc["sensorType"] = sensorType;
    doc["sensorId"] = sensorId;
    doc["sensorValue"] = sensorValue;
    String output;
    serializeJson(doc, output);
    return output;
  }
};

void ESP32Comm::requestDataFromPeripheral(HttpsRequest& httpsRequest) {
    SensorData sensorData;
    String receivedData = "";

    Wire.requestFrom(ARDUINO_I2C_ADDRESS, (uint8_t)32); // Request up to 32 bytes

    while (Wire.available()) {
        char c = Wire.read();
        if (c == '\0') break; // Sluta läsa vid null-terminator
        receivedData += c;
    }

    Serial.println("Received data: " + receivedData);

    // Dela upp strängen i sina komponenter
    int firstSep = receivedData.indexOf('|');
    int secondSep = receivedData.indexOf('|', firstSep + 1);
    int thirdSep = receivedData.indexOf('|', secondSep + 1);

    sensorData.timestamp = receivedData.substring(0, firstSep);
    sensorData.sensorType = receivedData.substring(firstSep + 1, secondSep);
    sensorData.sensorId = receivedData.substring(secondSep + 1, thirdSep);
    sensorData.sensorValue = receivedData.substring(thirdSep + 1) == "1";

    // Kontrollera om all data har tagits emot
    if (!sensorData.timestamp.isEmpty() && !sensorData.sensorType.isEmpty() && !sensorData.sensorId.isEmpty()) {
        httpsRequest.sendSensorLogToCosmo(sensorData.toJson());
        Serial.println("Sensor data sent to Cosmo");
    } else {
        Serial.println("Failed to parse received data.");
    }
}



