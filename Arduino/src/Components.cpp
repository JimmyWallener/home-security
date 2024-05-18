#include "Components.h"

Components::Components(int soundPin, int pirPin, uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, int buzzerPin, uint16_t buzzerDelayTime)
    : soundSensor(soundPin), pirSensor(pirPin), lcd(rs, enable, d4, d5, d6, d7), buzzer(buzzerPin, buzzerDelayTime) {}

void Components::begin() {
    soundSensor.begin();
    pirSensor.begin();
    lcd.begin();
    buzzer.begin();
    unoComm.begin();
    pirSensor.activate(); // Activate PIR sensor
}

 // TODO: Needs to be refactored or removed. This is just a placeholder for testing purposes.
SensorData Components::getSensorData() {
    SensorData data;
    data.motionDetected = pirSensor.isMotionDetected();
    data.soundDetected = soundSensor.isSoundDetected();
    data.formattedTime = unoComm.getRtcData();
    return data;
}

// TODO: SensorLog.h
