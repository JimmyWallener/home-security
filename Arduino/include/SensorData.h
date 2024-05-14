#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <ArduinoJson.h>

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
