#include "PIRSensor.h"

PIRSensor* PIRSensor::pirSensorInstance = nullptr; // Define static instance pointer

/**
 * @brief Constructor for the PIRSensor class.
 * 
 * Initializes the PIRSensor object with the specified pin.
 * 
 * @param pin The pin number where the PIR sensor is connected.
 */
PIRSensor::PIRSensor(uint8_t pin) {
    _pin = pin;
}

/**
 * @brief Destructor for the PIRSensor class.
 * 
 * Cleans up any resources used by the PIRSensor object.
 */
PIRSensor::~PIRSensor() {}

/**
 * @brief Initializes the PIR sensor.
 * 
 * Sets the pin mode to INPUT and assigns this object to the static instance.
 */
void PIRSensor::initialize() {
    pinMode(_pin, INPUT);
    pirSensorInstance = this; // Set static instance to this object
}

/**
 * @brief Activates the PIR sensor.
 * 
 * Attaches an interrupt to the pin and sets the isActive flag to true.
 */
void PIRSensor::activate() {
    attachInterrupt(digitalPinToInterrupt(_pin), handleStaticInterrupt, RISING);
    _isActive = true;
}

/**
 * @brief Deactivates the PIR sensor.
 * 
 * Detaches the interrupt from the pin and sets the isActive flag to false.
 */
void PIRSensor::deactivate() {
    detachInterrupt(digitalPinToInterrupt(_pin));
    _isActive = false;
}

/**
 * @brief Sets the trigger threshold for the PIR sensor.
 * 
 * Defines the minimum time interval (in milliseconds) between two motion detections.
 * 
 * @param threshold The time threshold in milliseconds.
 */
void PIRSensor::setTriggerThreshold(unsigned long threshold) {
    _triggerThreshold = threshold;
}

/**
 * @brief Updates the PIR sensor status.
 * 
 * Checks if motion is detected and if the time since the last trigger is greater than the threshold.
 * 
 * @return Returns 1 if motion is detected and the time since the last trigger is greater than the threshold, otherwise returns 0.
 */
int PIRSensor::update() {
    if (_isActive && digitalRead(_pin) == HIGH) {
        unsigned long currentTime = millis();
        if (currentTime - _lastTriggerTime > _triggerThreshold) {
            _lastTriggerTime = currentTime;
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Checks if motion is detected by the PIR sensor.
 * 
 * Calls the update method to determine if motion is detected.
 * 
 * @return Returns true if motion is detected, otherwise returns false.
 */
bool PIRSensor::isMotionDetected() {
    return update() == 1;
}

/**
 * @brief Handles the interrupt for the PIR sensor.
 * 
 * This method is called when an interrupt occurs and checks if motion is detected.
 */
void PIRSensor::_handleInterrupt() {
    if (_isActive) {
        update();
    }
}

/**
 * @brief Static method to handle the interrupt for the PIR sensor.
 * 
 * Calls the instance method to handle the interrupt.
 */
void PIRSensor::handleStaticInterrupt() {
    if (pirSensorInstance != nullptr) {
        pirSensorInstance->_handleInterrupt();
    }
}
