#include "WiFiConnection.h"


// Takes in the SSID and password of the WiFi network to connect to
void WiFiConnection::connect(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
}
