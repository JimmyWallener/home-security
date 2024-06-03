#include "Buzzer.h"

Buzzer::Buzzer(uint8_t buzzerPin, uint16_t delayTime)
    : _buzzerPin(buzzerPin), _delayTime(delayTime), _tones(nullptr), _tonesLength(0), _currentToneIndex(0), _isPlaying(false), _lastToneChangeTime(0) {}

Buzzer::~Buzzer() {
    noTone(_buzzerPin);
}

void Buzzer::initialize() {
    pinMode(_buzzerPin, OUTPUT);
    noTone(_buzzerPin);
}

void Buzzer::playAlarm() {
    static const int tones[] = {698, 784, 831};
    for(int i = 0; i < 3; i++) {
        tone(_buzzerPin, tones[i]);
        delay(40);
    } 
}

void Buzzer::alarmActiveSound() {
    static const int tones[] = {440, 466, 494, 523, 554};
    playTones(tones, sizeof(tones) / sizeof(tones[0]));
}

void Buzzer::alarmInactiveSound() {
    static const int tones[] = {554, 523, 494, 466, 440};
    playTones(tones, sizeof(tones) / sizeof(tones[0]));
}

void Buzzer::wrongPassword() {
    static const int tones[] = {554, 440};
    playTones(tones, sizeof(tones) / sizeof(tones[0]));
}

void Buzzer::playTones(const int* tones, int tonesLength) {
    _tones = tones;
    _tonesLength = tonesLength;
    _currentToneIndex = 0;
    _isPlaying = true;
    _lastToneChangeTime = millis();
    tone(_buzzerPin, _tones[_currentToneIndex]);
}

void Buzzer::update() {
    if (_isPlaying && (millis() - _lastToneChangeTime) >= _delayTime) {
        _lastToneChangeTime = millis();
        noTone(_buzzerPin);
        _currentToneIndex++;
        if (_currentToneIndex < _tonesLength) {
            tone(_buzzerPin, _tones[_currentToneIndex]);
        } else {
            _isPlaying = false;
        }
    }
}

void Buzzer::keyPressSound() {
    // low single tone for key press
    static const int tones[] = {523};
    playTones(tones, sizeof(tones) / sizeof(tones[0]));
}

void Buzzer::alarmOff() {
    digitalWrite(_buzzerPin, LOW);
}