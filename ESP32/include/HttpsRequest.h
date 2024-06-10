#ifndef HTTPSREQUEST_H
#define HTTPSREQUEST_H

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "WifiManager.h"
#include <Arduino.h>
#include "HttpsRequest.h"
#include "generateUUID.h"


class HttpsRequest {
    public:
        HttpsRequest(WifiManager*);
        ~HttpsRequest();
        bool isPinCodeValid(String);
        void sendSensorLogToCosmo(const String&);
        void syncTime();

    private:
        WifiManager* _wifiManager{nullptr};
        String generateAuthToken(const String &, const String &, const String &, const String &);
        void sendAccessLog(const String, const String, const String, const bool);
};


#endif //HTTPSREQUEST_H

