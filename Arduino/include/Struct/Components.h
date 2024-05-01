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
    components->rtc.RTCInitialize();
    components->lcdDisplay.LCDInitialize();
    components->buzzer.initialize();
}


#endif // COMPONENTS_H


// Skriven av: Jimmy Wallener