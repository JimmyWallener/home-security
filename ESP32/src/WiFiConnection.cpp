/**
 * @file WiFiConnection.cpp
 * @author Jimmy Wallener
 * @brief class for handling WiFi connection
 * @details This class handles the WiFi connection for the ESP32
 * @date 2024-05-08
 * 
 * 
 */


#include "WiFiConnection.h"
#include <Arduino.h>



/**
 * @brief ** Initialize WiFi connection
 * @details ** This function initializes the WiFi connection using the SSID and password from the secrets.h file
 * @return ** void 
 * @todo ** Add a check to see if the connection was successful, send message to lcd display on Arduino
 */
void WiFiConnection::initWiFi() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiConfig.ssid, wifiConfig.password);
  WiFi.setAutoReconnect(true);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);

    
    Serial.write("Connecting to WiFi");
  }
  
};

/**
 * @brief ** Print WiFi status
 * @details ** This function prints the WiFi status to the serial monitor
 * @return ** void 
 * @todo ** Add a check to see if the connection was successful, send message to lcd display on Arduino
 */

void WiFiConnection::printWiFiStatus() {
  if(WiFi.status() == WL_CONNECTED) {
    Serial.write("Connected to WiFi, IP address: ");
    Serial.write(WiFi.localIP());
  } else {
    Serial.write("Not connected to WiFi");
  }
};


/**
 * @brief ** Reconnect WiFi
 * @details ** This function reconnects the WiFi connection
 * @return ** void 
 * @todo ** Add a check to see if the connection was successful, send message to lcd display on Arduino
 */
void WiFiConnection::reconnectWiFi() {
  WiFi.reconnect();
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.write("Reconnecting to WiFi");
  }
};