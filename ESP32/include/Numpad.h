#include <Arduino.h>
#include <Keypad.h>

Keypad initializeKeypad() {

    const byte rowSize = 4;
    const byte colSize = 4;

    byte rowPins[rowSize] = {2, 3, 4, 5};
    byte colPins[rowSize] = {6, 7, 8, 9};

    char hexaKeys[rowSize][colSize] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    Keypad numpad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, rowSize, colSize);
    return numpad;
}