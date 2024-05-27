#include "UNOComm.h"
#include "constants.h"

using namespace constants;

// Initiate static instance pointer
UNOComm *UNOComm::instance = nullptr;

// Set pointer to current instance
UNOComm::UNOComm() : lcd(nullptr), sensorLog(nullptr){
    instance = this;
}

UNOComm::~UNOComm() {
    if(lcd) {
        delete lcd;
    }
    if (sensorLog) {
        delete sensorLog;
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


void UNOComm::sendLogDataToESP32() {

    String sensorLog = "Testing";

    for (char c : sensorLog) {
        Wire.write(c);
    }
}