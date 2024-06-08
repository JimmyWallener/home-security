#include "UNOComm.h"
#include "constants.h"

using namespace constants;

// Initiate static instance pointer
UNOComm *UNOComm::instance = nullptr;

// Set pointer to current instance
UNOComm::UNOComm() : _lcd(nullptr), _sensorLog(nullptr), _buzzer(nullptr), _alarmActivated(false) {
    instance = this;
}

UNOComm::~UNOComm() {
    if (_lcd) {
        delete _lcd;
    }
    if (_sensorLog) {
        delete _sensorLog;
    }
    if (_buzzer) {
        delete _buzzer;
    }
}

void UNOComm::initialize() {
    Wire.begin(ARDUINO_I2C_ADDRESS); // Join I2C bus with address #8
    Wire.onReceive(onReceive);
    Wire.onRequest(onRequest);
    Serial.println("UNOComm initialized and ready to receive data.");
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
        case 'T': // Trigger Event
            instance->handleTriggerEvent();
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
            Serial.println("Invalid command received.");
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

void UNOComm::handleTriggerEvent() {
    byte eventLength = Wire.read();
    char event[eventLength + 1];
    Wire.readBytes(event, eventLength);
    event[eventLength] = '\0';
    Serial.print("Trigger Event received: ");
    Serial.println(event);
}

void UNOComm::handleKeypadData() {
    _buzzer->keyPressSound();
    char key = Wire.read();
    _lastKeypadInputTime = millis();

    if (key == 'D' && !_userInputtedPassword.length() == 0) {
        Serial.println("Delete key pressed");
        _userInputtedPassword.remove(_userInputtedPassword.length() - 1);
    } else if (key == 'C') {
        _userInputtedPassword = "";
        Serial.println("Pin code cleared.");
    } else if (_userInputtedPassword.length() < 4 && isdigit(key)) {
        _userInputtedPassword += key; 
        _pinCode += '*'; 
        Serial.print("Current pin: ");
        Serial.println(_userInputtedPassword);
    }

    if (_lcd) {
        _lcd->setCursor(0, 1);
        _lcd->print("                "); 
        _lcd->setCursor(0, 1);
        _lcd->print(_pinCode);     }

    // clear the pin code after 4 digits and after 5 seconds or if the last input was more than 5 seconds ago
    if ((_userInputtedPassword.length() == 4 && millis() - _lastKeypadInputTime >= 5000) || 
        (millis() - _lastKeypadInputTime >= 5000)) {
        _pinCode = "";
        _userInputtedPassword = "";
        _lcd->setCursor(0, 1);
        _lcd->print("                ");
        Serial.println("Pin code input cleared.");
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
    if (_sensorLog == nullptr) {
        Serial.println("SensorLog is null, nothing to send.");
        return;
    }

    String logData = _sensorLog->toJson();
    Serial.println("Sending log data to ESP32: " + logData);

    if (logData.length() == 0) {
        Serial.println("Log data is empty, not sending.");
        return;
    }
    for (char c : logData) {
        Wire.write(c);
    }
    Wire.write('\0');  // Null-terminate the string
    Wire.endTransmission();

    _sensorLog = nullptr;
}

void UNOComm::setSensorLog(SensorLog* sensorLog) {
    if (sensorLog == nullptr) {
        Serial.println("Trying to set a null SensorLog.");
        return;
    }

    _sensorLog = sensorLog;
    Serial.println("SensorLog set with data: " + _sensorLog->toJson());
}

void UNOComm::setBuzzer(Buzzer *buzzer) {
    this->_buzzer = buzzer;
}

SensorLog* UNOComm::getSensorLog() {
    return _sensorLog;
}

void UNOComm::switchState() {
    if (_alarmActivated) {
        _alarmActivated = false;
        _lcd->print("Alarm Deactivated");
        Serial.println("Switching state to Deactivated");
    } else {
        _alarmActivated = true;
        _lcd->print("Alarm Activated");
        Serial.println("Switching state to Activated");
    }
}

bool UNOComm::getState() const {
    Serial.print("Current state is: ");
    Serial.println(_alarmActivated ? "Activated" : "Deactivated");
    return _alarmActivated;
}

void UNOComm::onRequest() {
    if (instance != nullptr)
        instance->sendLogDataToESP32();
}
