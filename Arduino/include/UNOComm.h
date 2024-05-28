#ifndef UNOCOMM_H
#define UNOCOMM_H

#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include "LCD.h"
#include "SensorLog.h"
#include "Components.h"
#include "Buzzer.h"

class UNOComm : public Component {
public:
    UNOComm();
    ~UNOComm() override;
    void initialize() override;
    void setLCD(LCD*);
    void setSensorLog(SensorLog*);
    void setBuzzer(Buzzer*);
    static UNOComm *instance;
    String getRealTimeClock();
    void updateLCD();
    void update();

private:
    String _dateTime{};
    LCD *lcd;
    SensorLog *sensorLog;
    Buzzer *buzzer;
    String _pinCode{};
    unsigned long _lastLCDUpdateTime{0};
    const unsigned long lcdUpdateInterval{1000};

    static void onReceive(int);
    unsigned long messageClearTime{0};
    void handleTriggerEvent();
    void processI2CCommand(int);
    void setRealTimeClock();
    void handleKeypadData();
    void handleAlarmActivation(char);
    void handleAlarmStatusRequest();
    void handlePinCodeFeedback();
    void displayTemporaryMessage(const String&, unsigned long);
    static void sendLogDataToESP32();

};

#endif // UNOCOMM_H
