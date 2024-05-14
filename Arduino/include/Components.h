#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "SoundSensor.h"
#include "PIRSensor.h"
#include "LCD.h"
#include "Buzzer.h"
#include "SensorData.h"
#include "UNOComm.h"

class Components {
public:
    SoundSensor soundSensor;
    PIRSensor pirSensor;
    LCD lcd;
    Buzzer buzzer;
    UNOComm unoComm;

    Components(int soundPin, int pirPin, uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, int buzzerPin, uint16_t buzzerDelayTime);
    void begin();
    SensorData getSensorData();
};

#endif // COMPONENTS_H
