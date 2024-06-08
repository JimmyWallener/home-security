#ifndef ESP32COMM_H
#define ESP32COMM_H

#include <Wire.h>
#include <ArduinoJson.h>
#include <RTClib.h>
#include <RealTimeClock.h>
#include "HttpsRequest.h"


class ESP32Comm {
private:
public:
    ESP32Comm();
    void begin();
    void sendRtcData(RealTimeClock&);
    void sendTriggerEvent(const char*);
    void sendAlarmActivationChange(bool);
    void sendPinCodeFeedback(bool, int );
    void sendKeypadData(char);
    static void requestDataFromPeripheral(HttpsRequest&);
};

#endif // ESP32COMM_H
