#include <Arduino.h>
#include "Components.h"
#include "constants.h"
#include "SoundSensor.h"
#include "LCD.h"
#include "Buzzer.h"
#include "PIRSensor.h"
#include "UNOComm.h"
#include "SensorLog.h"

using namespace constants;

Components components;

SoundSensor *soundSensor = new SoundSensor(SOUND_SENSOR_PIN);
LCD *lcd = new LCD(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
Buzzer *buzzer = new Buzzer(BUZZER_PIN, BUZZER_DELAY_TIME);
PIRSensor *pirSensor = new PIRSensor(PASSIVE_IR_SENSOR_PIN);
UNOComm *unoComm = new UNOComm;

void run();
void sendLog(String sensorType, String sensorId);

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
unsigned short timeCounter = 0;

void loop() {
    delay(10);
    run();
}

void run() {
    if (unoComm->getState()) {
        buzzer->update();
        unoComm->update();
        Serial.println("run update");

        if (pirSensor->isMotionDetected()) {
            alarmCount++;
            if(alarmCount >= SENSOR_OCCURENCES) {
                sendLog("motion", "pirSensor");
                alarmCount = 0;
            }
            
        }

        if (soundSensor->isSoundDetected()) {
            alarmCount++;
            if(alarmCount >= SENSOR_OCCURENCES) {
                sendLog("sound", "soundSensor");
                alarmCount = 0;
            }
        }

        if (alarmCount >= SENSOR_OCCURENCES) {
            alarmCount = 0;
            while (playingAlarm) {
                buzzer->playAlarm();
                buzzer->update();
                unoComm->update();
                if (!unoComm->getState()) {
                    playingAlarm = false;
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


void sendLog(String sensorType, String sensorId) {
    SensorLog sensorLog;
    sensorLog.sensorType = sensorType;
    sensorLog.sensorId = sensorId;
    sensorLog.timestamp = unoComm->getRealTimeClock();
    sensorLog.value = true;
    
    // Kontrollera genererad JSON
    String logData = sensorLog.toJson();
    Serial.println("Prepared SensorLog JSON: " + logData);

    if (!logData.length() == 0) {
        unoComm->setSensorLog(&sensorLog);
    } else {
        Serial.println("Error: Log data is empty, not setting SensorLog.");
    }
}