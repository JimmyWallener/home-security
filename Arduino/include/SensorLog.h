#ifndef SENSORLOG_H
#define SENSORLOG_H

#include <ArduinoJson.h>

struct SensorLog {
    String type = "sensor_log";
    String timestamp;
    String sensorType;
    String sensorId;
    bool value;

    String toJson() const {
        JsonDocument doc;
        doc["type"] = type;
        doc["timestamp"] = timestamp;
        doc["sensorType"] = sensorType;
        doc["sensorId"] = sensorId;
        doc["value"] = value;
        String json;
        serializeJson(doc, json);
        return json;
    }
};

#endif // SENSORLOG_H