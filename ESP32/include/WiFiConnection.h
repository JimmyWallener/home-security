#include <WiFi.h>
#include "secrets.h"

class WiFiConnection {
  public:
      WiFiConnection() {};
      void initWiFi();
      void printWiFiStatus();
      void reconnectWiFi();

  private:
        struct {
            const char* ssid = WIFI_SSID;
            const char* password = WIFI_PASSWORD;
        } wifiConfig;
};