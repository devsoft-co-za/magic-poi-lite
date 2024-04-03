#ifndef PLAYING_H
#define PLAYING_H

#include <Arduino.h>
#include <ArduinoJson.h>


class Playing {
public:
    Playing(); // Constructor declaration
    void processTimelineData(const String &timelineData);
    String loadTimeline();

    void play();

private:
    String timelineNumber = "0";
    String timelineFilePath = "/timeline" + timelineNumber + ".txt";
    const char *timeline_keys[50]; // array for keys
    int maxTimingsNum = 50; // using only 50 for now

    JsonVariant redVal;
    JsonVariant greenVal;
    JsonVariant blueVal;

    int redInt = 1;
    int greenInt = 1;
    int blueInt = 1;

    long timings[50];
    uint8_t colours[50];
    bool already_got_data = false;
    long playStartTime = 0;

    JsonObject root;
    int iter = 0;

};

#endif
