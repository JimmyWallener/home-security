#include "../include/PIRSensor.h"

PIRSensor::PIRSensor(int pirPin, int debounceTime) : pirPin_(pirPin), debounceTime_(debounceTime) {
  pinMode(pirPin_, INPUT);
  lastTriggerTime_ = millis();
  currentReading_ = false;
  previousReading_ = false;
}

bool PIRSensor::isTriggered() {
  update();
  return (millis() - lastTriggerTime_ <= debounceTime_);
}

void PIRSensor::update() {
  currentReading_ = digitalRead(pirPin_);

  // Debounce logic
  if (currentReading_ != previousReading_) {
    lastTriggerTime_ = millis();
  }

  previousReading_ = currentReading_;

  checkTrigger();
}

bool PIRSensor::checkTrigger() {
  if (currentReading_ == HIGH && (millis() - lastTriggerTime_ > debounceTime_)) {
    // Triggered! Send event through serial
    return true; // Replace with actual serial data including timestamp
    lastTriggerTime_ = millis();
  }
    return false;
}
