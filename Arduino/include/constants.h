#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
    constexpr uint8_t SOUND_SENSOR_PIN{7};
    constexpr uint8_t PASSIVE_IR_SENSOR_PIN{6};
    constexpr uint8_t LCD_RS_PIN{12};
    constexpr uint8_t LCD_ENABLE_PIN{11};
    constexpr uint8_t LCD_D4_PIN{2};
    constexpr uint8_t LCD_D5_PIN{3};
    constexpr uint8_t LCD_D6_PIN{4};
    constexpr uint8_t LCD_D7_PIN{5};
    constexpr uint8_t BUZZER_PIN{8};
    constexpr uint8_t BUZZER_DELAY_TIME{205};
    constexpr uint8_t ESP32_I2C_ADDRESS{0x07};
    constexpr uint8_t ARDUINO_I2C_ADDRESS{0x08};
    constexpr uint8_t SENSOR_OCCURENCES{3};
    constexpr uint16_t COUNTER_DURATION{500};
    constexpr uint16_t SENSOR_THRESHOLD{5};
    constexpr uint16_t KEYPAD_TIMEOUT{2000};
    constexpr unsigned long RESET_DELAY{6000};
}

#endif // CONSTANTS_H