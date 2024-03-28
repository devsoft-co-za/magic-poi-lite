#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H
#include <Arduino.h>
#include <WiFiClient.h>

class Authentication {
public:
    Authentication(); // Constructor declaration
    String readJWTTokenFromFile();
    void saveJWTTokenToFile(const char *token);
    bool authenticate();
    bool checkSavedToken();
private:
    char token[256];
    // Buffer for JWT token
    char jwtToken[256];
    boolean gotToken = false;
    // Path to the file storing JWT token
    const char *jwtFilePath = "/jwt.txt";
    WiFiClient client;
};

#endif
