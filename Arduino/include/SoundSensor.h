#ifndef SOUNDSENSOR_H
#define SOUNDSENSOR_H

#include <Arduino.h>
#include "Components.h"


// !! If you are having issues, remember to configure the potentiometer of the sound sensor !!
class SoundSensor : public Component {
private:
    int _digitalValue;
    uint8_t _digitalPinNumber;
    

public:
    SoundSensor() : _digitalValue(0), _digitalPinNumber(0) {}
    SoundSensor(uint8_t digitalPinNumber);
    ~SoundSensor() override;
    void initialize() override;
    bool isSoundDetected();
};

#endif // SOUNDSENSOR_H
