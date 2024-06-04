#include "MQTT.h"

MQTT::MQTT(WifiManager* wifiManager) {
    this->wifiManager = wifiManager;
}



void MQTT::connect() {
    if (this->wifiManager->isConnected()) {
        Serial.println("WiFi is connected, attempting to connect to IoT Hub...");

        if (Esp32MQTTClient_Init((const uint8_t*)AZURE_IOT_HUB_PRIMARY_CONNECTION, strlen(AZURE_IOT_HUB_PRIMARY_CONNECTION)) == 0) {
            hasIoTHub = true;
            Serial.println("IoT Hub connection successful");
        } else {
            hasIoTHub = false;
            Serial.println("IoT Hub connection failed");
        }
    } else {
        Serial.println("WiFi is not connected, cannot connect to IoT Hub");
    }
   
}

void MQTT::sendTelemetry(const String data) {
   
    if (hasIoTHub) {
        Serial.println("Sending telemetry data: " + data);
        Esp32MQTTClient_SendEvent(data.c_str());
    } else {
        Serial.println("IoT Hub connection is not established, cannot send telemetry data");
    }

    
}