#include <Arduino.h>
#include "Components.h"
#include "constants.h"
#include "SoundSensor.h"
#include "LCD.h"
#include "Buzzer.h"
#include "PIRSensor.h"
#include "UNOComm.h"
#include "SensorTypes.h"

using namespace constants;

Components components;

SoundSensor *soundSensor = new SoundSensor(SOUND_SENSOR_PIN);
LCD *lcd = new LCD(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
Buzzer *buzzer = new Buzzer(BUZZER_PIN, BUZZER_DELAY_TIME);
PIRSensor *pirSensor = new PIRSensor(PASSIVE_IR_SENSOR_PIN);
UNOComm *unoComm = new UNOComm;

void run();

/**
 * @brief Arduino setup function.
 * 
 * This function initializes serial communication, creates an array of component pointers,
 * adds them to the Components instance, and initializes all components. 
 * It also sets up the LCD and buzzer components in the UNOComm instance.
 */
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

/**
 * @brief Arduino main loop function.
 * 
 * This function continuously calls the run function every 10 milliseconds.
 */
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

/**
 * @brief Main run function for the alarm system.
 * 
 * This function handles the main logic of the alarm system. It checks for motion and sound detection,
 * updates the state of the buzzer and UNOComm, and triggers the alarm if necessary.
 */
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
                unoComm->setSensor(MOTION_SENSOR);
                alarmCount++;
                sensorCount = 0;
            }
        }

        if (soundDetected) {
            lastSoundTime = currentTime;
            sensorCount++;
            if (sensorCount >= SENSOR_THRESHOLD) {
                unoComm->setSensor(SOUND_SENSOR);
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
