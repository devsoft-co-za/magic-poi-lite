#include "Loading.h"
#include <Arduino.h>
#include <secrets.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <Authentication.h>

// Constructor definition
Loading::Loading()
{
    // token = authentication.readJWTTokenFromFile(); //todo: add logic to use Password login if this fails
    String savedToken = authentication.readJWTTokenFromFile();
    if (!savedToken.isEmpty())
    {
        strncpy(token, savedToken.c_str(), sizeof(token) - 1);
        token[sizeof(token) - 1] = '\0'; // Null-terminate the token string
        // strncpy(jwtToken, savedToken.c_str(), sizeof(jwtToken) - 1);
        // jwtToken[sizeof(jwtToken) - 1] = '\0'; // Null-terminate the token string
        // gotToken = true;
        Serial.println("Using saved JWT token:");
        Serial.println(token);
    }
}

String Loading::getTimelineNumber()
{
    HTTPClient http;
    http.begin(client, "http://" + String(serverIP) + ":" + String(serverPort) + "/lite/api/get-current-timeline-number");
    http.addHeader("Authorization", "Bearer " + String(token));

    Serial.println("[HTTP] GET...");
    int httpCode = http.GET();
    String response = "";
    // httpCode will be negative on error
    if (httpCode > 0)
    {
        if (httpCode == HTTP_CODE_OK)
        {
            // Print the API response
            response = http.getString();
            Serial.print("Got timeline number: ");
            Serial.println(response);
        }
        else
        {
            Serial.print("Failed to get Timeline Number - Error code: ");
            Serial.println(httpCode);
        }
    }
    else
    {
        Serial.println("Connection failed.");
    }

    http.end();
    return response;
}

// get timeline and save on disk
bool Loading::getTimeline(String tln)
{
    HTTPClient http;
    http.begin(client, "http://" + String(serverIP) + ":" + String(serverPort) + "/lite/api/load-timeline?number=" + tln);
    http.addHeader("Authorization", "Bearer " + String(token));

    Serial.println("[HTTP] GET...");
    int httpCode = http.GET();
    // httpCode will be negative on error
    if (httpCode > 0)
    {
        if (httpCode == HTTP_CODE_OK)
        {
            // Print the API response
            // DynamicJsonDocument led_doc(1500);
            String payload = http.getString();
            saveTimeline(payload);
            Serial.println("got Timeline: ");
            Serial.println(payload);
            // delay(10);
            // loadTimeline();

            // already_got_data = true;
            // digitalWrite(led, LOW);
            http.end();
            return true;
        }
        else
        {
            Serial.print("Failed to get Timeline - Error Code: ");
            Serial.println(httpCode);
            // gotToken=false;
        }
    }
    else
    {
        Serial.println("Connection failed.");
        // gotToken=false;
    }

    http.end();
    return false;
}

void Loading::saveTimeline(const String &timelineData)
{
    if (LittleFS.begin())
    {
        File file = LittleFS.open(timelineFilePath, "w");
        if (file)
        {
            file.print(timelineData);
            file.close();
            Serial.println("Timeline data saved to file.");
        }
        LittleFS.end();
    }
}

//todo: loadTimeline, processTimelineData here?

bool Loading::load()
{
    //todo: load saved timeline if not able to access internet or auth doesn't work? 
    Serial.println("Loading data...");

    //load from api: 
    timelineNumber = getTimelineNumber();
    if(getTimeline(timelineNumber)){
        return true;
    } 

    return false;
    
}
