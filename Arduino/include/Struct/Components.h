#ifndef COMPONENTS_H
#define COMPONENTS_H
#include "RealTimeClock.h"
#include "LCD.h"
#include "Buzzer.h"


struct Components {
    RTC rtc;
    LCD lcdDisplay;
    Buzzer buzzer;
};

void initializeComponents(Components* components) {
    components->lcdDisplay = LCD(12, 11, 5, 4, 3, 2);
    components->rtc.RTCInitialize();
    components->lcdDisplay.LCDInitialize();
    components->buzzer.initialize();
}


#endif // COMPONENTS_H


// Skriven av: Jimmy Wallener