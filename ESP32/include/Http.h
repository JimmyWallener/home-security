#ifndef HTTP_H
#define HTTP_H

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "WifiManager.h"
#include <Arduino.h>


class Http {
    public:
        Http(WifiManager*);
        bool isPinCodeValid(String);
        void syncTime();

    private:
        WifiManager* wifiManager;
        
        String generateAuthToken(const String&, const String&, const String&, const String&);
};


#endif //HTTP_H