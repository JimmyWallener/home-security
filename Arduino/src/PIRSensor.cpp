/***
 * @file PIRSensor.cpp
 * @brief Implementation of the PIRSensor class
 * @author Jimmy Wallener
 * @date 2024-04-27
 * 
*/
#include "PIRSensor.h"



/**
 * @brief class constructor for the passive IR sensor
 * @details This method constructs a new passive IR sensor object with the given pin and sets the pin mode to INPUT
 * @param pin
 * @return * PIRSensor
 * 
 */

PIRSensor::PIRSensor(uint32_t pin) {
    pinMode(pin, INPUT);
    _pin = pin;
}

/**
 * @brief activate the passive IR sensor
 * @details This method activates the passive IR sensor by attaching an interrupt to the sensor pin
 * 
 * @return void
 * 
 */
void PIRSensor::activate()
{
    attachInterrupt(digitalPinToInterrupt(_pin), handleStaticInterrupt, RISING);
    _isActive = true;
}

/**
 * @brief deactivate the passive IR sensor
 * @details This method deactivates the passive IR sensor by detaching the interrupt from the sensor pin
 * 
 * @return void
 * 
 */
void PIRSensor::deactivate() {
    detachInterrupt(digitalPinToInterrupt(_pin));
    _isActive = false;
}

/**
 * @brief set the trigger threshold for the sensor
 * @details This method sets the trigger threshold for the sensor
 * @param threshold 
 * @return ** void 
 */
void PIRSensor::setTriggerThreshold(unsigned long threshold) {
    _triggerThreshold = threshold;
}

/**
 * @brief update the sensor state based on the current state of the sensor pin
 * @details This method is called by the interrupt handler to update the sensor state based on the current state of the sensor pin and the trigger threshold
 * 
 * @return ** void
 * 
 * @todo implement the logic to send data to the ESP32 through Serial
 */
int PIRSensor::update() {
    if (_isActive && digitalRead(_pin) == HIGH) {
        unsigned long currentTime = millis();
        if (currentTime - _lastTriggerTime > _triggerThreshold) {
            _lastTriggerTime = currentTime;
            Serial.println("PIR Sensor Triggered");
            return 1;
        }

    }
    return 0;
    Serial.println("PIR Sensor Not Triggered");
}

/**
 * @brief interrupt handler for the passive IR sensor
 * @details This method is called by the interrupt handler to update the sensor state
 * 
 * @return ** void 
 */
void PIRSensor::_handleInterrupt() {
    if (_isActive) {
        update();
    }
}

/**
 * @brief static interrupt handler for the passive IR sensor
 * @details This method is called by the interrupt handler to update the sensor state
 * @return ** void 
 */
void PIRSensor::handleStaticInterrupt() {
    static PIRSensor* pirSensorInstance = nullptr;
    if (pirSensorInstance != nullptr) {
        pirSensorInstance->_handleInterrupt();
    }
   
}
