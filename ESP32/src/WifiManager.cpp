#include "WifiManager.h"

WifiManager::WifiManager(const char* ssid, const char* password) 
    : _ssid(ssid), _password(password) {}

void WifiManager::connect() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
}

bool WifiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

WiFiClientSecure& WifiManager::getWifiClient() {
    return _wifiClient;
}
