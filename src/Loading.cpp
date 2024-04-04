/**
 * @file Loading.cpp
 * @brief Implementation of the Loading class.
 * 
 * This class has methods to fetch the user data from MagicPoi api. 
 * Run on startup - saves timeline to LittleFS for later usage
 */


#include "Loading.h"
#include <Arduino.h>
#include <secrets.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <Authentication.h>

/**
 * @brief Default constructor for Loading class.
 * 
 * This constructor initializes the Loading object. It attempts to load a JWT token 
 * from a file using the Authentication object. If a token is successfully loaded,
 * it is stored internally. Otherwise, it prepares to use password login if token loading fails.
 */
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


/**
 * @brief Retrieves the current timeline number from the server.
 * 
 * This method sends a GET request to the server to retrieve the current timeline number.
 * It includes the JWT token in the request headers for authentication. If the request
 * is successful and the response code is OK, the method returns the timeline number
 * received from the server. If any error occurs during the HTTP request or the response
 * code indicates a failure, an empty string is returned.
 * 
 * @return A string containing the current timeline number, or an empty string if the
 * request fails or if the server response code is not OK.
 */
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


/**
 * @brief Retrieves the timeline data from the server based on the specified timeline number.
 * 
 * This method sends a GET request to the server to retrieve the timeline data corresponding
 * to the provided timeline number. It includes the JWT token in the request headers for authentication.
 * If the request is successful and the response code is OK, the timeline data is saved and the method
 * returns true. If any error occurs during the HTTP request or the response code indicates a failure,
 * the method returns false.
 * 
 * @param tln The timeline number to retrieve data for.
 * @return true if the timeline data is successfully retrieved, false otherwise.
 */
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

/**
 * @brief Saves the timeline data to a file.
 * 
 * This method saves the provided timeline data to a file using the LittleFS (Little File System).
 * If LittleFS initialization succeeds and the file is successfully created and written, 
 * a success message is printed to the Serial monitor. If there's any failure during 
 * the file system operation, the method returns without saving the data.
 * 
 * @param timelineData The timeline data to be saved to the file.
 */
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


/**
 * @brief Loads data required for the application.
 * 
 * This method attempts to load data required for the application. It first tries to retrieve the current
 * timeline number from the server. If successful, it proceeds to load the timeline data. If both operations
 * succeed, the method returns true indicating successful loading. If any of the operations fail, the method
 * returns false.
 * 
 * @return true if data loading is successful, false otherwise.
 */
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
