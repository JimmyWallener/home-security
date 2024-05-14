#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H

#include <Wire.h>
#include <RTClib.h>

class RealTimeClock {
private:
    RTC_DS3231 rtc;
public:
    RealTimeClock();
    void begin();
    String getFormattedTime();
    DateTime getCurrentTime();
};

#endif // REALTIMECLOCK_H
