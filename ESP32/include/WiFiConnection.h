#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include <WiFi.h>

class WiFiConnection {
private:
    const char* _ssid;
    const char* _password;

public:
    WiFiConnection(const char* ssid, const char* password) : _ssid(ssid), _password(password) {}
    void connect();
    bool isConnected();
};

#endif // WIFICONNECTION_H
