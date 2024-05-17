#include <Arduino.h>
#include "Components.h"
#include "Global.h"



Components components(SOUND_SENSOR_PIN, PASSIVE_IR_SENSOR_PIN, 
                        LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, 
                        LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN, 
                        BUZZER_PIN, BUZZER_DELAY_TIME);

void setup() {
    Serial.begin(115200);
    components.begin();
}

void loop() {
    SensorData data = components.getSensorData();
    components.lcd.printMessage(components.unoComm.getRtcData());

    if (data.soundDetected || data.motionDetected) {
        components.buzzer.playAlarm();
    } else {
        components.buzzer.alarmOff();
    }

    Serial.println(data.toJson());
}


