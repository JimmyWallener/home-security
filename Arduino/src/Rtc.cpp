#include <Rtc.h>
#include <LiquidCrystal.h>

RTC::RTC()
{
    RTCSetup();
}

void RTC::RTCSetup()
{
    // Set up the LCD's number of columns & rows
    lcd.begin(16, 2);

    // if (!rtc.begin())
    // {
    //     Serial.println("Couldn't find RTC");
    //     Serial.flush();
    //     while (1);
    // }

    // if (!rtc.isrunning())
    // {
    //     Serial.println("RTC is not running, let's set the time!");
    //     /* When time needs to be set on a new device, or after a power loss,
    //     the following line sets the RTC to the date & time this sketch was compileud */
    //     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // }
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
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


    // Print formatted date and time
    // Serial.print(year, DEC);
    // Serial.print('/');
    // Serial.print(month, DEC);
    // Serial.print('/');
    // Serial.print(day, DEC);
    // Serial.print(" (");
    // Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    // Serial.print(") ");
    // Serial.print(hour, DEC);
    // Serial.print(':');
    // Serial.print(minutes, DEC);
    // Serial.print(':');
    // Serial.print(seconds, DEC);
    // Serial.println();
    // delay(1000);
}