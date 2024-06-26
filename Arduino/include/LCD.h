#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal.h>
#include "Components.h"

class LCD : public Component {
private:
    LiquidCrystal lcd;
public:
    LCD(uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
    ~LCD() override;
    void initialize() override;
    void print(const String&);
    void setCursor(uint8_t, uint8_t);
};

#endif // LCD_H
