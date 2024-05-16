#include "UNOComm.h"

UNOComm* UNOComm::instance = nullptr;

UNOComm::UNOComm() {
    instance = this;
}

void UNOComm::begin() {
    Wire.begin(8); // Join I2C bus with address #8
    Serial.println(Wire.peek());
    Wire.onReceive(onReceiveWrapper);
    Serial.println("UNOComm initialized and ready to receive data.");
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
    Serial.println("RTC Data received.");
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
    // TODO: Implement alarm activation & write to LCD display.
    // For now, just print a message
    Serial.println("Alarm activated.");
}

void UNOComm::handleAlarmDeactivation() {
    // TODO: Implement alarm deactivation & write to LCD display. 
    // For now, just print a message
    Serial.println("Alarm deactivated.");
}

void UNOComm::handleAlarmStatusRequest() {
    // TODO: Implement alarm status request handling. Send alarm status to ESP32
    // For now, just print a message
    Serial.println("Alarm status requested.");
}

void UNOComm::handlePinCodeFeedback() {
    // TODO: Implement pin code feedback handling.  Payload from ESP32 is a boolean and an integer: success and attempts left
    // For now, just print a message
    Serial.println("Pin code feedback received.");
}