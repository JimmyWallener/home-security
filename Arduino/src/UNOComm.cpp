#include "UNOComm.h"
#include "constants.h"

using namespace constants;

// Initiate static instance pointer
UNOComm *UNOComm::instance = nullptr;

// Set pointer to current instance
UNOComm::UNOComm() : _lcd(nullptr), _sensorLog(nullptr), _buzzer(nullptr){
    instance = this;
}

UNOComm::~UNOComm() {
    if(_lcd) {
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
    if(instance != nullptr)
        instance->processI2CCommand(numBytes);
}

void UNOComm::processI2CCommand(int numBytes){
    if (numBytes > 0)
        {
            char command = Wire.read();

            switch (command)
            {
            case 'R': // RTC Data
                instance->setRealTimeClock();
                break;
            case 'T': // Trigger Event
                instance->handleTriggerEvent();
                break;
            case 'A': // Alarm Activation
                instance->handleAlarmActivation(command);
                break;
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
        while (Wire.available())
        {
            char c = Wire.read();
            _dateTime += c;
        }
    updateLCD();
}

 // Trigger Event if needed, method not complete
void UNOComm::handleTriggerEvent() {
    // TODO: Implement trigger event handling if needed. Payload from ESP32 is a string: sensor name
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

    if (key == 'D' && _pinCode.length() > 0) {
        Serial.println("Delete key pressed");
        _pinCode.remove(_pinCode.length() - 1, 1);
        _userInputtedPassword.remove(_userInputtedPassword.length() - 1, 1);
    } else if (key != 'D' && _pinCode.length() < 4) {
        if (key == 'C') {
            _pinCode = "";
            _userInputtedPassword = "";
        } else {
            _pinCode += '*';
            _userInputtedPassword += key;
        }
       
    }

    // Update the display immediately
    if (_lcd) {
        _lcd->setCursor(0, 1);
        _lcd->print("                ");  // Clear the line
        _lcd->setCursor(0, 1);
        _lcd->print(_pinCode);
    }

    if(_userInputtedPassword.length() == 4) {
        if(_alarmActivated) {
            _lcd->setCursor(0, 1);
            switchState();
            _userInputtedPassword.remove(0, 4);
        } else {
            _lcd->setCursor(0, 1);
            _lcd->print("Wrong Password");
            _buzzer->wrongPassword();
        }
    }

    // Clear _pinCode after 4 digits and displayMessage timer has passed
    if (_pinCode.length() == 4 && millis() >= _messageClearTime) {
        _pinCode.remove(0, 4);
        _userInputtedPassword.remove(0, 4);
    }
    
    // Update the message clear time
    _messageClearTime = millis() + 1000;  // Set the duration for the message to be displayed
}

void UNOComm::handleAlarmActivation(char command) {
    if(command == 'A') {
        _alarmActivated = true;
        displayTemporaryMessage("Alarm is active", 5000);
    } else {
        _alarmActivated = false;
        displayTemporaryMessage("Alarm is offline", 5000);
    }
}


void UNOComm::handlePinCodeFeedback() {
    bool success = Wire.read();
    int attemptsLeft = Wire.read();
    if(success) {
       displayTemporaryMessage("Pin code correct.", 3000);
    }
    else {
       displayTemporaryMessage("Pin code incorrect. Attempts left: " + String(attemptsLeft), 3000);
    }
}

void UNOComm::displayTemporaryMessage(const String &message, unsigned long duration) {
    if(_lcd) {
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

        // Clear second line after message duration has passed
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
    Serial.println("Log data length: " + String(logData.length()));

    if (logData.length() == 0) {
        Serial.println("Log data is empty, not sending.");
        return;
    }
/* 
    for (char c : logData) {
        Wire.write(c);
    } 
    Wire.write('\0');  // Null-terminate the string
    Wire.endTransmission();*/

    Serial.println(logData);

    // set _sensorLog to nullptr after sending the data
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
    if (this->_state == 0) {
        this->_state = 1;
        _lcd->print("Alarm Activated");
        Serial.println("Switching state to Activated");
    } else {
        this->_state = 0;
        _lcd->print("Deactivated");
        Serial.println("Switching state to Deactivated");
    }
}

bool UNOComm::getState() {
    Serial.print("Current state is: ");
    Serial.println(this->_state);
   return this->_state == 1;
}

void UNOComm::onRequest() {
    if(instance != nullptr)
        instance->sendLogDataToESP32();
}