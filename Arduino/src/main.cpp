#include <Arduino.h>
#include "../include/Struct/SensorData.h"
#include "../include/Struct/Components.h"

SensorData sensorData;
Components components;


void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    initializeSensorData(&sensorData, 3, 4);
    initializeComponents(&components);
}

void loop() {
    updateSensorData(&sensorData);
    delay(1000);
}