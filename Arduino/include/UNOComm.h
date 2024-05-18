#ifndef UNOCOMM_H
#define UNOCOMM_H

#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include "LCD.h"
#include "SensorLog.h"

class UNOComm {
public:
    UNOComm();
    void begin();
    void setLCD(LCD *lcd);
    void onReceive(int numBytes);
    String getRtcData();
    void updateLCD();

private:
    DateTime currentTime;
    LCD *lcd;
    static void onReceiveWrapper(int numBytes);
    static UNOComm *instance; // Pointer to the current instance

    unsigned long messageClearTime = 0;

    void handleRTCData();
    void handleTriggerEvent();
    void handleJsonData();
    void handleAlarmActivation();
    void handleAlarmDeactivation();
    void handleAlarmStatusRequest();
    void handlePinCodeFeedback();
    void displayTemporaryMessage(const String &message, unsigned long duration);
    void sendLogDataToESP32(const SensorLog &sensorLog);
    void sendAlarmStatusToESP32(bool alarmActive);

};

#endif // UNOCOMM_H
