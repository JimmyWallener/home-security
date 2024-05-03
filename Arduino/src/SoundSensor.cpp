#include "../include/SoundSensor.h"
#include <Arduino.h>



SoundSensor::SoundSensor(uint8_t ditigtalPinNumber){
    pinMode(ditigtalPinNumber, INPUT);
}

void SoundSensor::activateSoundSensor(){
        this->_digitalValue = digitalRead(_ditigtalPinNumber);

        if(this->_digitalValue == HIGH){
            // insert alarm logic here for when alarm is active
        }else{
            // insert alarm logic here for when alarm is in-active
        }
}

// Skriven av: Leo Katakalidis