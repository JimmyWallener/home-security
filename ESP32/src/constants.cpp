#include <Arduino.h>
#include "constants.h"

namespace constants {
    char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    byte KEYPAD_ROW_PINS[KEYPAD_ROWS] = {9, 8, 7, 6};
    byte KEYPAD_COLS_PINS[KEYPAD_COLS] = {5, 4, 3, 2};
}
