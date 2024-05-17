#ifndef ESP32COMM_H
#define ESP32COMM_H

#include <Wire.h>
#include <ArduinoJson.h>
#include <RTClib.h>

class ESP32Comm {
private:

public:
    ESP32Comm();
    void begin();
    void sendJsonData(const JsonDocument &doc);
    void sendRtcData(const DateTime &now);
};

#endif // ESP32COMM_H
