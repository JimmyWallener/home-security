#ifndef LOGINLOG_H
#define LOGINLOG_H

#include <ArduinoJson.h>

struct LoginLog {
    String type = "login_log";
    String timestamp;
    String userId;
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

#endif // LOGINLOG_H