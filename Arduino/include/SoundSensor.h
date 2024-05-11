#pragma once

#include <Arduino.h>

// !! If you are having issues, remember to configure the potienciometer of the sound sensor !!
class SoundSensor{
    private:
    int _digitalValue;
    uint8_t _ditigtalPinNumber;

    public:
        SoundSensor() : _digitalValue(0), _ditigtalPinNumber(0){};
        void initialize(uint8_t ditigtalPinNumber);
        int readSoundSensor();
};

// Skriven av: Leo Katakalidis