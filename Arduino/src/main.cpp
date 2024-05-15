#include <Arduino.h>
#include "Components.h"

Components components(7, 8, 12, 11, 5, 4, 3, 2, 9, 205);

void setup() {
    Serial.begin(115200);
    components.begin();
}

void loop() {
    SensorData data = components.getSensorData();
    components.lcd.printMessage(data.formattedTime);

    if (data.soundDetected || data.motionDetected) {
        components.buzzer.playAlarm();
    } else {
        components.buzzer.alarmOff();
    }

    Serial.println(data.toJson());
}


