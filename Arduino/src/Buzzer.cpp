/**
 * @file Buzzer.cpp
 * @author Leo Katakalidis
 * @brief Implementation of the Buzzer class
 * @date 2024-05-06
 * 
 * 
 */


#include "Buzzer.h"

/**
 * @brief Construct a new Buzzer:: Buzzer object
 * @details This method constructs a new Buzzer object with the given pin and delay time
 * @param buzzerPin 
 * @param delayTime 
 */
Buzzer::Buzzer(uint8_t buzzerPin, uint16_t delayTime) : _buzzerPin(buzzerPin), _delayTime(delayTime){}


/**
 * @brief Method to initialize the buzzer
 * @details This method initializes the buzzer by setting the pin mode to OUTPUT and turning off the buzzer
 * @return void
 */
void Buzzer::initialize(){
    pinMode(_buzzerPin, OUTPUT);
    noTone(_buzzerPin);
}

/**
 * @brief Method to play the alarm sound
 * @details This method plays the alarm sound by playing a sequence of tones
 * @return void
 */
void Buzzer::playAlarm(){
    tone(_buzzerPin, 698); // F
    delay(_delayTime - 70);
    tone(_buzzerPin, 784); // G
    delay(_delayTime - 70);
    tone(_buzzerPin, 831); // A FLAT
    delay(_delayTime - 70);
}

/**
 * @brief Method to play the active alarm sound
 * @details This method plays the active alarm sound by playing a sequence of tones
 * @return void
 */
void Buzzer::alarmActiveSound(){
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

/**
 * @brief Method to play the inactive alarm sound
 * @details This method plays the inactive alarm sound by playing a sequence of tones
 * @return void
 */
void Buzzer::alarmInactiveSound(){
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
