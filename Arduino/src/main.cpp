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

    components.addComponent(componentArray, numberOfComponents);
    delay(1000);
    components.initializeAll();
    delay(1000);
    unoComm->setLCD(lcd);
    unoComm->setBuzzer(buzzer);
    delay(1000);
}


void loop() {
    delay(10);
    run();
}



bool playingAlarm{true};
uint16_t alarmCount{0};
uint16_t sensorCount{0};
unsigned short timeCounter{0};
unsigned long lastMotionTime{0};
unsigned long lastSoundTime{0};


void run() {
    unsigned long currentTime = millis();

    if (unoComm->getState()) {
        buzzer->update();
        unoComm->update();

        bool motionDetected = pirSensor->isMotionDetected();
        bool soundDetected = soundSensor->isSoundDetected();

        if (motionDetected) {
            lastMotionTime = currentTime;
            sensorCount++;
            if (sensorCount >= SENSOR_THRESHOLD) {
                unoComm->setSensorLog("motion", "pirSensor", true);
                alarmCount++;
                sensorCount = 0;
            }
        }

        if (soundDetected) {
            lastSoundTime = currentTime;
            sensorCount++;
            if (sensorCount >= SENSOR_THRESHOLD) {
                unoComm->setSensorLog("sound", "soundSensor", true);
                alarmCount++;
                sensorCount = 0;
            }
        }

        // Check if no motion or sound has been detected for a while
        if (!motionDetected && !soundDetected && (currentTime - lastMotionTime > RESET_DELAY) && (currentTime - lastSoundTime > RESET_DELAY)) {
            alarmCount = 0;
        }

        // Trigger alarm if the sensor has been triggered SENSOR_OCCURENCES times
        if (alarmCount >= SENSOR_OCCURENCES) {
            alarmCount = 0;
            while (playingAlarm) {
                buzzer->playAlarm();
                buzzer->update();
                unoComm->update();
                if (!unoComm->getState()) {
                    playingAlarm = false;
                    buzzer->alarmInactiveSound();
                }
            }
        }
        timeCounter++;
        if (timeCounter > COUNTER_DURATION) {
            timeCounter = 0;
            alarmCount = 0;
        }
        playingAlarm = true;
    } else {
        buzzer->update();
        unoComm->update();
    }
}




