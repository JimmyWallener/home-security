#include "Buzzer.h"

Buzzer::Buzzer(uint8_t buzzerPin, uint16_t delayTime) : _buzzerPin(buzzerPin), _delayTime(delayTime) {}

void Buzzer::initialize() {
    pinMode(_buzzerPin, OUTPUT);
    noTone(_buzzerPin);
}

void Buzzer::playAlarm() {
    tone(_buzzerPin, 698); // F
    delay(_delayTime - 70);
    tone(_buzzerPin, 784); // G
    delay(_delayTime - 70);
    tone(_buzzerPin, 831); // A FLAT
    delay(_delayTime - 70);
}

void Buzzer::alarmActiveSound() {
    tone(_buzzerPin, 440);
    delay(_delayTime);
    tone(_buzzerPin, 466);
    delay(_delayTime);
    tone(_buzzerPin, 494);
    delay(_delayTime);
    tone(_buzzerPin, 523);
    delay(_delayTime);
    tone(_buzzerPin, 554);
    delay(_delayTime);
}

void Buzzer::alarmInactiveSound() {
    tone(_buzzerPin, 554);
    delay(_delayTime);
    tone(_buzzerPin, 523);
    delay(_delayTime);
    tone(_buzzerPin, 494);
    delay(_delayTime);
    tone(_buzzerPin, 466);
    delay(_delayTime);
    tone(_buzzerPin, 440);
    delay(_delayTime);
}

void Buzzer::alarmOff() {
    digitalWrite(_buzzerPin, LOW);
}
