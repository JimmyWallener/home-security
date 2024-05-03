#pragma once

#include <Arduino.h>

class Buzzer{
    private:
        uint8_t _buzzerPin;
        uint16_t _delayTime;

    public:
        Buzzer() : _buzzerPin(-1), _delayTime(205){};
        Buzzer(uint8_t buzzerPin, uint16_t delayTime);
        void initialize();
        void playAlarm();
        void alarmActiveSound();
        void alarmInactiveSound();
};


// Skriven av: Leo Katakalidis