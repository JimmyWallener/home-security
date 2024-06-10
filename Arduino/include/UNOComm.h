#ifndef UNOCOMM_H
#define UNOCOMM_H

#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include "LCD.h"
#include "Components.h"
#include "Buzzer.h"
#include "SensorTypes.h"

class UNOComm : public Component {
public:
    UNOComm();
    ~UNOComm() override;
    void initialize() override;
    void setLCD(LCD*);
    void setSensor(SensorType);
    void setBuzzer(Buzzer*);
    static UNOComm *instance;
    String getRealTimeClock();
    void updateLCD();
    void update();
    bool getState() const;

private:
    String _dateTime{};
    SensorType _sensorType{NO_SENSOR};
    LCD *_lcd;
    Buzzer *_buzzer;
    char _pinCode[5]{""};
    char _userInputtedPassword[5]{""};
    uint8_t _state = 0;
    unsigned long _lastKeypadInputTime{0};
    bool _alarmActivated = false;
    unsigned long _lastLCDUpdateTime{0};
    const unsigned long _lcdUpdateInterval{1000};

    static void onReceive(int);
    static void onRequest();
    unsigned long _messageClearTime{0};
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
