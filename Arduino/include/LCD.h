#pragma once
#include <LiquidCrystal.h>
#include "Struct/DateTimeInfo.h"

class LCD
{
public:
    LCD();
    void LCDInitialize();
    void LCDUpdate(const DateTimeInfo& dt);

private:
    // Initialize the LCD library by associating any needed LCD interface pin
    // with the arduino pin number it is connected to
    LiquidCrystal lcd = LiquidCrystal(12, 11, 5, 4, 3, 2);
    const char* _daysOfTheWeek[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
};

// Skriven av: Joakim Sjöström