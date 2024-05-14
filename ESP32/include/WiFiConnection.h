#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include <WiFi.h>

class WiFiConnection {
public:
    void connect(const char* ssid, const char* password);
};

#endif // WIFICONNECTION_H
