#include "RealTimeClock.h"

RealTimeClock::RealTimeClock() {}

void RealTimeClock::begin() {
    if (!rtc.begin(&Wire1)) {  // Use Wire1 for RTC communication
        Serial.println("Couldn't find RTC");
        while (1);
    }
    
    if (rtc.lostPower()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}
// Format the current time as a string and return it
String RealTimeClock::getFormattedTime() {
    DateTime now = rtc.now();
    char buf1[] = "DD-MM-YYYY hh:mm:ss";
    return String(now.toString(buf1));
}

// Get the current time from the RTC
DateTime RealTimeClock::getCurrentTime() {
    return rtc.now();
}
