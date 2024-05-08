/**
 * @file SoundSensor.cpp
 * @author Leo Katakalidis
 * @brief Implementation of the SoundSensor class
 * @date 2024-05-06
 * 
 * 
 */


#include "SoundSensor.h"
#include <Arduino.h>

/**
 * @brief Construct a new Sound Sensor:: Sound Sensor object
 * @details This method constructs a new sound sensor object with the given pin and sets the pin mode to INPUT
 * @param ditigtalPinNumber 
 */

SoundSensor::SoundSensor(uint8_t ditigtalPinNumber){
    pinMode(ditigtalPinNumber, INPUT);
    this->_ditigtalPinNumber = ditigtalPinNumber;
}


/**
 * @brief Method to activate the sound sensor
 * @details This method activates the sound sensor by reading the digital value of the sensor pin
 * @return void
 * @todo Implement alarm logic for when alarm is active and in-active
 */
void SoundSensor::activateSoundSensor(){
        this->_digitalValue = digitalRead(_ditigtalPinNumber);

        if(this->_digitalValue == HIGH){
            // insert alarm logic here for when alarm is active
        }else{
            // insert alarm logic here for when alarm is in-active
        }
}
