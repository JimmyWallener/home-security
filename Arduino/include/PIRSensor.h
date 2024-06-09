#ifndef PIRSENSOR_H
#define PIRSENSOR_H

#include <Arduino.h>
#include "Components.h"

class PIRSensor : public Component {
public:
    PIRSensor() : _pin(-1), _lastTriggerTime(0), _triggerThreshold(500), _isActive(false) {};
    PIRSensor(uint8_t);
    ~PIRSensor() override;
    void initialize() override;
    void activate();
    void deactivate();
    int update();
    void setTriggerThreshold(unsigned long);
    bool isMotionDetected(); 
private:
    uint8_t _pin;
    unsigned long _lastTriggerTime;
    unsigned long _triggerThreshold;
    bool _isActive;

    void _handleInterrupt();

    static void handleStaticInterrupt();
    static PIRSensor* pirSensorInstance; // Static instance pointer
};

#endif // PIRSENSOR_H
