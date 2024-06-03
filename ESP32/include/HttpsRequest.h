#ifndef HTTPSREQUEST_H
#define HTTPSREQUEST_H

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "WifiManager.h"
#include <Arduino.h>
#include "MQTT.h"


class HttpsRequest {
    public:
        HttpsRequest(WifiManager*);
        bool isPinCodeValid(String);
        void syncTime();

    private:
        WifiManager* _wifiManager{nullptr};
        MQTT* _mqtt{nullptr};
        
        String generateAuthToken(const String&, const String&, const String&, const String&);
        void sendAccessLog(const String, const String, const String, const bool);
};


#endif //HTTPSREQUEST_H

