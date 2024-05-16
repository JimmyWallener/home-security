#ifndef UNOCOMM_H
#define UNOCOMM_H

#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>

class UNOComm {
public:
    UNOComm();
    void begin();
    void onReceive(int numBytes);
    String getRtcData();
private:
    DateTime currentTime;
    static void onReceiveWrapper(int numBytes);
    static UNOComm* instance; // Pointer to the current instance
    void handleRTCData();
    void handleTriggerEvent();
    void handleJsonData();
    void handleAlarmActivation();
    void handleAlarmDeactivation();
    void handleAlarmStatusRequest();
    void handlePinCodeFeedback();
};

#endif // UNOCOMM_H
