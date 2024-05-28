#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "Components.h"

class Buzzer : public Component{
public:
    Buzzer(uint8_t buzzerPin, uint16_t delayTime);
    ~Buzzer() override;

    void initialize() override;
    void playAlarm();
    void alarmActiveSound();
    void alarmInactiveSound();
    void keyPressSound();
    void alarmOff();
    void update();

private:
    void playTones(const int* tones, int tonesLength);

    uint8_t _buzzerPin;
    uint16_t _delayTime;
    const int* _tones;
    int _tonesLength;
    int _currentToneIndex;
    bool _isPlaying;
    unsigned long _lastToneChangeTime;
};


#endif // BUZZER_H
