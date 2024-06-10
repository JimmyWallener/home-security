#include "WifiManager.h"

/**
 * @brief Constructor for the WifiManager class.
 * 
 * Initializes the WifiManager with the provided SSID and password for connecting to a WiFi network.
 * 
 * @param ssid The SSID of the WiFi network to connect to.
 * @param password The password for the WiFi network.
 */
WifiManager::WifiManager(const char* ssid, const char* password) 
    : _ssid(ssid), _password(password) {}

/**
 * @brief Connects to the WiFi network using the SSID and password provided during initialization.
 * 
 * This method sets the WiFi mode to station mode, initiates the connection to the WiFi network,
 * and waits until the connection is established. It prints a dot to the serial monitor every 
 * 500 milliseconds until the connection is successful.
 */
void WifiManager::connect() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
}

/**
 * @brief Checks if the device is connected to a WiFi network.
 * 
 * This method checks the current status of the WiFi connection and returns true if the device 
 * is connected to a WiFi network.
 * 
 * @return True if the device is connected to a WiFi network, false otherwise.
 */
bool WifiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

/**
 * @brief Gets the secure WiFi client.
 * 
 * This method returns a reference to the WiFiClientSecure object, which can be used for 
 * secure communications over WiFi.
 * 
 * @return A reference to the WiFiClientSecure object.
 */
WiFiClientSecure& WifiManager::getWifiClient() {
    return _wifiClient;
}
