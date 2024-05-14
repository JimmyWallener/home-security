#include "PIRSensor.h"

PIRSensor* PIRSensor::pirSensorInstance = nullptr; // Define static instance pointer

PIRSensor::PIRSensor(uint32_t pin) {
    _pin = pin;
}

// Set the pin mode to input
void PIRSensor::begin() {
    pinMode(_pin, INPUT);
    pirSensorInstance = this; // Set static instance to this object
}

//  Attaches the interrupt to the pin and sets the isActive flag to true
void PIRSensor::activate() {
    attachInterrupt(digitalPinToInterrupt(_pin), handleStaticInterrupt, RISING);
    _isActive = true;
}

//  Detaches the interrupt from the pin and sets the isActive flag to false
void PIRSensor::deactivate() {
    detachInterrupt(digitalPinToInterrupt(_pin));
    _isActive = false;
}

// Set the threshold for the time between triggers
void PIRSensor::setTriggerThreshold(unsigned long threshold) {
    _triggerThreshold = threshold;
}

// returns 1 if motion is detected and the time since last trigger is greater than the threshold
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
}

// returns 1 if motion is detected
bool PIRSensor::isMotionDetected() {
    return update() == 1;
}

// method to handle the interrupt, needed for the attachInterrupt function
void PIRSensor::_handleInterrupt() {
    if (_isActive) {
        update();
    }
}

// Static method to handle the interrupt
void PIRSensor::handleStaticInterrupt() {
    if (pirSensorInstance != nullptr) {
        pirSensorInstance->_handleInterrupt();
    }
}
