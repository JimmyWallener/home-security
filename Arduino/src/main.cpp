#include <Arduino.h>
#include <SPI.h>
#include "../include/Struct/SensorData.h"
#include "../include/Struct/Components.h"

SensorData sensorData;
Components components;

const int pirPin = -1;
const int soundPin = -1;    
const int buzzerPin = -1;


void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    initializeSensorData(&sensorData, pirPin, soundPin);
    initializeComponents(&components);
}

void loop() {
    updateSensorData(&sensorData);
    components.rtc.RTCUpdate();
    components.lcdDisplay.LCDUpdate(components.rtc.GetTime());
    delay(1000);
}