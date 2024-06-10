#include "SoundSensor.h"
#include <Arduino.h>

/**
 * @brief Constructor for the SoundSensor class.
 * 
 * Initializes the SoundSensor object with the specified digital pin number.
 * Sets the initial value of the last trigger time to 0 and the trigger threshold to 500 milliseconds.
 * 
 * @param digitalPinNumber The digital pin number where the sound sensor is connected.
 */
SoundSensor::SoundSensor(uint8_t digitalPinNumber) 
    : _digitalPinNumber(digitalPinNumber), _lastTriggerTime(0), _triggerThreshold(500) {}

/**
 * @brief Destructor for the SoundSensor class.
 * 
 * Cleans up any resources used by the SoundSensor object.
 */
SoundSensor::~SoundSensor() {}

/**
 * @brief Initializes the sound sensor.
 * 
 * Sets the specified digital pin as an input pin for reading sound sensor data.
 */
void SoundSensor::initialize() {
    pinMode(_digitalPinNumber, INPUT);
}

/**
 * @brief Checks if sound is detected by the sound sensor.
 * 
 * This method reads the digital value from the sound sensor. If the sensor detects sound (i.e., 
 * the digital value is HIGH) and the time since the last trigger is greater than the trigger threshold, 
 * it updates the last trigger time and returns true. If the digital value is LOW or the time 
 * since the last trigger is less than the threshold, it returns false.
 * 
 * @return True if sound is detected and the time since the last trigger is greater than the threshold, otherwise false.
 */
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
