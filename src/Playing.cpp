#include "Playing.h"
#include <Arduino.h>
#include <Loading.h>
#include <secrets.h>
#include <LittleFS.h>

#define led D4

DynamicJsonDocument led_doc(1500);

// Constructor definition
Playing::Playing()
{
    // String currentTimelineData = loadTimeline();
    // Serial.print("Got timeline Data from disk: ");
    // Serial.println(currentTimelineData);
}

void Playing::processTimelineData(const String &timelineData)
{
    // Serial.println("processTimelineData");
    // DynamicJsonDocument led_doc(1500);
    deserializeJson(led_doc, timelineData);
    root = led_doc.as<JsonObject>();

    iter = 0;
    for (JsonPair kv : root)
    {
        // String key = kv.key().c_str();
        // JsonObject subDict = kv.value().as<JsonObject>();
        // iter = 0;
        // // Process the sub-dictionary
        // for (JsonPair subKv : subDict) {
        Serial.print("got ");
        Serial.println(kv.key().c_str());
        timeline_keys[iter] = kv.key().c_str(); // should be the last one here..
        iter++;
        // }
    }
    Serial.print("iter: ");
    Serial.print(iter);

    maxTimingsNum = iter;

    for (int i = 0; i < iter; i++)
    {
        if (strcmp(timeline_keys[i], "") == 0)
        {
            Serial.println("end");
        }
        else
        {
            redVal = led_doc[timeline_keys[i]][0];
            greenVal = led_doc[timeline_keys[i]][1];
            blueVal = led_doc[timeline_keys[i]][2];

            redInt = redVal.as<int>();
            // initialise arrays for timings:
            colours[i] = redInt;
            timings[i] = atol(timeline_keys[i]);

            // if (redInt == 13) {
            //   Serial.println("LUCKY 13!!!");
            // }
            greenInt = greenVal.as<int>();
            blueInt = blueVal.as<int>();
            Serial.print("got colours for ");
            Serial.print(timeline_keys[i]);
            Serial.print(": ");
            Serial.print(redInt);
            Serial.print(", ");
            Serial.print(greenInt);
            Serial.print(", ");
            Serial.println(blueInt);
        }
    }
    // todo: errors from here?

    // Print the colours array
    Serial.print("Colours from disk: ");
    for (int i = 0; i < iter; i++)
    {
        Serial.print(colours[i]);
        Serial.print(" ");
    }
    Serial.println(); // Print a new line after the array
    // Print the timeline array
    Serial.print("Timings from disk: ");
    for (int i = 0; i < iter; i++)
    {
        Serial.print(timings[i]);
        Serial.print(" ");
    }
    Serial.println(); // Print a new line after the array
    already_got_data = true;
    digitalWrite(led, LOW);

    // todo: test:
    playStartTime = millis();
}

String Playing::loadTimeline() // load from disk
{
    String timelineData = "";
    if (LittleFS.begin())
    {
        if (LittleFS.exists(timelineFilePath))
        {
            File file = LittleFS.open(timelineFilePath, "r");
            if (file)
            {
                timelineData = file.readString();
                file.close();
                Serial.println("Timeline data:");
                Serial.println(timelineData);
                processTimelineData(timelineData); // Process the timeline data. Todo: error here?
            }
        }
        LittleFS.end();
    }
    return timelineData;
}

void Playing::play()
{
    digitalWrite(led, HIGH);
    // Simulate playing process
    Serial.println("Playing...");
    // Main function of the app here
    String currentTimelineData = loadTimeline();
    Serial.print("Got timeline Data from disk: ");
    Serial.println(currentTimelineData);
}