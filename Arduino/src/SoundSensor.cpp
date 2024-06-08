#include "SoundSensor.h"
#include <Arduino.h>

SoundSensor::SoundSensor(uint8_t digitalPinNumber) : _digitalPinNumber(digitalPinNumber) {}

SoundSensor::~SoundSensor() {}

void SoundSensor::initialize() {
    pinMode(_digitalPinNumber, INPUT);
}

bool SoundSensor::isSoundDetected(){
    this->_digitalValue = digitalRead(_digitalPinNumber);

    if(this->_digitalValue == HIGH){
        return true;
    }else{
        return false;
    }
}