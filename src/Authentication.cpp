/**
 * @file Authentication.cpp
 * @brief Implementation of the Authentication class.
 * 
 * This class includes methods for authentication using JWT to the Magic Poi Lite server api. 
 * It also has methods to save and load the JWT token from LittleFS. 
 */


#include "Authentication.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <secrets.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

/**
 * @brief Default constructor for Authentication class.
 * 
 * This constructor initializes the Authentication object by clearing the jwtToken buffer.
 */
Authentication::Authentication()
{
    // Initialize variables
    memset(jwtToken, 0, sizeof(jwtToken)); // Clear jwtToken buffer
}


/**
 * @brief Reads JWT token from a file stored on LittleFS.
 * 
 * This method attempts to read a JWT token from a file stored on LittleFS (Little File System).
 * If the file exists, it reads the token from the file and returns it. If the file does not exist
 * or if there's any failure during the file system operation, an empty string is returned.
 * 
 * @return A string containing the JWT token read from the file, or an empty string if the token couldn't be read.
 */
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


/**
 * @brief Saves a JWT token to a file on LittleFS.
 * 
 * This method attempts to save a JWT token to a file stored on LittleFS (Little File System).
 * If LittleFS initialization succeeds and the file is successfully created and written, 
 * a success message is printed to the Serial monitor. If there's any failure during 
 * the file system operation, an error message is printed.
 * 
 * @param token The JWT token to be saved to the file.
 */
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


/**
 * @brief Authenticates the client with the server.
 * 
 * This method attempts to authenticate the client with the server by sending a POST request
 * to the login endpoint with the provided email and password. If the authentication is successful,
 * the JWT token received from the server is parsed and saved, and the method returns true.
 * If any error occurs during the HTTP request or JSON parsing, or if the server returns an error
 * response code, the method returns false.
 * 
 * @return true if the authentication is successful, false otherwise.
 */
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


/**
 * @brief Checks for a saved JWT token and loads it if available.
 * 
 * This method checks if a JWT token is saved in a file. If a token is found,
 * it loads the token into the internal token buffer. If no token is found,
 * the method returns false.
 * 
 * @return true if a saved token is successfully loaded, false otherwise.
 */
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
