#ifndef LOADING_H
#define LOADING_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <Authentication.h>

/**
 * @file Loading.h
 * @brief Declaration of the Loading class.
 */

class Loading {
public:
    Loading(); // Constructor declaration
    String getTimelineNumber();
    bool getTimeline(String tln);
    void saveTimeline(const String &timelineData);
    bool load();

private:
    /**
     * @brief WiFi client object.
     *
     * This object is used for network communication with the server.
     */
    WiFiClient client;

    /**
     * @brief Authentication object.
     *
     * This object is used for handling authentication operations.
     */
    Authentication authentication;

    /**
     * @brief JWT token buffer.
     *
     * This buffer stores the JWT token retrieved from authentication.
     */
    char token[512];

    /**
     * @brief Default timeline number.
     *
     * This string represents the default timeline number used for file paths.
     */
    String timelineNumber = "0";

    /**
     * @brief File path of the timeline file.
     *
     * This string represents the file path of the timeline data file, saved in LittleFS.
     */
    String timelineFilePath = "/timeline" + timelineNumber + ".txt";
};

#endif
