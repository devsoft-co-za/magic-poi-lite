/**
 * @file Playing.cpp
 * @brief Implementation of the Playing class.
 * 
 * This class has methods to parse the Json format timelines display the colours on LED's. 
 * Runs in Loop()
 * 
 */


#include "Playing.h"
#include <Arduino.h>
#include <Loading.h>
#include <secrets.h>
#include <LittleFS.h>

#define led D4
const int blueLEDPin = D5;
const int greenLEDPin = D6;
const int redLEDPin = D7;

DynamicJsonDocument led_doc(1500);

// Constructor definition
Playing::Playing()
{
    // String currentTimelineData = loadTimeline();
    // Serial.print("Got timeline Data from disk: ");
    // Serial.println(currentTimelineData);
}


/**
 * @brief Processes the timeline data received from the server.
 * 
 * This method processes the timeline data received from the server. It deserializes the JSON data,
 * extracts key-value pairs, and stores the LED colors and timings for each event in the timeline.
 * 
 * @param timelineData The timeline data received from the server.
 */
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

    // // Print the colours array
    // Serial.print("Colours from disk: ");
    // for (int i = 0; i < iter; i++)
    // {
    //     Serial.print(colours[i]);
    //     Serial.print(" ");
    // }
    // Serial.println(); // Print a new line after the array
    // // Print the timeline array
    // Serial.print("Timings from disk: ");
    // for (int i = 0; i < iter; i++)
    // {
    //     Serial.print(timings[i]);
    //     Serial.print(" ");
    // }
    // Serial.println(); // Print a new line after the array
    already_got_data = true;
    digitalWrite(led, LOW);

    // todo: test:
    playStartTime = millis();
}

/**
 * @brief Returns the array of LED colors.
 * 
 * This method returns a pointer to the array containing LED colors extracted from the timeline data.
 * 
 * @return Pointer to the array of LED colors.
 */
int *Playing::getColours()
{
    return colours;
}

/**
 * @brief Returns the array of timings.
 * 
 * This method returns a pointer to the array containing timings extracted from the timeline data.
 * 
 * @return Pointer to the array of timings.
 */
long *Playing::getTimings()
{
    return timings;
}

/**
 * @brief Returns the maximum number of timings in the timeline.
 * 
 * This method returns the maximum number of timings available in the timeline data.
 * 
 * @return Maximum number of timings in the timeline.
 */
int Playing::getMaxTimingsNum()
{
    return maxTimingsNum;
}

/**
 * @brief Loads the timeline data from the disk.
 * 
 * This method loads the timeline data from the disk using the LittleFS (Little File System).
 * If LittleFS initialization succeeds and the file exists, the method reads the data from the file,
 * processes it using the processTimelineData() method, and returns the timeline data as a string.
 * If the file doesn't exist or if there's any failure during the file system operation, an empty string
 * is returned.
 * 
 * @return A string containing the loaded timeline data, or an empty string if the data couldn't be loaded.
 */
String Playing::loadTimeline() // load from disk
{
    Serial.print("Loading Timeline from LittleFS ");
    Serial.println(timelineFilePath);
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


/**
 * @brief Sets up the playing process.
 * 
 * This method sets up the playing process by initializing necessary components,
 * loading timeline data from the disk, and processing it.
 * 
 * @return true if setup is successful, false otherwise.
 */
bool Playing::setup()
{
    digitalWrite(led, HIGH);
    // Simulate playing process
    Serial.println("Playing...");
    // Main function of the app here
    String currentTimelineData = loadTimeline();
    Serial.print("Got timeline Data from disk: ");
    Serial.println(currentTimelineData);
    processTimelineData(currentTimelineData);
    return true;
}


/**
 * @brief Changes the colors of the RGB LED based on the provided choice.
 * 
 * This method changes the colors of the RGB LED based on the provided choice.
 * 
 * @param choice An integer representing the color choice:
 *               - 0: Red
 *               - 1: Green
 *               - 2: Blue
 *               - 3: Cyan
 *               - 4: Magenta
 *               - 5: Yellow
 *               - 6: White
 *               - Any other value: Default (turn off the LED)
 */
void Playing::changeColours(int choice)
{
    switch (choice)
    {
    case 0: // red
        analogWrite(redLEDPin, 255);
        analogWrite(greenLEDPin, 0);
        analogWrite(blueLEDPin, 0);
        Serial.println("RED");
        break;
    case 1: // green
        analogWrite(redLEDPin, 0);
        analogWrite(greenLEDPin, 255);
        analogWrite(blueLEDPin, 0);
        Serial.println("GREEN");
        break;
    case 2: // blue
        analogWrite(redLEDPin, 0);
        analogWrite(greenLEDPin, 0);
        analogWrite(blueLEDPin, 255);
        Serial.println("BLUE");
        break;
    case 3: // cyan
        analogWrite(redLEDPin, 0);
        analogWrite(greenLEDPin, 255);
        analogWrite(blueLEDPin, 255);
        Serial.println("CYAN");
        break;
    case 4: // magenta
        analogWrite(redLEDPin, 255);
        analogWrite(greenLEDPin, 0);
        analogWrite(blueLEDPin, 255);
        Serial.println("MAGENTA");
        break;
    case 5: // yellow
        analogWrite(redLEDPin, 255);
        analogWrite(greenLEDPin, 255);
        analogWrite(blueLEDPin, 0);
        Serial.println("YELLOW");
        break;
    case 6: // white
        analogWrite(redLEDPin, 255);
        analogWrite(greenLEDPin, 255);
        analogWrite(blueLEDPin, 255);
        Serial.println("WHITE");
        break;
    default:
    //todo: add more colours - default is shown for all strobe effects currently
        analogWrite(redLEDPin, 0);
        analogWrite(greenLEDPin, 0);
        analogWrite(blueLEDPin, 0);
        Serial.println("DEFAULT");
        break;
    }
}




/**
 * @brief Uses the timeline data to change LED colors over time.
 * 
 * This method uses the timeline data to change LED colors over time. It checks if it's time
 * to change colors based on the timings in the timeline data. If it's time, it changes the colors
 * according to the current index in the timeline data and moves to the next index. If the end of the
 * timeline data is reached, it loops back to the beginning.
 */
void Playing::useTimelineData()
{
    // Get the current time
    currentMillis2 = millis();

    // Check if it's time to change colors
    if (currentMillis2 - previousMillis >= timings[currentIndex]) {
        // Update the previousMillis for the next iteration
        previousMillis = currentMillis2;
        Serial.print(timings[currentIndex]);
        Serial.print(": ");
        // Change the colors based on the current index
        changeColours(colours[currentIndex]);

        // Move to the next index
        currentIndex++;

        // If we reach the end of the array, loop back to the beginning
        if (currentIndex >= maxTimingsNum) {
            currentIndex = 0;
        }
    }
//     Serial.println("play");
//   // test:
//   if (playing)
//   {
//     Serial.println("checking Timeline here");
//     if (runNum > maxTimingsNum - 2) // todo: ???
//     {
//       // Serial.println("initialising runNum and playStartTime");s
//       runNum = 0;
//       playStartTime = millis();
//     }
//     // another sanity check:
//     if (timings[runNum] > 0)
//     {
//       Serial.print("Checking timing for ");
//       Serial.print(timings[runNum]);
//       currentMillis2 = millis() - playStartTime; // reset the clock! At the beginning of play this should be 0!

//       if (currentMillis2 < timings[0])
//       {
//         Serial.print("< ");
//         Serial.println(runNum);
//         runNum++;
//       }
//       //   I think currently timings[] array must be in time order to work. If the last time is smaller than the previous nothing happens? todo: sort?
//       else if (currentMillis2 >= timings[runNum] && currentMillis2 <= timings[runNum + 1]) // todo: this omits the last signal..?
//       {                                                                                    // saved signal
//                                                                                            // Serial.print("== ");
//         Serial.print(runNum);
//         Serial.print(" of: ");
//         Serial.println(maxTimingsNum - 1);
//         // sanity check:
//         // if (colours[runNum] < 13)
//         // { // max is 22 otherwise it's a bogus signal
//         signal = colours[runNum];
//         // long time = timings[runNum];
//         // flashy = flashes[runNum];
//         Serial.print("colour is set to: ");
//         Serial.println(colours[runNum]);
//         Serial.print("timings is set to: ");
//         Serial.println(timings[runNum]);
//         changeColours(colours[runNum]);
//         runNum++;
//         // todo: deal with colours here - separate funciton
//         // }
//       }
//       else if (currentMillis2 >= timings[runNum + 1])
//       {
//         runNum = maxTimingsNum; // went over - trigger reset
//       }
//       else
//       {
//         // test code:
//         //  Serial.println("what else? ");
//         //  Serial.println("runNum: ");
//         //  Serial.println(runNum);
//         //  Serial.print("millis: ");
//         //  Serial.println(millis());
//         //  Serial.print("currentMillis2: ");
//         //  Serial.println(currentMillis2);
//         //  Serial.print("timings[runNum]: ");
//         //  Serial.println(timings[runNum]);
//         //  Serial.print("timings[runNum + 1]: ");
//         //  Serial.println(timings[runNum + 1]);

//         playStartTime = millis();
//         Serial.println("resetting playStartTime");
//         runNum++;
//       }
//     }
//   }
}


/**
 * @brief Plays the timeline data.
 * 
 * This method plays the timeline data by repeatedly calling the useTimelineData() method,
 * which changes LED colors over time according to the timeline data.
 */
void Playing::play()
{
    // int num_timings = getMaxTimingsNum();
    // // Print the colours array
    // Serial.print("Colours in memory: ");
    // for (int i = 0; i < num_timings; i++)
    // {
    //     Serial.print(colours[i]);
    //     Serial.print(" ");
    // }
    // Serial.println(); // Print a new line after the array
    // // Print the timeline array
    // Serial.print("Timings in memory: ");
    // for (int i = 0; i < num_timings; i++)
    // {
    //     Serial.print(timings[i]);
    //     Serial.print(" ");
    // }
    // Serial.println(); // Print a new line after the array
    useTimelineData();
}

