#include "HttpsRequest.h"
#include "secrets.h"
#include <mbedtls/base64.h>
#include <string.h>
#include <time.h>
#include "AccessLog.h"




HttpsRequest::HttpsRequest(WifiManager* wifiManager) : _wifiManager(wifiManager){
    this->_mqtt = new MQTT(wifiManager);
}

/**
 * @brief Synchronize the time with the NTP server. The time is used to generate the authorization token
 *  for Azure Cosmos DB. Becasue the token is valid for a short period of time, the time must be in sync.
 *  
 */

void HttpsRequest::syncTime() {
    const long gmtOffset_sec = 3600 * 2;
    const int daylightOffset_sec = 0; 

    configTime(gmtOffset_sec, daylightOffset_sec, "europe.pool.ntp.org", "se.pool.ntp.org");
    Serial.println("Waiting for NTP time sync: ");
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println("");
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    Serial.println("Current time: ");
    Serial.println(asctime(&timeinfo));
}

/**
 * @brief Generate the authorization token for Azure Cosmos DB. Depending on the verb, resource type,
 *  resource link and date, the token is generated. The token is then encoded in base64 and signed using
 * the primary key of the Azure Cosmos DB account.
 * 
 * @param verb 
 * @param resourceType 
 * @param resourceLink 
 * @param date 
 * @return ** String 
 */

String HttpsRequest::generateAuthToken(const String& verb, const String& resourceType, const String& resourceLink, const String& date) {

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


/**
 * @brief Check if the pin code is valid. The pin code is checked against the Azure Cosmos DB.
 * 
 * @param pinCode 
 * @return ** bool 
 */

bool HttpsRequest::isPinCodeValid(String pinCode) {

        

    // Root CA certificate for Azure Cosmos DB, obtained from https://www.digicert.com/kb/digicert-root-certificates.htm
    const char *rootCA = ROOT_CA;

    if (!_wifiManager->isConnected()) {
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
     // Set headers for the HTTPS request
    https.addHeader("Content-Type", "application/query+json");
    https.addHeader("x-ms-documentdb-isquery", "true");
    https.addHeader("x-ms-documentdb-query-enablecrosspartition", "true");
    https.addHeader("x-ms-documentdb-query-rawresponse", "true");
    https.addHeader("Authorization", authorizationToken);
    // Important that the date in the header matches the date used to generate the authorization token
    https.addHeader("x-ms-date", date);
    https.addHeader("x-ms-version", "2018-12-31");
    https.addHeader("x-ms-documentdb-partitionkey", "");

   // String query = "{\"query\": \"SELECT * FROM " + AZURE_COSMO_DB_USER_CONTAINER + " c WHERE c.pinCode = @pinCode\", \"parameters\": [{\"name\": \"@pinCode\", \"value\": \"" + pinCode + "\"}]}";

    JsonDocument doc;

    // Simplified query that only checks if the pin code exists in the Azure Cosmos DB
    doc["query"] = "SELECT * FROM " + AZURE_COSMO_DB_USER_CONTAINER + " c WHERE c.pinCode = @pinCode"; 
    JsonArray params = doc["parameters"].to<JsonArray>();
    JsonObject param1 = params.add<JsonObject>();
    param1["name"] = "@pinCode";
    param1["value"] = pinCode;

    String requestBody;
    serializeJson(doc, requestBody);

    int httpResponseCode = https.POST(requestBody);

    Serial.println(https.getString());

    if (httpResponseCode > 0) {
        String response = https.getString();
        Serial.println(response);

        JsonDocument responseDoc;
        deserializeJson(responseDoc, response);
        JsonArray documents = responseDoc["Documents"].as<JsonArray>();

        for (JsonObject doc : documents) {

            String pinCode = doc["pinCode"].as<String>();
            if (pinCode == pinCode) {

               sendAccessLog(doc["userId"].as<String>(), date, "loginattempt", true);

                // if the pin code is found in the Azure Cosmos DB, return true and end the HTTPS request
                https.end();
                return true;
            }
        }

        sendAccessLog("Unknown", date, "loginattempt", false);
        // if the pin code is not found in the Azure Cosmos DB, return false and end the HTTPS request
        https.end();

        return false;
    }
    else {
        // if the HTTPS request fails, print the error code and end the HTTPS request
        Serial.println("Error on HTTPS request");
        Serial.println(httpResponseCode);
        https.end();
    }

    // if the HTTPS request fails, return false
    Serial.println("Request failed");
    https.end();
    return false; 
}


void HttpsRequest::sendAccessLog(const String userId, const String timestamp, const String action, bool success) {

    if(_mqtt == nullptr) {
        _mqtt->connect();
    }

    AccessLog log;
    log.userId = userId;
    log.timestamp = timestamp;
    log.action = action;
    log.success = success;

    _mqtt->sendTelemetry(log.toJson());
} 