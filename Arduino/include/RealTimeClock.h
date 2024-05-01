#pragma once
#include <RTClib.h>
#include "Struct/DateTimeInfo.h"
#include "LCD.h"

class RTC
{
public:
    RTC();
    void RTCInitialize();
    void RTCUpdate();

    // Getter & Setter
    DateTimeInfo GetTime();
    void SetTime(const DateTimeInfo& dt);

private:
    RTC_DS1307 rtc;
    DateTimeInfo currentTime;
    LCD lcdDisplay;

    // Helper function to sync the structured time to RTC hardware
    void SyncTimeToRTC();
    // Helper function to sync time from RTC hardware to the structured time
    void SyncTimeFromRTC();
};

// Skriven av: Joakim Sjöström