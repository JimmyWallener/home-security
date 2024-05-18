#ifndef ESP32COMM_H
#define ESP32COMM_H

#include <Wire.h>
#include <WireSlave.h>
#include <ArduinoJson.h>
#include <RTClib.h>

class ESP32Comm {
private:
    static void onReceiveWrapper(int numBytes);
    static ESP32Comm *instance;

public:
    ESP32Comm();
    void begin();
    void sendJsonData(const JsonDocument &doc);
    void sendRtcData(const DateTime &now);
    void sendTriggerEvent(const char *sensor);
    void sendAlarmActivation();
    void sendAlarmDeactivation();
    void sendPinCodeFeedback(bool success, int attemptsLeft);
    void sendAlarmStatusRequest();
    void onReceive(int numBytes);
    

};

#endif // ESP32COMM_H
