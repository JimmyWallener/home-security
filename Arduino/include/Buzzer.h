#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "Components.h"

class Buzzer : public Component {
private:
    uint8_t _buzzerPin;
    uint16_t _delayTime;

public:
    Buzzer() : _buzzerPin(-1), _delayTime(205) {}
    Buzzer(uint8_t, uint16_t);
    ~Buzzer() override;
    void initialize() override;
    void playAlarm();
    void alarmActiveSound();
    void alarmInactiveSound();
    void alarmOff();
};

#endif // BUZZER_H
