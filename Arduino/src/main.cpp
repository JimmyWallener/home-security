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



void run();

void setup() {
    Serial.begin(115200);

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
    run();
}

void run() {
    if(unoComm->getState()) {
        buzzer->update();
        unoComm->update();

        // Få denna att funka
        if(pirSensor->isMotionDetected()) {
            alarmCount++;
        }

        if(soundSensor->isSoundDetected()) {
            alarmCount++;
        }
        // bort med magical number
        if(alarmCount >= SENSOR_OCCURENCES) {
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
        // bort med magical number
        if(timeCounter > COUNTER_DURATION) {
            timeCounter = 0;
            alarmCount = 0;
        }
        playingAlarm = true;
    
    } else {
        buzzer->update();
        unoComm->update();
    }
}