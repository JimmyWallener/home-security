#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include "PIRSensor.h"
#include "SoundSensor.h"

struct SensorData {
    PIRSensor pirSensor;
    SoundSensor soundSensor;
    
};


void initializeSensorData(SensorData* sensorData, uint32_t pirPin, uint32_t soundPin) {
    sensorData->pirSensor = PIRSensor(pirPin);
    sensorData->soundSensor = SoundSensor(soundPin);
    sensorData->pirSensor.activate();
    sensorData->soundSensor.activateSoundSensor();
}

void updateSensorData(SensorData* sensorData) {
    sensorData->pirSensor.update();
}

void destroySensorData(SensorData* sensorData) {
    sensorData->pirSensor.deactivate();
}

#endif // SENSOR_DATA_H


// Skriven av: Jimmy Wallener