#include "LCD.h"
#include <Arduino.h>



void LCD::LCDInitialize()
{
    int columnIndex = 16;
    int rowIndex = 2;
    // Set up the LCD's number of columns & rows
    lcd.begin(columnIndex, rowIndex);
}

void LCD::LCDUpdate(const DateTimeInfo& dt)
{
    // Set the cursor to column 0, line 0
    lcd.setCursor(0, 0);
  
    // Print abbreviated day of the week
    lcd.print(_daysOfTheWeek[dt.dayOfWeek]);

    // Space between day and date
    lcd.print(" ");

    // Date in DD/MM format
    lcd.print(dt.day < 10 ? "0" : "");
    lcd.print(dt.day);
    lcd.print("/");
    lcd.print(dt.month < 10 ? "0" : "");
    lcd.print(dt.month);
    lcd.print(" ");
    lcd.print(dt.year);

    // Move to the next line and print the time in HH:MM format
    lcd.setCursor(0, 1);

    // Time in HH:MM format
    lcd.print(dt.hour < 10 ? "0" : "");
    lcd.print(dt.hour);
    lcd.print(":");
    lcd.print(dt.minutes < 10 ? "0" : "");
    lcd.print(dt.minutes);
}

// Skriven av: Joakim Sjöström