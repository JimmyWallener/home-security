#include "LCD.h"
#include <Arduino.h>

/**
 * @brief Constructor for the LCD class.
 * 
 * Initializes the LCD object with the specified pin connections.
 * 
 * @param rs Register select pin.
 * @param enable Enable pin.
 * @param d4 Data pin 4.
 * @param d5 Data pin 5.
 * @param d6 Data pin 6.
 * @param d7 Data pin 7.
 */
LCD::LCD(uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
    : lcd(rs, enable, d4, d5, d6, d7) {}

/**
 * @brief Destructor for the LCD class.
 * 
 * Cleans up any resources used by the LCD object.
 */
LCD::~LCD() {}

/**
 * @brief Initializes the LCD display.
 * 
 * Sets up the LCD display with a 16x2 character layout.
 */
void LCD::initialize() {
    lcd.begin(16, 2);
}

/**
 * @brief Prints a message to the LCD display.
 * 
 * Displays the specified message on the LCD.
 * 
 * @param message The message to display on the LCD.
 */
void LCD::print(const String &message) {
    lcd.print(message);
}

/**
 * @brief Sets the cursor position on the LCD display.
 * 
 * Moves the cursor to the specified column and row.
 * 
 * @param col The column position (0-based).
 * @param row The row position (0-based).
 */
void LCD::setCursor(uint8_t col, uint8_t row) {
    lcd.setCursor(col, row);
}
