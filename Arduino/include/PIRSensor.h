#ifndef PIRSENSOR_H
#define PIRSENSOR_H

#include <Arduino.h>

class PIRSensor {
public:
    PIRSensor() : _pin(-1), _lastTriggerTime(0), _triggerThreshold(10000), _isActive(false) {};
    PIRSensor(uint32_t pin);
    void begin(); 
    void activate();
    void deactivate();
    int update();
    void setTriggerThreshold(unsigned long threshold);
    bool isMotionDetected(); 
private:
    uint32_t _pin;
    unsigned long _lastTriggerTime;
    unsigned long _triggerThreshold;
    bool _isActive;

    void _handleInterrupt();

    static void handleStaticInterrupt();
    static PIRSensor* pirSensorInstance; // Static instance pointer
};

#endif // PIRSENSOR_H
