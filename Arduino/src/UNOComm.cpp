#include "UNOComm.h"

UNOComm* UNOComm::instance = nullptr;

UNOComm::UNOComm() : lcd(nullptr) {
    instance = this;
}

void UNOComm::begin() {
    Wire.begin(8); // Join I2C bus with address #8
    Wire.onReceive(onReceiveWrapper);
    Serial.println("UNOComm initialized and ready to receive data.");
}

void UNOComm::setLCD(LCD *lcd) {
    this->lcd = lcd;
}

void UNOComm::onReceiveWrapper(int numBytes) {
    Serial.print("onReceiveWrapper called with "); 
    Serial.print(numBytes); 
    Serial.println(" bytes."); // Debug output
    if (instance != nullptr) {
        instance->onReceive(numBytes);
    }
}

void UNOComm::onReceive(int numBytes) {
   if (numBytes > 0) {
       char command = Wire.read();

       switch (command) {
              case 'R': // RTC Data
                handleRTCData();
                break;
              case 'T': // Trigger Event
                handleTriggerEvent();
                break;
              case 'J': // JSON Data
                handleJsonData();
                break;
              case 'A': // Alarm Activation
                handleAlarmActivation();
                break;
              case 'D': // Alarm Deactivation
                handleAlarmDeactivation();
                break;
              case 'S': // Alarm Status Request
                handleAlarmStatusRequest();
                break;
              case 'P': // Pin Code Feedback
                handlePinCodeFeedback();
                break;
              default:
                Serial.println("Invalid command received.");
                break;
         
       }
   }
}


String UNOComm::getRtcData() {
    char buf1[] = "DD-MM-YYYY hh:mm:ss";
    return String(currentTime.toString(buf1));
}

void UNOComm::handleRTCData() {

    uint8_t year = Wire.read();
    uint8_t month = Wire.read();
    uint8_t day = Wire.read();
    uint8_t hour = Wire.read();
    uint8_t minute = Wire.read();
    uint8_t second = Wire.read();
    currentTime = DateTime(year + 2000, month, day, hour, minute, second);
    
}

 // Trigger Event if needed, method not complete
void UNOComm::handleTriggerEvent() {
    // TODO: Implement trigger event handling if needed. Payload from ESP32 is a string: sensor name
    char event[10];
    Wire.readBytes(event, 10);
    Serial.print("Trigger Event received: ");
    Serial.println(event);
}

// Method not complete and not used
void UNOComm::handleJsonData() {
    // TODO: Implement JSON data handling. Payload from ESP32 is a JSON string: {"key": "value"}
    char json[100];
    Wire.readBytes(json, 100);
    Serial.print("JSON Data received: ");
    Serial.println(json);
}

void UNOComm::handleAlarmActivation() {
    // TODO: Create a countdown displayed in the LCD, then activate the alarm. But commented out disrupts date and time display
    // For now, just print a message
    // create a 5 second count down displayed in lcd
    /* displayTemporaryMessage("Alarm activated in 5 seconds.", 1000);
    delay(1000);
    displayTemporaryMessage("Alarm activated in 4 seconds.", 1000);
    delay(1000);
    displayTemporaryMessage("Alarm activated in 3 seconds.", 1000);
    delay(1000);
    displayTemporaryMessage("Alarm activated in 2 seconds.", 1000);
    delay(1000);
    displayTemporaryMessage("Alarm activated in 1 seconds.", 1000);
    delay(1000); */
    displayTemporaryMessage("Alarm activated!", 5000);
}

void UNOComm::handleAlarmDeactivation() {
    // TODO: Implement alarm deactivation. 
    // For now, just print a message
    displayTemporaryMessage("Alarm deactivated.", 3000);
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
    if(lcd) {
        lcd->setCursor(0, 0);
        lcd->print(getRtcData());

        // Clear second line after message duration has passed
        if (millis() >= messageClearTime && messageClearTime != 0) {
            lcd->setCursor(0, 1);
            lcd->print("                ");
            messageClearTime = 0;
        }
    }
}