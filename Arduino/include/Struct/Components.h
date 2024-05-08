/**
 * @file Components.h
 * @author Jimmy Wallener
 * @brief Struct for the components
 * @details This file contains the struct for the components used in the system
 * @date 2024-05-06
 * 
 * 
 */


#ifndef COMPONENTS_H
#define COMPONENTS_H
#include "RealTimeClock.h"
#include "LCD.h"
#include "Buzzer.h"

const int rs = 12, en = 11, d4 = 2, d5 = 3, d6 = 4, d7 = 5;

struct Components {
    RTC rtc;
    LCD lcdDisplay;
    Buzzer buzzer;
};

/**
 * @brief Initialize the components
 * @details This method initializes the components
 *  
 * 
 * @param components
 * @return void
 *  
 */

void initializeComponents(Components* components) {
    components->lcdDisplay = LCD(rs, en, d4, d5, d6, d7);
    components->rtc.RTCInitialize();
    components->lcdDisplay.LCDInitialize();
    components->buzzer.initialize();
}


#endif // COMPONENTS_H


// Skriven av: Jimmy Wallener