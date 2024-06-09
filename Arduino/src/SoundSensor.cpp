#include "SoundSensor.h"
#include <Arduino.h>

SoundSensor::SoundSensor(uint8_t digitalPinNumber) : _digitalPinNumber(digitalPinNumber), _lastTriggerTime(0), _triggerThreshold(500) {}

SoundSensor::~SoundSensor() {}

void SoundSensor::initialize() {
    pinMode(_digitalPinNumber, INPUT);
}

bool SoundSensor::isSoundDetected(){
    unsigned long currentTime = millis();
    this->_digitalValue = digitalRead(_digitalPinNumber);

    if(this->_digitalValue == HIGH) {
        // check if the time since the last trigger is greater than the threshold
        if(currentTime - _lastTriggerTime > _triggerThreshold) {
            _lastTriggerTime = currentTime;
            return true;
        } else {
            // return false if the time since the last trigger is less than the threshold
            return false;
        }
    }
    // return false if the digital value is LOW
    return false;
}