#ifndef PLAYING_H
#define PLAYING_H

#include <Arduino.h>
#include <ArduinoJson.h>

/**
 * @file Playing.h
 * @brief Declaration of the Playing class.
 */

class Playing
{
public:
    Playing(); // Constructor declaration
    void processTimelineData(const String &timelineData);
    int *getColours();
    long *getTimings();
    int getMaxTimingsNum();
    String loadTimeline();
    bool setup();
    void play();
    void changeColours(int choice);
    void useTimelineData();

private:
    /**
     * @brief Number of timings in the timeline.
     *
     * This variable stores the maximum number of timings in the timeline.
     * It is used to limit the size of the timings array.
     */
    int maxTimingsNum = 50;

    /**
     * @brief File path of the timeline file.
     *
     * This variable stores the file path of the timeline data file.
     */
    String timelineFilePath;

    /**
     * @brief Array of keys in the timeline JSON.
     *
     * This array stores the keys extracted from the timeline JSON data.
     */
    const char *timeline_keys[50]; // array for keys

    /**
     * @brief Red color value extracted from JSON.
     */
    JsonVariant redVal;

    /**
     * @brief Green color value extracted from JSON.
     */
    JsonVariant greenVal;

    /**
     * @brief Blue color value extracted from JSON.
     */
    JsonVariant blueVal;

    /**
     * @brief Red color value as an integer.
     */
    int redInt = 1;

    /**
     * @brief Green color value as an integer.
     */
    int greenInt = 1;

    /**
     * @brief Blue color value as an integer.
     */
    int blueInt = 1;

    /**
     * @brief Array storing timings extracted from JSON.
     */
    long timings[50];

    /**
     * @brief Array storing colors extracted from JSON.
     */
    int colours[50];

    /**
     * @brief Flag indicating whether timeline data has been loaded.
     */
    bool already_got_data = false;

    /**
     * @brief Timestamp when timeline playing started.
     */
    long playStartTime = 0;

    /**
     * @brief JSON object root.
     */
    JsonObject root;

    /**
     * @brief Iterator for timeline data.
     */
    int iter = 0;

    /**
     * @brief Signal indicator.
     */
    uint8_t signal;

    /**
     * @brief Timestamp of the last LED update.
     */
    long previousMillis = 0;

    /**
     * @brief Current timestamp.
     */
    long currentMillis2 = 0;

    /**
     * @brief Flag indicating whether playing is active.
     */
    bool playing = true;

    /**
     * @brief Run number.
     */
    int runNum = 0;

    /**
     * @brief Current index in the timeline data arrays.
     */
    int currentIndex = 0;
};

#endif
