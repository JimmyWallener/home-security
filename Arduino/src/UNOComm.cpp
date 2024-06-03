#include "UNOComm.h"
#include "constants.h"

using namespace constants;

// Initiate static instance pointer
UNOComm *UNOComm::instance = nullptr;

// Set pointer to current instance
UNOComm::UNOComm() : lcd(nullptr), sensorLog(nullptr), buzzer(nullptr){
    instance = this;
}

UNOComm::~UNOComm() {
    if(lcd) {
        delete lcd;
    }
    if (sensorLog) {
        delete sensorLog;
    }
    if (buzzer) {
        delete buzzer;
    }
}

void UNOComm::initialize() {
    Wire.begin(ARDUINO_I2C_ADDRESS); // Join I2C bus with address #8
    Wire.onReceive(onReceive);
    Wire.onRequest(sendLogDataToESP32);
    Serial.println("UNOComm initialized and ready to receive data.");
}

void UNOComm::setLCD(LCD *lcd) {
    this->lcd = lcd;
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
            case 'S': // Alarm Status Request
                instance->handleAlarmStatusRequest();
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
    buzzer->keyPressSound();
    char key = Wire.read();

    if (key == 'D' && _pinCode.length() > 0) {
        Serial.println("Delete key pressed");
        _pinCode.remove(_pinCode.length() - 1, 1);
        _userInputtedPassword.remove(_userInputtedPassword.length() - 1, 1);
        Serial.println(_pinCode);
    } else if (key != 'D' && _pinCode.length() < 4) {
        _pinCode += '*';
        this->_userInputtedPassword += key;
        Serial.println(_pinCode);
    }

    // Update the display immediately
    if (lcd) {
        lcd->setCursor(0, 1);
        lcd->print("                ");  // Clear the line
        lcd->setCursor(0, 1);
        lcd->print(_pinCode);
    }

    if(_userInputtedPassword.length() == 4) {
        if(checkPassword()) {
            lcd->setCursor(0, 1);
            switchState();
            _userInputtedPassword.remove(0, 4);
        } else {
            lcd->setCursor(0, 1);
            lcd->print("Wrong Password");
            buzzer->wrongPassword();
        }
    }

    // Clear _pinCode after 4 digits and displayMessage timer has passed
    if (_pinCode.length() == 4 && millis() >= messageClearTime) {
        _pinCode.remove(0, 4);
        _userInputtedPassword.remove(0, 4);
    }
    
    // Update the message clear time
    messageClearTime = millis() + 1000;  // Set the duration for the message to be displayed
}

void UNOComm::handleAlarmActivation(char command) {
    if(command == 'A') {
        displayTemporaryMessage("Alarm is active", 5000);
    } else {
        displayTemporaryMessage("Alarm is offline", 5000);
    }
}

void UNOComm::handleAlarmStatusRequest() {
    // TODO: Implement alarm status request handling. Send alarm status to ESP32
    // For now, just print a message
    Serial.println("Alarm status requested.");
}

void UNOComm::handlePinCodeFeedback() {
    // TODO: Implement pin code feedback handling.  Payload from ESP32 is a boolean and an integer: success and attempts left
    // For now, just print a message
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
    if(lcd) {
        lcd->setCursor(0, 1);
        lcd->print(message);
        messageClearTime = millis() + duration;
        updateLCD();
    }
}

void UNOComm::updateLCD() {
    if (lcd) {
        
        lcd->setCursor(0, 0);
        lcd->print(getRealTimeClock());

        // Clear second line after message duration has passed
        if (millis() >= messageClearTime && messageClearTime != 0) {
            lcd->setCursor(0, 1);
            lcd->print("                ");
            messageClearTime = 0;
        }
    }
}

void UNOComm::update() {
    if (millis() - _lastLCDUpdateTime >= lcdUpdateInterval) {
        _lastLCDUpdateTime = millis();
        updateLCD();
    }
}


void UNOComm::sendLogDataToESP32() {

    String sensorLog = "Testing";

    for (char c : sensorLog) {
        Wire.write(c);
    }
}

void UNOComm::setSensorLog(SensorLog *sensorLog) {
    this->sensorLog = sensorLog;
}

void UNOComm::setBuzzer(Buzzer *buzzer) {
    this->buzzer = buzzer;
}

bool UNOComm::checkPassword() {
    for(int i = 0; i < 4; i++) {
        if(_userInputtedPassword[i] != _password[i]) {
            return false; 
        }
    }
    return true;
}

void UNOComm::switchState() {
    switch (this->_state)
            {
            case 0:
                this->_state = 1;
                lcd->print("Alarm Activated");
                break;

            case 1:
                this->_state = 0;
                lcd->print("Deactivated");
                break;

            default:
                break;
            }
}

bool UNOComm::getState() {
    if(this->_state == 1) {
        return true;
    } else {
        return false;
    }
}