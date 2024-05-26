#include "SoundSensor.h"
#include <Arduino.h>

SoundSensor::SoundSensor(uint8_t digitalPinNumber) : _digitalPinNumber(digitalPinNumber) {}

void SoundSensor::initialize() {
    pinMode(_digitalPinNumber, INPUT);
}

bool SoundSensor::isSoundDetected() {
    _digitalValue = digitalRead(_digitalPinNumber);
    return _digitalValue == HIGH;
}

/*
SoundSensor::SoundSensor(uint8_t ditigtalPinNumber){
    pinMode(ditigtalPinNumber, INPUT);
    this->ditigtalPinNumber = ditigtalPinNumber;
}

void SoundSensor::activateSoundSensor(){
        this->digitalValue = digitalRead(ditigtalPinNumber);

        if(this->digitalValue == HIGH){
            // insert alarm logic here for when alarm is active
        }else{
            // insert alarm logic here for when alarm is in-active
        }
}*/