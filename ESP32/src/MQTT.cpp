#include "MQTT.h"

MQTT::MQTT(WifiManager* wifiManager) {
    this->wifiManager = wifiManager;
}

void MQTT::connect() {

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected");
        if (!Esp32MQTTClient_Init((const u_int8_t*)AZURE_IOT_HUB_PRIMARY_CONNECTION)) {
            Serial.println("Initializing IoT hub failed.");
            hasIoTHub = false;
            return;
        }
        hasIoTHub = true;
        Serial.println("IoT hub initialized");
    }
}

void MQTT::sendTelemetry(const String data) {
    if (hasIoTHub) {
        char buff[128];

        snprintf(buff, sizeof(buff), "{\"%s\":\"%s\"}", data);

        if (!Esp32MQTTClient_SendEvent(buff)) {
            Serial.println("Sending log failed.");
            return;
        }

        Serial.println("Log sent");
        Esp32MQTTClient_Close();
    }
}