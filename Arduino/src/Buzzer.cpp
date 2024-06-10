#include "Buzzer.h"

/**
 * @brief Constructor for the Buzzer class.
 * 
 * Initializes the buzzer with the specified pin and delay time between tones.
 * 
 * @param buzzerPin The pin number connected to the buzzer.
 * @param delayTime The time delay between tone changes in milliseconds.
 */
Buzzer::Buzzer(uint8_t buzzerPin, uint16_t delayTime)
    : _buzzerPin(buzzerPin), _delayTime(delayTime), _tones(nullptr), _tonesLength(0), _currentToneIndex(0), _isPlaying(false), _lastToneChangeTime(0) {}

/**
 * @brief Destructor for the Buzzer class.
 * 
 * Stops any tone currently being played by the buzzer.
 */
Buzzer::~Buzzer() {
    noTone(_buzzerPin);
}

/**
 * @brief Initializes the buzzer.
 * 
 * Sets the buzzer pin as an OUTPUT and ensures no tone is playing.
 */
void Buzzer::initialize() {
    pinMode(_buzzerPin, OUTPUT);
    noTone(_buzzerPin);
}

/**
 * @brief Plays an alarm sound with a sequence of tones.
 */
void Buzzer::playAlarm() {
    static const int tones[] = {698, 784, 831};
    for(int i = 0; i < 3; i++) {
        tone(_buzzerPin, tones[i]);
        delay(40);
    } 
}

/**
 * @brief Plays a sequence of tones to indicate that the alarm is active.
 */
void Buzzer::alarmActiveSound() {
    static const int tones[] = {440, 466, 494, 523, 554};
    playTones(tones, sizeof(tones) / sizeof(tones[0]));
}

/**
 * @brief Plays a sequence of tones to indicate that the alarm is inactive.
 * 
 * Stops any currently playing tone after the sequence is played.
 */
void Buzzer::alarmInactiveSound() {
    static const int tones[] = {554, 523, 494, 466, 440};
    playTones(tones, sizeof(tones) / sizeof(tones[0]));
    noTone(_buzzerPin);
}

/**
 * @brief Plays a short sequence of tones to indicate an incorrect password entry.
 */
void Buzzer::wrongPassword() {
    static const int tones[] = {554, 440};
    playTones(tones, sizeof(tones) / sizeof(tones[0]));
}

/**
 * @brief Plays a sequence of tones stored in an array.
 * 
 * This function sets up the tones array and starts playing the first tone.
 * 
 * @param tones An array of tone frequencies to play.
 * @param tonesLength The number of tones in the array.
 */
void Buzzer::playTones(const int* tones, int tonesLength) {
    _tones = tones;
    _tonesLength = tonesLength;
    _currentToneIndex = 0;
    _isPlaying = true;
    _lastToneChangeTime = millis();
    tone(_buzzerPin, _tones[_currentToneIndex]);
}

/**
 * @brief Updates the buzzer state and changes tones if necessary.
 * 
 * This function checks if the delay time has elapsed and changes to the next tone if it has.
 * It should be called repeatedly to ensure continuous tone playback.
 */
void Buzzer::update() {
    if (_isPlaying && (millis() - _lastToneChangeTime) >= _delayTime) {
        _lastToneChangeTime = millis();
        noTone(_buzzerPin);
        _currentToneIndex++;
        if (_currentToneIndex < _tonesLength) {
            tone(_buzzerPin, _tones[_currentToneIndex]);
        } else {
            _isPlaying = false;
            noTone(_buzzerPin);
        }
    }
}

/**
 * @brief Plays a single low tone to indicate a key press.
 */
void Buzzer::keyPressSound() {
    static const int tones[] = {523};
    playTones(tones, sizeof(tones) / sizeof(tones[0]));
}

/**
 * @brief Turns off the buzzer.
 * 
 * Sets the buzzer pin to LOW, stopping any sound.
 */
void Buzzer::alarmOff() {
    digitalWrite(_buzzerPin, LOW);
}
