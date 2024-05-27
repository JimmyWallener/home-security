#include <Arduino.h>
#include <Keypad.h>

enum alarmState {
    active = true,
    inActive = false
};

class AlarmSystem {
    private: 
    const String _secretPassword = "12345"; // This will later come from the database?
    char _currentPassword[5];
    alarmState _currentState = alarmState::inActive; 

    public:
    bool getState(); 
    bool checkPassword();
    void getPassword(Keypad&);
    void changeAlarmState();
    // void playAlarm(Buzzer &Buzzer); // this needs to send a message to Arduino to play buzzer instead of doing it itself
    void awaitSensorData();
};