#include "PIRSensor.h"

// Default constructor due to SensorData struct
PIRSensor::PIRSensor(uint32_t pin) : _pin(pin), _lastTriggerTime(0), _triggerThreshold(1000), _isActive(false){}

// Method to activate the sensor
void PIRSensor::activate()
{
    attachInterrupt(digitalPinToInterrupt(_pin), handleStaticInterrupt, RISING);
    _isActive = true;
}

// Method to deactivate the sensor
void PIRSensor::deactivate() {
    detachInterrupt(digitalPinToInterrupt(_pin));
    _isActive = false;
}

// Method to set the trigger threshold
void PIRSensor::setTriggerThreshold(unsigned long threshold) {
    _triggerThreshold = threshold;
}

// Update method to check if the sensor is triggered
void PIRSensor::update() {
    if (_isActive && digitalRead(_pin) == HIGH) {
        unsigned long currentTime = millis();
        if (currentTime - _lastTriggerTime > _triggerThreshold) {
            _lastTriggerTime = currentTime;
            Serial.println("PIR Sensor Triggered");
            // Send data to ESP32 through Serial
        }

    }
    Serial.println("PIR Sensor Not Triggered");
}

// Method to handle the interrupt
void PIRSensor::_handleInterrupt() {
    if (_isActive) {
        update();
    }
}

// Static method to handle interrupts
void PIRSensor::handleStaticInterrupt() {
    static PIRSensor* pirSensorInstance = nullptr;
    if (pirSensorInstance != nullptr) {
        pirSensorInstance->_handleInterrupt();
    }
   
}

// Skriven av: Jimmy Wallener