#include "Authentication.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <secrets.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Constructor definition
Authentication::Authentication()
{
    // Initialize variables
    memset(jwtToken, 0, sizeof(jwtToken)); // Clear jwtToken buffer
}

String Authentication::readJWTTokenFromFile()
{
    String jwtToken = "";

    if (LittleFS.begin())
    {
        if (LittleFS.exists(jwtFilePath))
        {
            File file = LittleFS.open(jwtFilePath, "r");
            if (file)
            {
                jwtToken = file.readString();
                file.close();
            }
            else
            {
                Serial.println("no file?");
            }
        }
        else
        {
            Serial.println("file not found on LittleFS");
        }
        LittleFS.end();
    }
    else
    {
        Serial.println("LittlFS Failed to begin");
    }

    return jwtToken;
}

void Authentication::saveJWTTokenToFile(const char *token)
{
    if (LittleFS.begin())
    {
        File file = LittleFS.open(jwtFilePath, "w");
        if (file)
        {
            file.print(token);
            file.close();
            Serial.println("JWT token saved to file.");
        }
        else
        {
            Serial.println("couldn't create file?");
        }
        LittleFS.end();
    }
    else{
        Serial.println("Couldn't open Littlefs to write jwt");
    }
}

bool Authentication::authenticate()
{
    Serial.println("Authenticating...");

    HTTPClient http;
    http.begin(client, "http://" + String(serverIP) + ":" + String(serverPort) + "/api/login");
    http.addHeader("Content-Type", "application/json");

    Serial.println("[HTTP] POST...");
    int httpCode = http.POST("{\"email\":\"" + String(email) + "\",\"password\":\"" + String(passwordJwt) + "\"}");

    // httpCode will be negative on error
    if (httpCode > 0)
    {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED)
        {
            // Parse the response JSON to get the token
            DynamicJsonDocument doc(1024);
            DeserializationError error = deserializeJson(doc, http.getString());
            if (error)
            {
                Serial.println("Failed to parse JSON.");
                return false;
            }

            strcpy(token, doc["token"]);
            Serial.println("Authentication successful.");
            Serial.println(token);
            gotToken = true;
            saveJWTTokenToFile(token); //save for next time, so we won't need to do this again. 
            return true;
        }
        else
        {
            Serial.print("[HTTP] Error code: ");
            Serial.println(httpCode);
        }
    }
    else
    {
        Serial.println("Connection failed.");
    }

    http.end();
    return false;

    // bool isAuthenticated = true; // Simulated result
    // return isAuthenticated;
}

bool Authentication::checkSavedToken()
{
    // check for saved token, load
    String savedToken = readJWTTokenFromFile();
    if (!savedToken.isEmpty())
    {
        strncpy(token, savedToken.c_str(), sizeof(token) - 1);
        token[sizeof(token) - 1] = '\0'; // Null-terminate the token string
        // strncpy(jwtToken, savedToken.c_str(), sizeof(jwtToken) - 1);
        // jwtToken[sizeof(jwtToken) - 1] = '\0'; // Null-terminate the token string
        gotToken = true;
        Serial.println("Using saved JWT token:");
        Serial.println(token);
        return true;
    }
    else
    {
        return false;
    }
}
