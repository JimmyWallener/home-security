#include <Arduino.h>
#include "Components.h"
#include "constants.h"
#include "SoundSensor.h"
#include "LCD.h"
#include "Buzzer.h"
#include "PIRSensor.h"
#include "UNOComm.h"

using namespace constants;

Components components;

SoundSensor *soundSensor = new SoundSensor(SOUND_SENSOR_PIN);
LCD *lcd = new LCD(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
Buzzer *buzzer = new Buzzer(BUZZER_PIN, BUZZER_DELAY_TIME);
PIRSensor *pirSensor = new PIRSensor(PASSIVE_IR_SENSOR_PIN);
UNOComm *unoComm = new UNOComm;

extern int __heap_start, *__brkval;

int freeMemory() {
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void setup() {
    Serial.begin(115200);

    Serial.println("Free memory: ");
    Serial.println(freeMemory());

    Component *componentArray[] = {soundSensor, lcd, buzzer, pirSensor, unoComm};
    int numberOfComponents = sizeof(componentArray) / sizeof(componentArray[0]);
    Serial.print("Number of components to add: ");
    Serial.println(numberOfComponents);

    components.addComponent(componentArray, numberOfComponents);
    Serial.println("Components added");
    delay(1000);
    components.initializeAll();
    delay(1000);
    unoComm->setLCD(lcd);
    unoComm->setBuzzer(buzzer);
    Serial.println("Components initialized");
    delay(1000);
}

bool playingAlarm = true;
int alarmCount = 0;
int timeCounter = 0; 

void loop() {
    delay(10);
    
    if(unoComm->getState()) {
        buzzer->update();
        unoComm->update();

        if(pirSensor->isMotionDetected()) {
            alarmCount++;
        }

        if(soundSensor->isSoundDetected()) {
            alarmCount++;
        }

        if(alarmCount >= 3) {
            alarmCount = 0;
            while(playingAlarm) {
                buzzer->playAlarm();
                buzzer->update();
                unoComm->update();
                if(!unoComm->getState()) {
                    playingAlarm = false;
                }
            }
        }
        timeCounter++;
        if(timeCounter > 500) {
            timeCounter = 0;
            alarmCount = 0;
        }
        playingAlarm = true;
    
    } else {
        buzzer->update();
        unoComm->update();
    }
}
