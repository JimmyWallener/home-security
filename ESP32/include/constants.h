#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
    constexpr uint8_t ESP32_I2C_ADDRESS{0x07};
    constexpr uint8_t ARDUINO_I2C_ADDRESS{0x08};
    constexpr byte KEYPAD_ROWS{4};
    constexpr byte KEYPAD_COLS{4};
    
    // Deklarera arrayer som externa utan const
    extern char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS];
    extern byte KEYPAD_ROW_PINS[KEYPAD_ROWS];
    extern byte KEYPAD_COLS_PINS[KEYPAD_COLS];
}

#endif // CONSTANTS_H