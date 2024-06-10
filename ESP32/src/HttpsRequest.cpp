#include "HttpsRequest.h"
#include "secrets.h"
#include <mbedtls/base64.h>
#include <string.h>
#include <time.h>
#include "AccessLog.h"

HttpsRequest::HttpsRequest(WifiManager* wifiManager) : _wifiManager(wifiManager) {
}

HttpsRequest::~HttpsRequest() {
    delete _wifiManager;
}

/**
 * @brief Synchronize the time with the NTP server.
 * 
 * This method synchronizes the time with the NTP server to ensure the system time is accurate. This is important for generating 
 * valid authorization tokens for Azure Cosmos DB, as the token is time-sensitive.
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
 * @brief Generate the authorization token for Azure Cosmos DB.
 * 
 * Generates an authorization token required to authenticate requests to Azure Cosmos DB. The token is generated based on the 
 * provided HTTP verb, resource type, resource link, and current date. The token is then encoded in base64.
 * 
 * @param verb HTTP verb (e.g., "GET", "POST").
 * @param resourceType The type of resource being accessed (e.g., "dbs", "colls").
 * @param resourceLink The link to the resource.
 * @param date The current date in GMT format.
 * @return The generated authorization token.
 */
String HttpsRequest::generateAuthToken(const String& verb, const String& resourceType, const String& resourceLink, const String& date) {
    String tempVerb = verb;
    String tempResourceType = resourceType;
    String tempResourceLink = resourceLink;

    tempVerb.toLowerCase();
    tempResourceType.toLowerCase();

    const String key = AZURE_COSMO_SECONDARY_KEY;
    
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

    String authToken = "type=master&ver=1.0&sig=" + signature;
    authToken.replace("+", "%2B");
    authToken.replace("/", "%2F");
    authToken.replace("=", "%3D");

    return authToken;
}

/**
 * @brief Check if the provided pin code is valid by querying Azure Cosmos DB.
 * 
 * This method sends a query to Azure Cosmos DB to verify if the provided pin code exists. It checks against the specified 
 * user container and uses the authorization token for authentication.
 * 
 * @param pinCode The pin code to validate.
 * @return True if the pin code is valid, false otherwise.
 */
bool HttpsRequest::isPinCodeValid(String pinCode) {
    const char *rootCA = ROOT_CA;

    if (!_wifiManager->isConnected()) {
        Serial.println("WiFi is not connected");
        return false;
    }

    time_t now;
    time(&now);
    struct tm* timeinfo = gmtime(&now);
    char buf[50];
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    String date = String(buf);
    date.toLowerCase();

    WiFiClientSecure* client = new WiFiClientSecure;
    if(client) {
        client->setCACert(rootCA);
    }
    client->setInsecure();

    HTTPClient https;
    https.setReuse(false);

    String url = AZURE_COSMO_DB_URI + "dbs/" + AZURE_COSMO_DB_NAME + "/colls/" + AZURE_COSMO_DB_USER_CONTAINER + "/docs";
    String resourceLink = "dbs/" + AZURE_COSMO_DB_NAME + "/colls/" + AZURE_COSMO_DB_USER_CONTAINER;
    String authorizationToken = generateAuthToken("POST", "docs", resourceLink, date);

    https.begin(*client, url.c_str());
    https.addHeader("Content-Type", "application/query+json");
    https.addHeader("x-ms-documentdb-isquery", "true");
    https.addHeader("x-ms-documentdb-query-enablecrosspartition", "true");
    https.addHeader("x-ms-documentdb-query-rawresponse", "true");
    https.addHeader("Authorization", authorizationToken);
    https.addHeader("x-ms-date", date);
    https.addHeader("x-ms-version", "2018-12-31");
    https.addHeader("x-ms-documentdb-partitionkey", "");

    JsonDocument doc;
    doc["query"] = "SELECT * FROM " + AZURE_COSMO_DB_USER_CONTAINER + " c WHERE c.pinCode = @pinCode"; 
    JsonArray params = doc["parameters"].to<JsonArray>();
    JsonObject param1 = params.add<JsonObject>();
    param1["name"] = "@pinCode";
    param1["value"] = pinCode;

    String requestBody;
    serializeJson(doc, requestBody);

    int httpResponseCode = https.POST(requestBody);

    if (httpResponseCode > 0) {
        String response = https.getString();

        JsonDocument responseDoc;
        deserializeJson(responseDoc, response);
        JsonObject firstDoc = responseDoc["Documents"][0];

        String pin = firstDoc["pinCode"].as<String>();

        if (pin == pinCode) {
            sendAccessLog(firstDoc["userId"].as<String>(), date, "loginattempt", true);
            https.end();
            delete client;
            return true;
        }
        else {
            sendAccessLog("Unknown", date, "loginattempt", false);
            https.end();
            delete client;
            return false;
        }

        https.end();
        return false;
    }
    else {
        Serial.println("Error on HTTPS request");
        Serial.println(httpResponseCode);
        https.end();
        delete client;
        return false;
    }

    Serial.println("Request failed");
    delete client;
    https.end(); 
    return false; 
}

/**
 * @brief Send an access log to Azure Cosmos DB.
 * 
 * This method logs access attempts (e.g., login attempts) by sending the log data to Azure Cosmos DB.
 * The log includes information such as user ID, timestamp, action, and success status.
 * 
 * @param userId The ID of the user attempting access.
 * @param timestamp The timestamp of the access attempt.
 * @param action The action being logged (e.g., "loginattempt").
 * @param success True if the access attempt was successful, false otherwise.
 */
void HttpsRequest::sendAccessLog(const String userId, const String timestamp, const String action, bool success) {
    AccessLog log;
    log.id = generateUUID();
    log.userId = userId;
    log.type = "login_log";
    log.timestamp = timestamp;
    log.action = action;
    log.success = success;
    Serial.println(log.toJson());

    const char *rootCA = ROOT_CA;
    if (!_wifiManager->isConnected()) {
        Serial.println("WiFi is not connected");
        return;
    }

    time_t now;
    time(&now);
    struct tm* timeinfo = gmtime(&now);
    char buf[50];
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    String date = String(buf);
    date.toLowerCase();

    WiFiClientSecure* client = new WiFiClientSecure;
    if(client) {
        client->setCACert(rootCA);
    }
    client->setInsecure();
    HTTPClient https;
    https.setReuse(false);

    String url = AZURE_COSMO_DB_URI + "dbs/" + AZURE_COSMO_DB_NAME + "/colls/" + AZURE_COSMO_DB_ALARM_CONTAINER + "/docs";
    String resourceLink = "dbs/" + AZURE_COSMO_DB_NAME + "/colls/" + AZURE_COSMO_DB_ALARM_CONTAINER;
    String authorizationToken = generateAuthToken("POST", "docs", resourceLink, date);

    https.begin(*client, url.c_str());
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Authorization", authorizationToken);
    https.addHeader("x-ms-date", date);
    https.addHeader("x-ms-version", "2018-12-31");
    https.addHeader("x-ms-documentdb-partitionkey", "[\"login_log\"]");

    String logData = log.toJson();
    int httpResponseCode = https.POST(logData);

    if (httpResponseCode > 0) {
        String response = https.getString();
        Serial.println("Response from Cosmos DB: " + response);
    } else {
        Serial.println("Error sending log to Cosmos DB");
        Serial.println(httpResponseCode);
    }

    https.end();
    delete client;
}

/**
 * @brief Send a sensor log to Azure Cosmos DB.
 * 
 * This method sends a sensor log to Azure Cosmos DB. The log includes sensor data such as timestamp,
 * sensor type, sensor ID, and sensor value.
 * 
 * @param doc The JSON string representing the sensor log data.
 */
void HttpsRequest::sendSensorLogToCosmo(const String &doc) {
    const char *rootCA = ROOT_CA;
    if (!_wifiManager->isConnected()) {
        Serial.println("WiFi is not connected");
        return;
    }

    time_t now;
    time(&now);
    struct tm* timeinfo = gmtime(&now);
    char buf[50];
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    String date = String(buf);
    date.toLowerCase();

    WiFiClientSecure* client = new WiFiClientSecure;
    if(client) {
        client->setCACert(rootCA);
    }

    client->setInsecure();
    HTTPClient https;
    https.setReuse(false);

    String url = AZURE_COSMO_DB_URI + "dbs/" + AZURE_COSMO_DB_NAME + "/colls/"+ AZURE_COSMO_DB_SENSOR_CONTAINER + "/docs";
    String resourceLink = "dbs/" + AZURE_COSMO_DB_NAME + "/colls/" + AZURE_COSMO_DB_SENSOR_CONTAINER;
    String authorizationToken = generateAuthToken("POST", "docs", resourceLink, date);

    https.begin(*client, url.c_str());
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Authorization", authorizationToken);
    https.addHeader("x-ms-date", date);
    https.addHeader("x-ms-version", "2018-12-31");
    https.addHeader("x-ms-documentdb-partitionkey", "[\"sensor_log\"]");

    int httpResponseCode = https.POST(doc);

    if (httpResponseCode > 0) {
        String response = https.getString();
        Serial.println("Response from Cosmos DB: " + response);
    } else {
        Serial.println("Error sending log to Cosmos DB");
        Serial.println(httpResponseCode);
    }

    https.end();
    delete client;
}
