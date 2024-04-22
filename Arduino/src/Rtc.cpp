#include <Rtc.h>

RTC::RTC()
{
}

void RTC::RTCSetup()
{
    // Set up the LCD's number of columns & rows
    lcd.begin(16, 2);

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1);
    }

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void RTC::RTCLoop()
{
    // Get the current time from the RTC
    DateTime now = rtc.now();
    // Store current time variables
    year = now.year();
    month = now.month();
    day = now.day();
    hour = now.hour();
    minutes = now.minute();
    seconds = now.second();

    // Set the cursor to column 0, line 0
    lcd.setCursor(0, 0);

    if(day<10)
    {
        lcd.print(" ");
    }
    lcd.print(day);
    lcd.print("/");
    if(month<10)
    {
        lcd.print(" ");
    }
    lcd.print(month);
    lcd.print("/");
    lcd.print(year);

    // Print time in HH:MM:SS
    // Add leading zeroes if minutes or seconds are less than ten to keep spacing consistent
    lcd.setCursor(0, 1);
    if(hour<10)
    {
        lcd.print(" ");
    }
    lcd.print(hour);
    lcd.print(":");
    if(minutes<10)
    {
        lcd.print("0");
    }
    lcd.print(minutes);
    lcd.print(":");
    if(seconds<10)
    {
        lcd.print("0");
    }
    lcd.print(seconds);
}