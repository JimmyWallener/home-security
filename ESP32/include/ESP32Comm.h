#ifndef ESP32COMM_H
#define ESP32COMM_H

#include <Wire.h>
#include <WireSlave.h>
#include <ArduinoJson.h>
#include <RTClib.h>
#include <RealTimeClock.h>


class ESP32Comm {
private:
    bool isActive{false};

public:
    ESP32Comm();
    void begin();
    void sendJsonData(const JsonDocument &doc);
    void sendRtcData(RealTimeClock &now);
    void sendTriggerEvent(const char *sensor);
    void sendAlarmActivationChange();
    void sendPinCodeFeedback(bool success, int attemptsLeft);
    static JsonDocument requestDataFromPeripheral();
};

#endif // ESP32COMM_H
