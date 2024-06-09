#include "UNOComm.h"
#include "constants.h"

#include <ArduinoJson.h>

using namespace constants;

// Initiate static instance pointer
UNOComm *UNOComm::instance = nullptr;

// Set pointer to current instance
UNOComm::UNOComm() : _lcd(nullptr), _buzzer(nullptr), _alarmActivated(false) {
    instance = this;
}

UNOComm::~UNOComm() {
    if (_lcd) {
        delete _lcd;
    }
    if (_buzzer) {
        delete _buzzer;
    }
  
}

void UNOComm::initialize() {
    Wire.begin(ARDUINO_I2C_ADDRESS); // Join I2C bus with address #8
    Wire.onReceive(onReceive);
    Wire.onRequest(onRequest);
}

void UNOComm::setLCD(LCD *lcd) {
    this->_lcd = lcd;
}

void UNOComm::onReceive(int numBytes) {
    if (instance != nullptr)
        instance->processI2CCommand(numBytes);
}

void UNOComm::processI2CCommand(int numBytes) {
    if (numBytes > 0) {
        char command = Wire.read();
        switch (command) {
        case 'R': // RTC Data
            instance->setRealTimeClock();
            break;
        case 'A': // Alarm Activation
        case 'D': // Alarm Deactivation
            instance->handleAlarmActivation(command);
            break;
        case 'P': // Pin Code Feedback
            instance->handlePinCodeFeedback();
            break;
        case 'K': // Keypad Data
            instance->handleKeypadData();
            break;
        default:
            break;
        }
    }
}

String UNOComm::getRealTimeClock() {
    return _dateTime;
}

void UNOComm::setRealTimeClock() {
    _dateTime = "";
    while (Wire.available()) {
        char c = Wire.read();
        _dateTime += c;
    }
    updateLCD();
}

void UNOComm::handleKeypadData() {
    char key = Wire.read();
    unsigned long currentMillis = millis();  // Current time

    // If the timeout has occurred, clear the pin code
    if (currentMillis - _lastKeypadInputTime > KEYPAD_TIMEOUT && strlen(_userInputtedPassword) > 0) {
        memset(_pinCode, 0, sizeof(_pinCode));
        memset(_userInputtedPassword, 0, sizeof(_userInputtedPassword));
        if (_lcd) {
            _lcd->setCursor(0, 1);
            _lcd->print("                ");
        }
    }
    // Update the pin code if a key is pressed
    if (isdigit(key) || key == 'D' || key == 'C') {
        _lastKeypadInputTime = currentMillis;

        // Handled the delete key
        if (key == 'D' && strlen(_userInputtedPassword) > 0) {
            _userInputtedPassword[strlen(_userInputtedPassword) - 1] = '\0';
            _pinCode[strlen(_pinCode) - 1] = '\0';
        } 
        // Handled the clear key
        else if (key == 'C') {
            memset(_userInputtedPassword, 0, sizeof(_userInputtedPassword));
            memset(_pinCode, 0, sizeof(_pinCode));
        } 
        // Handled number keys
        else if (strlen(_userInputtedPassword) < 4 && isdigit(key)) {
            size_t len = strlen(_userInputtedPassword);
            _userInputtedPassword[len] = key;
            _userInputtedPassword[len + 1] = '\0';
            _pinCode[len] = '*';
            _pinCode[len + 1] = '\0';
        }

        // Update the LCD
        if (_lcd) {
            _lcd->setCursor(0, 1);
            _lcd->print("                ");
            _lcd->setCursor(0, 1);
            _lcd->print(_pinCode);
        }
    }
}






void UNOComm::handleAlarmActivation(char command) {
    if (command == 'A') {
        _alarmActivated = true;
        displayTemporaryMessage("Alarm is active", 5000);
    } else if (command == 'D') {
        _alarmActivated = false;
        displayTemporaryMessage("Alarm is offline", 5000);
    }
}

void UNOComm::handlePinCodeFeedback() {
    bool success = Wire.read();
    int attemptsLeft = Wire.read();
    if (success) {
        displayTemporaryMessage("Correct pin", 3000);
    } else {
        displayTemporaryMessage("Wrong pin ", 1000);
        displayTemporaryMessage("Attempts left: " + String(attemptsLeft), 3000);
    }
}

void UNOComm::displayTemporaryMessage(const String &message, unsigned long duration) {
    if (_lcd) {
        _lcd->setCursor(0, 1);
        _lcd->print(message);
        _messageClearTime = millis() + duration;
        updateLCD();
    }
}

void UNOComm::updateLCD() {
    if (_lcd) {
        _lcd->setCursor(0, 0);
        _lcd->print(getRealTimeClock());
        if (millis() >= _messageClearTime && _messageClearTime != 0) {
            _lcd->setCursor(0, 1);
            _lcd->print("                ");
            _messageClearTime = 0;
        }
    }
}

void UNOComm::update() {
    if (millis() - _lastLCDUpdateTime >= _lcdUpdateInterval) {
        _lastLCDUpdateTime = millis();
        updateLCD();
    }
}

void UNOComm::sendLogDataToESP32() {
    if (!_sensorType || !_sensorId || !_sensorValue) {
        Serial.println("No log data to send to ESP32.");
        return;
    }

    // Skapa en strukturerad sträng
    String logData = _dateTime + "|" + 
                     *_sensorType + "|" + 
                     *_sensorId + "|" + 
                     (*_sensorValue ? "1" : "0");

    Serial.println("Sending log data to ESP32: " + logData);

    // Skicka varje tecken i strängen
    for (char c : logData) {
        Wire.write(c);
    }

    Wire.write('\0'); // Null-terminate the string

    // Kontrollera om minne är tilldelat innan det frigörs
    if (_sensorType) {
        delete _sensorType;
        _sensorType = nullptr;
    }

    if (_sensorId) {
        delete _sensorId;
        _sensorId = nullptr;
    }

    if (_sensorValue) {
        delete _sensorValue;
        _sensorValue = nullptr;
    }
}






void UNOComm::setSensorLog(const char* sensorType, const char* sensorId, bool value) {
    // Kontrollera om tidigare minne är tilldelat och frigör det
    if (_sensorType) {
        delete _sensorType;
        _sensorType = nullptr;
    }
    if (_sensorId) {
        delete _sensorId;
        _sensorId = nullptr;
    }
    if (_sensorValue) {
        delete _sensorValue;
        _sensorValue = nullptr;
    }

    // Tilldela nytt minne och sätt värdena
    _sensorType = new String(sensorType);
    _sensorId = new String(sensorId);
    _sensorValue = new bool(value);
}


void UNOComm::setBuzzer(Buzzer *buzzer) {
    this->_buzzer = buzzer;
}


void UNOComm::switchState() {
    if (_alarmActivated) {
        _alarmActivated = false;
        _lcd->print("Alarm Deactivated");
        _buzzer->alarmInactiveSound();
    } else {
        _alarmActivated = true;
        _lcd->print("Alarm Activated");
        _buzzer->alarmActiveSound();
    }
}

bool UNOComm::getState() const {
    return _alarmActivated;
}

void UNOComm::onRequest() {
    if (instance != nullptr)
        instance->sendLogDataToESP32();
}




