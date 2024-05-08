/**
 * @file RealTimeClock.cpp
 * @author Joakim Sjöström
 * @brief RealTimeClock class implementation
 * @date 2024-05-06
 * 
 * 
 */


#include "RealTimeClock.h"

RTC::RTC() {}


/**
 * @brief Initialize the RTC hardware
 * @details This method initializes the RTC hardware and syncs the time to the RTC
 * @return void
 */

void RTC::RTCInitialize()
{
    
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1);
    }

    // Initial sync from RTC
    SyncTimeToRTC();
}

/**
 * @brief update the RTC time
 * @details This method updates the RTC time
 * @return void
 */

void RTC::RTCUpdate()
{
    // Sync time from RTC to our currentTime structure
    SyncTimeFromRTC();
}

/**
 * @brief get the current time from the RTC
 * @details This method gets the current time from the RTC
 * 
 * @return ** DateTimeInfo 
 */

DateTimeInfo RTC::GetTime()
{
    SyncTimeFromRTC();
    return currentTime;
}


/**
 * @brief Initialize the RTC hardware and sync the time to the RTC
 * @details This method initializes the RTC hardware and syncs the time to the RTC
 * @return void
 */

void RTC::SyncTimeToRTC()
{
    // Convert the DateTimeInfo to DateTime format and set it to RTC
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

/**
 * @brief Sync the time from RTC hardware to the structured time
 * @details This method syncs the time from RTC hardware to the structured time
 * @return void
 */

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
