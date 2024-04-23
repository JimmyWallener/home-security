#include <Rtc.h>

RTC::RTC() {}

void RTC::RTCInitialize()
{
    // Set up the LCD's number of columns & rows
    lcd.begin(16, 2);

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

    // Set the cursor to column 0, line 0
    lcd.setCursor(0, 0);

    // Print abbreviated day of the week
    lcd.print(daysOfTheWeek[currentTime.dayOfWeek]);

    // Space between day and date
    lcd.print(" ");

    // Date in DD/MM format
    lcd.print(currentTime.day < 10 ? "0" : "");
    lcd.print(currentTime.day);
    lcd.print("/");
    lcd.print(currentTime.month < 10 ? "0" : "");
    lcd.print(currentTime.month);
    lcd.print(" ");
    lcd.print(currentTime.year);

    // Move to the next line and print the time in HH:MM format
    lcd.setCursor(0, 1);

    // Time in HH:MM format
    lcd.print(currentTime.hour < 10 ? "0" : "");
    lcd.print(currentTime.hour);
    lcd.print(":");
    lcd.print(currentTime.minutes < 10 ? "0" : "");
    lcd.print(currentTime.minutes);
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