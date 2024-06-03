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
    bool getState();

private:
    String _dateTime{};
    LCD *lcd;
    SensorLog *sensorLog;
    Buzzer *buzzer;
    String _pinCode{};
    String _userInputtedPassword;
    const String _password = "1234";
    uint8_t _state = 0;
    unsigned long _lastLCDUpdateTime{0};
    const unsigned long lcdUpdateInterval{1000};

    static void onReceive(int);
    unsigned long messageClearTime{0};
    void handleTriggerEvent();
    void switchState();
    void processI2CCommand(int);
    void setRealTimeClock();
    void handleKeypadData();
    void handleAlarmActivation(char);
    void handleAlarmStatusRequest();
    void handlePinCodeFeedback();
    bool checkPassword();
    void displayTemporaryMessage(const String&, unsigned long);
    static void sendLogDataToESP32();

};

#endif // UNOCOMM_H
