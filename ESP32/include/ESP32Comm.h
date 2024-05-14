#ifndef ESP32COMM_H
#define ESP32COMM_H

#include <Wire.h>
#include <ArduinoJson.h>
#include <RTClib.h>

class ESP32Comm {
private:
    int sdaPin;
    int sclPin;
public:
    ESP32Comm(int sdaPin, int sclPin);
    void begin();
    void sendJsonData(const JsonDocument &doc);
    void sendRtcData(const DateTime &now);
};

#endif // ESP32COMM_H
