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
    Serial.print("onReceive called with "); 
    Serial.print(numBytes); 
    Serial.println(" bytes."); // Debug output

    if (numBytes == 6) {
        int year = Wire.read() + 2000; // year
        int month = Wire.read();       // month
        int day = Wire.read();         // day
        int hour = Wire.read();        // hours
        int minute = Wire.read();      // minutes
        int second = Wire.read();      // seconds
        currentTime = DateTime(year, month, day, hour, minute, second);

        // Debug output
        Serial.print("Received RTC data: ");
        Serial.print(year); Serial.print("-");
        Serial.print(month); Serial.print("-");
        Serial.print(day); Serial.print(" ");
        Serial.print(hour); Serial.print(":");
        Serial.print(minute); Serial.print(":");
        Serial.println(second);
    } else {
        Serial.println("Incorrect number of bytes received.");
    }
}

String UNOComm::getRtcData() {
    char buf1[] = "DD-MM-YYYY hh:mm:ss";
    return String(currentTime.toString(buf1));
}
