#include "RealTimeClock.h"
#include <Wire.h>

RealTimeClock::RealTimeClock() {}

/**
 * @brief Initialize the Real Time Clock (RTC) and begin communication using Wire1.
 * 
 * This method initializes the RTC module and sets up the I2C communication on the 
 * specified SDA and SCL pins. If the RTC module is not found or has lost power, 
 * it adjusts the RTC to the current compile date and time.
 */
void RealTimeClock::begin() {
    Wire1.begin(10, 11); // SDA, SCL pins for RTC
    rtc.begin(&Wire1);
    if (!rtc.begin(&Wire1)) {  // Use Wire1 for RTC communication
        Serial.println("Couldn't find RTC");
        while (1); // Loop indefinitely if RTC is not found
    }

    if (rtc.lostPower()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Adjust to compile date and time
    }
}

/**
 * @brief Get the current time formatted as a string.
 * 
 * This method retrieves the current time from the RTC and formats it as a 
 * string in the format "DD-MM-YYYY hh:mm:ss".
 * 
 * @return A formatted string representing the current time.
 */
String RealTimeClock::getFormattedTime() {
    DateTime now = rtc.now();
    char buf1[] = "DD-MM-YYYY hh:mm:ss";
    return String(now.toString(buf1));
}

/**
 * @brief Get the current DateTime object from the RTC.
 * 
 * This method retrieves the current time from the RTC and returns it as a 
 * DateTime object, which can be used for more precise time manipulations.
 * 
 * @return The current DateTime object from the RTC.
 */
DateTime RealTimeClock::getCurrentTime() {
    return rtc.now();
}
