#ifndef LOADING_H
#define LOADING_H
#include <Arduino.h>
#include <WiFiClient.h>
#include <Authentication.h>
class Loading {
public:
    Loading(); // Constructor declaration
    String getTimelineNumber();
    bool getTimeline(String tln);
    void saveTimeline(const String &timelineData);
    bool load();
private:
    WiFiClient client;
    Authentication authentication;
    char token[512];
    String timelineNumber = "0";
    String timelineFilePath = "/timeline" + timelineNumber + ".txt";
};

#endif
