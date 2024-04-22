#pragma once
#include <RTClib.h>
#include <LiquidCrystal.h>

class RTC
{
public:
    RTC();
    void RTCSetup();
    void RTCLoop();

private:
    RTC_DS1307 rtc;

    char daysOfTheWeek[7][12] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    int year;
    int month;
    int day;
    int hour;
    int minutes;
    int seconds;

    // Initialize the LCD library by associating any needed LCD interface pin
    // with the arduino pin number it is connected to
    const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
    LiquidCrystal lcd = LiquidCrystal(rs, en, d4, d5, d6, d7);
};