#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <ArduinoJson.h>

// TODO: Refactor this struct to be more generic and handle different types of sensor data
struct SensorData {
    bool motionDetected;
    bool soundDetected;
    String formattedTime;

    String toJson() {
        JsonDocument doc;
        doc["motionDetected"] = motionDetected;
        doc["soundDetected"] = soundDetected;
        doc["formattedTime"] = formattedTime;
        String output;
        serializeJson(doc, output);
        return output;
    }
};

#endif // SENSORDATA_H
