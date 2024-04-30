#include "Rtc.h"

RTC::RTC() {}

void RTC::RTCInitialize()
{
    lcdDisplay.LCDInitialize();
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1);
    }

    // Initial sync from RTC
    SyncTimeFromRTC();
}

void RTC::RTCUpdate()
{
    // Sync time from RTC to our currentTime structure
    SyncTimeFromRTC();

    // Update the LCD
    lcdDisplay.LCDUpdate(currentTime);
}

void RTC::SetTime(const DateTimeInfo& dt)
{
    currentTime = dt;
    SyncTimeToRTC();
}

DateTimeInfo RTC::GetTime()
{
    SyncTimeFromRTC();
    return currentTime;
}

void RTC::SyncTimeToRTC()
{
    // Convert the DateTimeInfo to DateTime format and set it to RTC
    rtc.adjust(DateTime(currentTime.year, currentTime.month, currentTime.day,
                        currentTime.hour, currentTime.minutes, currentTime.seconds));
}

void RTC::SyncTimeFromRTC()
{
    DateTime now = rtc.now();
    currentTime.year = now.year();
    currentTime.month = now.month();
    currentTime.day = now.day();
    currentTime.hour = now.hour();
    currentTime.minutes = now.minute();
    currentTime.seconds = now.second();

    // Update the day of the week
    currentTime.dayOfWeek = now.dayOfTheWeek();
}