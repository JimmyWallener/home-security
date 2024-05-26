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

SoundSensor *soundSensor;
LCD *lcd;
Buzzer *buzzer;
PIRSensor *pirSensor;
UNOComm *unoComm;



void setup() {
    Serial.begin(115200);

     soundSensor = new SoundSensor(SOUND_SENSOR_PIN);
     lcd = new LCD(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
     buzzer = new Buzzer(BUZZER_PIN, BUZZER_DELAY_TIME);
     pirSensor = new PIRSensor(PASSIVE_IR_SENSOR_PIN);
     unoComm = new UNOComm;

     Serial.println("Components set");
     delay(10000);
     Component *componentArray[] = {soundSensor, lcd, buzzer, pirSensor, unoComm};
     int numberOfComponents = sizeof(componentArray) / sizeof(componentArray[0]);

     components.addComponent(componentArray, numberOfComponents);
     Serial.println("Components added");
     delay(1000);
     components.initializeAll();
     delay(1000);
     unoComm->setLCD(lcd);
     Serial.println("Components initialized");
     delay(1000);
}

void loop() {
}


