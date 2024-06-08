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
    SensorLog *getSensorLog();
    void updateLCD();
    void update();
    bool getState();

private:
    String _dateTime{};
    LCD *_lcd;
    SensorLog *_sensorLog{nullptr};
    Buzzer *_buzzer;
    String _pinCode{};
    String _userInputtedPassword;
    uint8_t _state = 0;
    bool _alarmActivated = false;
    unsigned long _lastLCDUpdateTime{0};
    const unsigned long _lcdUpdateInterval{1000};

    static void onReceive(int);
    static void onRequest();
    unsigned long _messageClearTime{0};
    void handleTriggerEvent();
    void switchState();
    void processI2CCommand(int);
    void setRealTimeClock();
    void handleKeypadData();
    void handleAlarmActivation(char);
    void handlePinCodeFeedback();
    void displayTemporaryMessage(const String&, unsigned long);
    void sendLogDataToESP32();

};

#endif // UNOCOMM_H
