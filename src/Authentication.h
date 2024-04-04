#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <Arduino.h>
#include <WiFiClient.h>

/**
 * @file Authentication.h
 * @brief Declaration of the Authentication class.
 */

class Authentication {
public:
    Authentication(); // Constructor declaration
    String readJWTTokenFromFile();
    void saveJWTTokenToFile(const char *token);
    bool authenticate();
    bool checkSavedToken();

private:
    /**
     * @brief Buffer for JWT token.
     *
     * This buffer stores the JWT token retrieved from authentication.
     */
    char token[500];

    /**
     * @brief Buffer for JWT token.
     *
     * This buffer stores the JWT token retrieved from LittleFS.
     */
    char jwtToken[500];

    /**
     * @brief Flag indicating whether a token has been retrieved.
     */
    boolean gotToken = false;

    /**
     * @brief Path to the file storing JWT token.
     *
     * This string represents the file path of the file storing the JWT token in LittleFS.
     */
    const char *jwtFilePath = "/jwt.txt";

    /**
     * @brief WiFi client object.
     *
     * This object is used for network communication with the server.
     */
    WiFiClient client;
};

#endif
