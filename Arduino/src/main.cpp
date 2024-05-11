#include <Arduino.h>
#include <SPI.h>
#include <Keypad.h>
#include <Wire.h>
#include "../include/Struct/SensorData.h"
#include "../include/Struct/Components.h"
// This is a SLAVE unit

SensorData sensorData;
Components components;

const int pirPin = -1;
const int soundPin = -1;    
const int buzzerPin = -1;

void receiveEvent(int howMany) {
    while(1 < Wire.available()) {
        int x = Wire.read();
        if(x == 1) {
            int sensorOccurenceCounter = 0;
            int timeCounter = 0;

            while(sensorOccurenceCounter < 3) {
                sensorOccurenceCounter += sensorData.soundSensor.readSoundSensor();
                sensorOccurenceCounter += sensorData.pirSensor.update();
                timeCounter++;

                if(timeCounter >= 50) {
                    sensorOccurenceCounter = 0;
                }
            }

            bool loopActive = true;
            while(loopActive) {
                components.buzzer.playAlarm();
                int y = Wire.read();
                if(y == 0) {
                    sensorOccurenceCounter = 0;
                    loopActive = false;
                }
            }
        }
    }
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    initializeSensorData(&sensorData, pirPin, soundPin);
    initializeComponents(&components);
    Wire.begin(4);
    Wire.onReceive(receiveEvent);
}

void loop() {
    //updateSensorData(&sensorData);
    //components.rtc.RTCUpdate();
    //Serial.println(components.rtc.GetTime().year);
    //components.lcdDisplay.LCDUpdate(components.rtc.GetTime());
    //delay(1000);
}

