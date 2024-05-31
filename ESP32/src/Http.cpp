#include "Http.h"
#include "secrets.h"
#include <mbedtls/base64.h>
#include <string.h>
#include <time.h>

Http::Http(WifiManager* wifiManager) : wifiManager(wifiManager) {
    this->wifiManager = wifiManager;
    
}


void Http::syncTime() {
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    Serial.println("Waiting for NTP time sync: ");
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println("");
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.println("Current time: ");
    Serial.println(asctime(&timeinfo));
}

String Http::generateAuthToken(const String& verb, const String& resourceType, const String& resourceLink, const String& date) {

    String tempVerb = verb;
    String tempResourceType = resourceType;
    String tempResourceLink = resourceLink;

    tempVerb.toLowerCase();
    tempResourceType.toLowerCase();

    const String key = AZURE_COSMO_PRIMARY_KEY;
    
    const String stringToSign = tempVerb + "\n" +
                                tempResourceType + "\n" +
                                resourceLink + "\n" +
                                date + "\n" +
                                "\n";

    unsigned char* decodedKey;
    size_t decodedKeyLen;
    mbedtls_base64_decode(NULL, 0, &decodedKeyLen, (const unsigned char*)key.c_str(), key.length());
    decodedKey = (unsigned char*)malloc(decodedKeyLen);
    mbedtls_base64_decode(decodedKey, decodedKeyLen, &decodedKeyLen, (const unsigned char*)key.c_str(), key.length());

    unsigned char hmacResult[32];
    mbedtls_md_context_t ctx;
    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1);
    mbedtls_md_hmac_starts(&ctx, decodedKey, decodedKeyLen);
    mbedtls_md_hmac_update(&ctx, (const unsigned char*)stringToSign.c_str(), stringToSign.length());
    mbedtls_md_hmac_finish(&ctx, hmacResult);
    mbedtls_md_free(&ctx);

    free(decodedKey);

    size_t base64HmacLen;
    unsigned char base64Hmac[64];
    mbedtls_base64_encode(base64Hmac, sizeof(base64Hmac), &base64HmacLen, hmacResult, sizeof(hmacResult));
    String signature = String((char*)base64Hmac).substring(0, base64HmacLen);

   // Encode the signature for URL usage
    String authToken = "type=master&ver=1.0&sig=" + signature;
    authToken.replace("+", "%2B"); // URL encode '+' character
    authToken.replace("/", "%2F"); // URL encode '/' character
    authToken.replace("=", "%3D"); // URL encode '=' character

    return authToken;
}


bool Http::isPinCodeValid(String pinCode) {

    const char *rootCA = R"(-----BEGIN CERTIFICATE-----
MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH
MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG
9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI
2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx
1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ
q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz
tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ
vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP
BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV
5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY
1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4
NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG
Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91
8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe
pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl
MrY=
-----END CERTIFICATE-----)";

    if (!wifiManager->isConnected()) {
        Serial.println("WiFi is not connected");
        return false;
    }

    // Get current date in required format
    time_t now;
    time(&now);
    struct tm* timeinfo = gmtime(&now);
    char buf[50];
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    String date = String(buf);
    date.toLowerCase();

    WiFiClientSecure client;
    client.setCACert(rootCA);
    HTTPClient https;
   
    String url = AZURE_COSMO_DB_URI + "dbs/" + AZURE_COSMO_DB_NAME + "/colls/" + AZURE_COSMO_DB_USER_CONTAINER + "/docs";
    String resourceLink = "dbs/" + AZURE_COSMO_DB_NAME + "/colls/" + AZURE_COSMO_DB_USER_CONTAINER;
    String authorizationToken = generateAuthToken("POST", "docs", resourceLink, date);

    https.begin(client, url);

    https.addHeader("Content-Type", "application/query+json");
    https.addHeader("x-ms-documentdb-isquery", "true");
    https.addHeader("x-ms-documentdb-query-enablecrosspartition", "true");
    https.addHeader("x-ms-documentdb-query-rawresponse", "true");
    https.addHeader("Authorization", authorizationToken);
    https.addHeader("x-ms-date", date);
    https.addHeader("x-ms-version", "2018-12-31");
    https.addHeader("x-ms-documentdb-partitionkey", "");

   // String query = "{\"query\": \"SELECT * FROM " + AZURE_COSMO_DB_USER_CONTAINER + " c WHERE c.pinCode = @pinCode\", \"parameters\": [{\"name\": \"@pinCode\", \"value\": \"" + pinCode + "\"}]}";

    JsonDocument doc;
    doc["query"] = "SELECT * FROM " + AZURE_COSMO_DB_USER_CONTAINER + " c WHERE c.pinCode = @pinCode"; // Simplified query
    JsonArray params = doc["parameters"].to<JsonArray>();
    JsonObject param1 = params.add<JsonObject>();
    param1["name"] = "@pinCode";
    param1["value"] = pinCode;

    String requestBody;
    serializeJson(doc, requestBody);

    int httpResponseCode = https.POST(requestBody);

    if (httpResponseCode > 0) {
        String response = https.getString();
        Serial.println("Response: " + response);

        JsonDocument responseDoc;
        deserializeJson(responseDoc, response);
        JsonArray documents = responseDoc["Documents"].as<JsonArray>();

        for (JsonObject doc : documents) {

            String pinCode = doc["pinCode"].as<String>();
            if (pinCode == pinCode) {
                Serial.println("Pin code is valid");
                https.end();
                return true;
            }
        }
      
        https.end();

        return false;
    }
    else
    {
        Serial.println("Error on HTTPS request");
        Serial.println(httpResponseCode);
    }
    Serial.println("Request failed");
    https.end();
    return false; 
}