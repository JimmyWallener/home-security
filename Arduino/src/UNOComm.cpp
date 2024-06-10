#include "UNOComm.h"
#include "constants.h"
#include <ArduinoJson.h>

using namespace constants;

// Initiate static instance pointer
UNOComm *UNOComm::instance = nullptr;

// Set pointer to current instance

/**
 * @brief Constructor for the UNOComm class.
 * 
 * Initializes the UNOComm object with default values for LCD, buzzer, and alarm activation status.
 * Also sets the static instance pointer to the current object.
 */
UNOComm::UNOComm() : _lcd(nullptr), _buzzer(nullptr), _alarmActivated(false) {
    instance = this;
}

/**
 * @brief Destructor for the UNOComm class.
 * 
 * Cleans up dynamically allocated resources for the LCD and buzzer.
 */
UNOComm::~UNOComm() {
    if (_lcd) {
        delete _lcd;
    }
    if (_buzzer) {
        delete _buzzer;
    }
}

/**
 * @brief Initializes the UNOComm object.
 * 
 * This method initializes the I2C communication on the Arduino with a specified address and sets up 
 * the callbacks for receiving and requesting data over I2C.
 */
void UNOComm::initialize() {
    Wire.begin(ARDUINO_I2C_ADDRESS); // Join I2C bus with address #8
    Wire.onReceive(onReceive);
    Wire.onRequest(onRequest);
}

/**
 * @brief Sets the LCD object for the UNOComm.
 * 
 * @param lcd Pointer to an LCD object to be used for displaying messages.
 */
void UNOComm::setLCD(LCD *lcd) {
    this->_lcd = lcd;
}

/**
 * @brief Callback function for receiving I2C data.
 * 
 * This static method is called when data is received over I2C. It delegates the processing of the 
 * I2C command to the instance of the UNOComm object.
 * 
 * @param numBytes Number of bytes received over I2C.
 */
void UNOComm::onReceive(int numBytes) {
    if (instance != nullptr)
        instance->processI2CCommand(numBytes);
}

/**
 * @brief Processes I2C commands received from the ESP32.
 * 
 * This method reads the command byte from the I2C data and calls the appropriate handler function 
 * based on the command received.
 * 
 * @param numBytes Number of bytes received over I2C.
 */
void UNOComm::processI2CCommand(int numBytes) {
    if (numBytes > 0) {
        char command = Wire.read();
        switch (command) {
        case 'R': // RTC Data
            instance->setRealTimeClock();
            break;
        case 'A': // Alarm Activation
        case 'D': // Alarm Deactivation
            instance->handleAlarmActivation(command);
            break;
        case 'P': // Pin Code Feedback
            instance->handlePinCodeFeedback();
            break;
        case 'K': // Keypad Data
            instance->handleKeypadData();
            break;
        default:
            break;
        }
    }
}

/**
 * @brief Gets the current real-time clock value as a string.
 * 
 * @return A string representing the current date and time.
 */
String UNOComm::getRealTimeClock() {
    return _dateTime;
}

/**
 * @brief Sets the real-time clock value from data received over I2C.
 * 
 * This method reads the date and time data from the I2C buffer and updates the internal date-time 
 * string.
 */
void UNOComm::setRealTimeClock() {
    _dateTime = "";
    while (Wire.available()) {
        char c = Wire.read();
        _dateTime += c;
    }
    updateLCD();
}

/**
 * @brief Handles keypad data received from the I2C buffer.
 * 
 * This method processes the keypad data, including handling timeout for pin code entry, 
 * and updating the pin code and LCD display accordingly.
 */
void UNOComm::handleKeypadData() {
    char key = Wire.read();
    unsigned long currentMillis = millis();  // Current time
    _buzzer->keyPressSound();
    // If the timeout has occurred, clear the pin code
    if (currentMillis - _lastKeypadInputTime > KEYPAD_TIMEOUT && strlen(_userInputtedPassword) > 0) {
        memset(_pinCode, 0, sizeof(_pinCode));
        memset(_userInputtedPassword, 0, sizeof(_userInputtedPassword));
        if (_lcd) {
            _lcd->setCursor(0, 1);
            _lcd->print("                ");
        }
    }
    // Update the pin code if a key is pressed
    if (isdigit(key) || key == 'D' || key == 'C') {
        _lastKeypadInputTime = currentMillis;

        // Handled the delete key
        if (key == 'D' && strlen(_userInputtedPassword) > 0) {
            _userInputtedPassword[strlen(_userInputtedPassword) - 1] = '\0';
            _pinCode[strlen(_pinCode) - 1] = '\0';
        } 
        // Handled the clear key
        else if (key == 'C') {
            memset(_userInputtedPassword, 0, sizeof(_userInputtedPassword));
            memset(_pinCode, 0, sizeof(_pinCode));
        } 
        // Handled number keys
        else if (strlen(_userInputtedPassword) < 4 && isdigit(key)) {
            auto len = strlen(_userInputtedPassword);
            _userInputtedPassword[len] = key;
            _userInputtedPassword[len + 1] = '\0';
            _pinCode[len] = '*';
            _pinCode[len + 1] = '\0';
        }

        // Update the LCD
        if (_lcd) {
            _lcd->setCursor(0, 1);
            _lcd->print("                ");
            _lcd->setCursor(0, 1);
            _lcd->print(_pinCode);
        }
    }
}

/**
 * @brief Handles alarm activation commands received over I2C.
 * 
 * This method sets the alarm activation status and updates the LCD display and buzzer based on 
 * the received command ('A' for activation, 'D' for deactivation).
 * 
 * @param command The command received over I2C ('A' for activation, 'D' for deactivation).
 */
void UNOComm::handleAlarmActivation(char command) {
    if (command == 'A') {
        _alarmActivated = true;
        _buzzer->alarmActiveSound();
        displayTemporaryMessage("Alarm is active", 5000);
        
    } else if (command == 'D') {
        _alarmActivated = false;
        _buzzer->alarmInactiveSound();
        displayTemporaryMessage("Alarm is offline", 5000);
        
    }
}

/**
 * @brief Handles pin code feedback received over I2C.
 * 
 * This method processes the pin code feedback, updates the LCD display with the number of attempts 
 * left, and triggers appropriate buzzer sounds for success or failure.
 */
void UNOComm::handlePinCodeFeedback() {
    bool success = Wire.read();
    int attemptsLeft = Wire.read();
    _buzzer->keyPressSound();
    if (success)
    {
        displayTemporaryMessage("Correct pin", 3000);
    }
    else
    {
        _buzzer->wrongPassword();
        displayTemporaryMessage("Wrong pin ", 1000);
        displayTemporaryMessage("Attempts left: " + String(attemptsLeft), 3000);
        
    }
}

/**
 * @brief Displays a temporary message on the LCD.
 * 
 * This method sets a message on the LCD for a specified duration.
 * 
 * @param message The message to be displayed on the LCD.
 * @param duration The duration (in milliseconds) for which the message will be displayed.
 */
void UNOComm::displayTemporaryMessage(const String &message, unsigned long duration) {
    if (_lcd) {
        _lcd->setCursor(0, 1);
        _lcd->print(message);
        _messageClearTime = millis() + duration;
        updateLCD();
    }
}

/**
 * @brief Updates the LCD display with the current date and time.
 * 
 * This method updates the LCD with the current date and time, and clears the message if the 
 * specified message display duration has expired.
 */
void UNOComm::updateLCD() {
    if (_lcd) {
        _lcd->setCursor(0, 0);
        _lcd->print(getRealTimeClock());
        if (millis() >= _messageClearTime && _messageClearTime != 0) {
            _lcd->setCursor(0, 1);
            _lcd->print("                ");
            _messageClearTime = 0;
        }
    }
}

/**
 * @brief Periodically updates the LCD display.
 * 
 * This method checks if the LCD update interval has elapsed and updates the LCD accordingly.
 */
void UNOComm::update() {
    if (millis() - _lastLCDUpdateTime >= _lcdUpdateInterval) {
        _lastLCDUpdateTime = millis();
        updateLCD();
    }
}

/**
 * @brief Sends log data to the ESP32 over I2C.
 * 
 * This method sends log data to the ESP32, indicating the type of sensor that triggered the event.
 */
void UNOComm::sendLogDataToESP32() {
    if (_sensorType != NO_SENSOR) {
        Wire.write('S');  
        Wire.write(static_cast<uint8_t>(_sensorType));  
        Wire.write('\0');  
        _sensorType = NO_SENSOR;
    }
}

/**
 * @brief Sets the sensor type to be logged.
 * 
 * This method sets the sensor type that will be logged and sent to the ESP32.
 * 
 * @param sensor The type of sensor to be set.
 */
void UNOComm::setSensor(SensorType sensor) {
    _sensorType = sensor;
}

/**
 * @brief Sets the buzzer object for the UNOComm.
 * 
 * @param buzzer Pointer to a Buzzer object to be used for sound alerts.
 */
void UNOComm::setBuzzer(Buzzer *buzzer) {
    this->_buzzer = buzzer;
}

/**
 * @brief Switches the alarm state between activated and deactivated.
 * 
 * This method toggles the alarm state and updates the LCD display and buzzer accordingly.
 */
void UNOComm::switchState() {
    if (_alarmActivated) {
        _alarmActivated = false;
        _lcd->print("Alarm Deactivated");
        _buzzer->alarmInactiveSound();
    } else {
        _alarmActivated = true;
        _lcd->print("Alarm Activated");
        _buzzer->alarmActiveSound();
    }
}

/**
 * @brief Gets the current alarm activation state.
 * 
 * @return True if the alarm is activated, false otherwise.
 */
bool UNOComm::getState() const {
    return _alarmActivated;
}

/**
 * @brief Callback function for handling I2C data requests.
 * 
 * This static method is called when the ESP32 requests data over I2C. It triggers the sending of 
 * log data to the ESP32.
 */
void UNOComm::onRequest() {
    if (instance != nullptr)
        instance->sendLogDataToESP32();
}
