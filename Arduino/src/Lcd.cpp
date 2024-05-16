#include "LCD.h"
#include <Arduino.h>

LCD::LCD(uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
    : lcd(rs, enable, d4, d5, d6, d7) {}

void LCD::begin() {
    lcd.begin(16, 2);
}
void LCD::print(const String &message) {
    lcd.print(message);
}

void LCD::setCursor(uint8_t col, uint8_t row) {
    lcd.setCursor(col, row);
}

