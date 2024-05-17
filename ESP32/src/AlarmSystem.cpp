#include "../include/AlarmSystem.h"

bool AlarmSystem::checkPassword() {
    
    for(int i = 0; i < 5; i++) {
        if(this->_currentPassword[i] != _secretPassword[i]) {
            return false;
        }
    }
    return true;
}

// password kommer att ligga i db inte i programmet

void AlarmSystem::getPassword(Keypad &numpad) {
    int counter = 0;

    while(counter < 5) {
        char currentKey = numpad.getKey();
        if(currentKey) {
            this->_currentPassword[counter] = currentKey;
            Serial.println(currentKey);
            counter++;
        }
    }
}

void AlarmSystem::changeAlarmState() {
    bool passwordCheck = AlarmSystem::checkPassword();

    if(passwordCheck) {
        switch (this->_currentState)
        {
        case false:
            this->_currentState = alarmState::active;
            Serial.println("Alarm will activate in 5 seconds"); // Print this to lcd instead.
            break;
        
        case true:
            this->_currentState = alarmState::inActive;
            Serial.println("Alarm has been de-activated"); // Print this to lcd instead.
            break;

        default:
            break;
        }
    } else {
        Serial.println("Incorrect Password!");
    }
}

// snygga till!
bool AlarmSystem::getState() {
    if(this->_currentState == alarmState::active) {
        return true;
    } else {
        return false;
    }
}
