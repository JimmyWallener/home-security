Project Name: 

Home Security System with Arduino

Introduction

This project implements a home security system utilizing Arduino Uno and ESP32, leveraging Azure IoT Hub for cloud connectivity and data processing. Sensor data is collected and analyzed to trigger alarms when necessary, ensuring the safety and security of your premises.

Hardware Components

    - Arduino Uno Rev3
    - ESP32 with Wi-Fi
    - Passive Infrared Sensor (PIR)
    - Sound Sensor
    - Keypad for Arming/Disarming
    - LED Indicator for Alarm Status
    - Buzzer for Audible Alarm
    Additional hardware components can be integrated as needed

Software Environment

    - Visual Studio Code (VS Code)
    - PlatformIO extension (for Arduino development within VS Code)

Project Structure

    - arduino/: Contains Arduino Uno code for sensor readings, real-time clock (RTC) management, buzzer control, and LCD display functionality.
    - esp32/: Houses ESP32 code for Wi-Fi communication, MQTT messaging, keypad interaction, and Azure IoT Hub connection.

Key Features

    - Sensor integration: PIR and sound sensors detect potential intrusions.
    - Real-time clock (RTC) management: Maintains accurate timekeeping for reliable alarm triggering.
    - Alarm functionality: Alerts you of security breaches through audible alarms and visual indicators.
    - Azure IoT Hub connectivity: Enables cloud-based data storage, analysis, and potential remote control capabilities.

Getting Started

    - Clone the Repository: Use git clone https://github.com/JimmyWallener/home-security.git to clone this repository locally.
    - Install PlatformIO: Refer to the official documentation (https://docs.platformio.org/en/latest/installation.html) for installation instructions.
    - Set Up Hardware: Connect the Arduino Uno, ESP32, and other components as per your chosen hardware configuration.
    - Install Dependencies (Optional): If the project has specific library dependencies, run platformio lib install <library_name> within your project directory to install them.
    - Configure Azure IoT Hub (Optional): If you intend to use Azure IoT Hub, create an Azure account, set up an IoT Hub instance, and configure the connection details within the code.
    - Compile and Upload: Use the PlatformIO IDE (integrated into VS Code) to compile and upload the code to your Arduino and ESP32 boards.
    
Additional Notes

    - This is a basic framework, and you may need to customize the code, libraries, and hardware connections based on your specific requirements and sensor choices.
    - Consult the documentation for your chosen sensors, libraries, and Azure IoT Hub for detailed instructions and configuration steps.
    - Consider implementing security measures, such as authentication and encryption, if your system transmits sensitive data to Azure IoT Hub.

Disclaimer

    - This project is for educational purposes only. The developers is not responsible for any misuse or malfunction of the system.


Contributors:

    - Leo Katakalidis
    - Joakim Sjöström
    - Jimmy Wallener


