#ifndef ACCESSLOG_H
#define ACCESSLOG_H

#include <ArduinoJson.h>

struct AccessLog {
    String type = "login_log";
    String timestamp;
    String userId = "Unknown";
    String action;
    bool success;

    String toJson() const {
        JsonDocument doc;
        doc["type"] = type;
        doc["timestamp"] = timestamp;
        doc["userId"] = userId;
        doc["action"] = action;
        doc["success"] = success;
        String json;
        serializeJson(doc, json);
        return json;
    }
};

#endif // ACCESSLOG_H