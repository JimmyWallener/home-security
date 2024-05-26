#include "WiFiConnection.h"




// Takes in the SSID and password of the WiFi network to connect to
void WiFiConnection::connect() {
    WiFi.begin(_ssid, _password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
}

bool WiFiConnection::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}
