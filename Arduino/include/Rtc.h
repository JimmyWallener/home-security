#pragma once
#include <RTClib.h>
#include <LiquidCrystal.h>

// Time structure to hold the date and time information
struct DateTimeInfo
{
    int year;
    int month;
    int day;
    int hour;
    int minutes;
    int seconds;
    int dayOfWeek;
};

class RTC
{
public:
    RTC();
    void RTCInitialize();
    void RTCUpdate();

    // Setter & Getter
    void SetTime(const DateTimeInfo& dt);
    DateTimeInfo GetTime();

private:
    RTC_DS1307 rtc;
    // LiquidCrystal lcd;
    DateTimeInfo currentTime;

    // Helper function to sync the structured time to RTC hardware
    void SyncTimeToRTC();
    // Helper function to sync time from RTC hardware to the structured time
    void SyncTimeFromRTC();
    const char* daysOfTheWeek[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    // Initialize the LCD library by associating any needed LCD interface pin
    // with the arduino pin number it is connected to
    const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
    LiquidCrystal lcd = LiquidCrystal(rs, en, d4, d5, d6, d7);
};