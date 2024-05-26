#ifndef UNOCOMM_H
#define UNOCOMM_H

#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include "LCD.h"
#include "SensorLog.h"
#include "Components.h"

class UNOComm : public Component {
public:
    UNOComm();
    ~UNOComm() override;
    void initialize() override;
    void setLCD(LCD*);
    static UNOComm *instance;
    String getRealTimeClock();
    void updateLCD();

private:
    String _dateTime;
    LCD *lcd;
    SensorLog *sensorLog;
    static void onReceive(int);
    unsigned long messageClearTime = 0;
    void handleTriggerEvent();
    void processI2CCommand(int);
    void setRealTimeClock();
    void handleJsonData();
    void handleAlarmActivation();
    void handleAlarmDeactivation();
    void handleAlarmStatusRequest();
    void handlePinCodeFeedback();
    void displayTemporaryMessage(const String&, unsigned long);
    static void sendLogDataToESP32();

};

#endif // UNOCOMM_H
