#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>

class WifiManager {
private:
    const char* _ssid;
    const char* _password;

public:
    WifiManager(const char* ssid, const char* password) : _ssid(ssid), _password(password) {}
    void connect();
    bool isConnected();
};

#endif // WIFIMANAGER_H
