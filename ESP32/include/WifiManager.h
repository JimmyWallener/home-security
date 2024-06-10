#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <WiFiClientSecure.h>

class WifiManager {
public:
    WifiManager(const char* ssid, const char* password);
    void connect();
    bool isConnected();
    WiFiClientSecure& getWifiClient(); 

private:
    const char* _ssid;
    const char* _password;
    WiFiClientSecure _wifiClient; 
};

#endif // WIFIMANAGER_H
