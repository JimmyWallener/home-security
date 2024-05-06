#ifndef PIRSensors_h
#define PIRSensors_h

#include <Arduino.h>

class PIRSensor{
public:
    PIRSensor() : _pin(-1), _lastTriggerTime(0), _triggerThreshold(10000), _isActive(false){};
    PIRSensor(uint32_t pin);
    void activate();
    void deactivate();
    void update();
    void setTriggerThreshold(unsigned long threshold);

private:
    uint32_t _pin;
    unsigned long _lastTriggerTime;
    unsigned long _triggerThreshold;
    bool _isActive;

    void _handleInterrupt();

    static void handleStaticInterrupt();
};

#endif // PIRSensors_h


// Skriven av: Jimmy Wallener