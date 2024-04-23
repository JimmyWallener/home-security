#pragma once

#include <Arduino.h>

class PIRSensor {
public:
  PIRSensor(int pirPin, int debounceTime);
  bool isTriggered();
  void update();

private:
  unsigned int pirPin_;
  unsigned int debounceTime_;
  unsigned long lastTriggerTime_;
  bool currentReading_;
  bool previousReading_;

  bool checkTrigger();
};

