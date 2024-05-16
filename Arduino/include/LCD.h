#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal.h>

class LCD {
private:
    LiquidCrystal lcd;
public:
    LCD(uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
    void begin();
    void print(const String &message);
    void setCursor(uint8_t col, uint8_t row);
};

#endif // LCD_H
