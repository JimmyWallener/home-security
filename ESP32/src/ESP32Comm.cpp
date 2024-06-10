#include "ESP32Comm.h"
#include "constants.h"
#include "time.h"

using namespace constants;

ESP32Comm::ESP32Comm(){};

/// @brief Initializes the I2C communication as a master.
/// This function sets up the ESP32 as an I2C master device.
void ESP32Comm::begin() {
  // ESP32 I2C Master initialization
  Wire.begin();
}

/// @brief Sends the formatted real-time clock data to the Arduino UNO.
/// @param realTimeClock An instance of RealTimeClock containing the current time.
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

/// @brief Sends the alarm activation status to the Arduino UNO.
/// @param isActive A boolean indicating whether the alarm is active (true) or deactivated (false).
void ESP32Comm::sendAlarmActivationChange(bool isActive) {
  Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
  Wire.write((isActive ? 'A' : 'D'));
  Wire.endTransmission();
}

/// @brief Sends feedback about the pin code entry status to the Arduino UNO.
/// This is mainly for LCD display purposes.
/// @param success A boolean indicating if the pin code entry was successful (true) or not (false).
/// @param attemptsLeft The number of remaining attempts left for pin code entry.
void ESP32Comm::sendPinCodeFeedback(bool success, int attemptsLeft) {
  Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
  Wire.write('P');
  Wire.write((success ? 1 : 0));
  Wire.write(attemptsLeft);
  Wire.endTransmission();
}

/// @brief Sends a single keypad character entry to the Arduino UNO.
/// @param key The character key that was pressed on the keypad.
void ESP32Comm::sendKeypadData(char key) {
  Wire.beginTransmission(ARDUINO_I2C_ADDRESS);
  Wire.write('K');
  Wire.write(key);
  Wire.endTransmission();
}

/// @brief Gets the current local time formatted as a string.
/// @return A string representing the current time in the format "YYYY-MM-DD HH:MM:SS".
String getFormattedTime() {
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(buffer);
}

/// @brief Structure to store and convert sensor data to JSON format.
struct SensorData {
  String id; ///< Unique identifier for the sensor data.
  String type; ///< Type of the data, usually "sensor_log".
  String timestamp; ///< The timestamp of when the data was recorded.
  String sensorType; ///< The type of sensor that generated the data.
  String sensorId; ///< The identifier of the sensor.
  bool sensorValue; ///< The value recorded by the sensor (true or false).

  /// @brief Converts the sensor data to a JSON string.
  /// @return A JSON string representation of the sensor data.
  String toJson() const {
    StaticJsonDocument<200> doc;  // Fixed size to avoid dynamic allocation issues
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

/// @brief Requests data from the Arduino UNO and processes it.
/// This function reads data from the Arduino UNO, identifies the sensor type, and creates a sensor log which is then sent to Cosmo.
/// @param httpsRequest An instance of HttpsRequest used to send the sensor log to Cosmo.
void ESP32Comm::requestDataFromPeripheral(HttpsRequest& httpsRequest) {
    SensorData sensorData;
    String receivedData = "";

    Wire.requestFrom(ARDUINO_I2C_ADDRESS, (uint8_t)32); // Request up to 32 bytes

    if (Wire.available() && Wire.read() == 'S') {
        auto sensorType = Wire.read();
        
        if (sensorType != 1 && sensorType != 2) {
            Serial.println("Invalid sensor type received.");
            return;
        }
        
        // Clear the rest of the buffer
        while (Wire.available()) {
            Wire.read();
        }

        sensorData.id = generateUUID();
        sensorData.type = "sensor_log";
        sensorData.timestamp = getFormattedTime(); // Get NTP time

        if (sensorType == 1) {
            sensorData.sensorType = "motion";
            sensorData.sensorId = "motion_sensor";
            sensorData.sensorValue = true;
        } else if (sensorType == 2) {
            sensorData.sensorType = "sound";
            sensorData.sensorId = "sound_sensor";
            sensorData.sensorValue = true;
        }

        httpsRequest.sendSensorLogToCosmo(sensorData.toJson());
    } else {
        Serial.println("No sensor data received.");
    }
}
