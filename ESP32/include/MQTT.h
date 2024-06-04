#ifndef MQTT_H
#define MQTT_H

#include <WifiManager.h>
#include <AzureIotHub.h>
#include <Esp32MQTTClient.h>
#include "secrets.h"


class MQTT {
    public:
        MQTT(WifiManager*);
        void connect();
        void sendTelemetry(const String);

    private:
        WifiManager* wifiManager{nullptr};
        bool hasIoTHub;
        
};

#endif //MQTT_H