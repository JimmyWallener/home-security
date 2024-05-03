#pragma once
#include <LiquidCrystal.h>
#include "Struct/DateTimeInfo.h"

class LCD
{
public:
    LCD() : lcd(-1, -1, -1, -1, -1, -1){};
    LCD(uint8_t rs, uint8_t en, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) : lcd(rs, en, d4, d5, d6, d7){};
    void LCDInitialize();
    void LCDUpdate(const DateTimeInfo& dt);

private:
    // Initialize the LCD library by associating any needed LCD interface pin
    // with the arduino pin number it is connected to
    LiquidCrystal lcd;
    const char *_daysOfTheWeek[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
};

// Skriven av: Joakim Sjöström