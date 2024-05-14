#include "UNOComm.h"

UNOComm* UNOComm::instance = nullptr; // Initiate static member

// Constructor
UNOComm::UNOComm() {
    instance = this; // Point to the current instance
}

// Initiate the I2C communication
void UNOComm::begin() {
    Wire.begin(8); // Join I2C bus with address #8
    Wire.onReceive(onReceiveWrapper); // Use a static method as a wrapper
}

// This static method is called when the UNO receives data from the ESP32
void UNOComm::onReceiveWrapper(int numBytes) {
    if (instance != nullptr) {
        instance->onReceive(numBytes); // Call the instance method
    }
}

// This method is called when the UNO receives data from the ESP32
void UNOComm::onReceive(int numBytes) {
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
    }
}

// Get the current time from the RTC
String UNOComm::getRtcData() {
    char buf1[] = "DD-MM-YYYY hh:mm:ss";
    return String(currentTime.toString(buf1));
}
