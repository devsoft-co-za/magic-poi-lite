/**
 * @file Main.cpp
 * @brief Main program file.
 * 
 * The main program - includes setup() and loop(). 
 * Currently set to connect to Magic Poi Lite API on startup and fetch the current timeline. 
 * Then display the timeline on RGB LEDs according to timings.
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <EEPROM.h>

#include "secrets.h"
#include "Authentication.h"
#include "Loading.h"
#include "Playing.h"

/**
 * @brief Instance of the Authentication class.
 *
 * This object is used for handling authentication operations.
 */
Authentication authentication;

/**
 * @brief Instance of the Loading class.
 *
 * This object is used for loading timeline data.
 */
Loading loading;

/**
 * @brief Instance of the Playing class.
 *
 * This object is used for playing timeline data.
 */
Playing playing;

/**
 * @brief Pin number for the built-in LED.
 *
 * This constant represents the pin number for the built-in LED on the D1 Mini.
 */
#define led D4

/**
 * @brief Pin number for the blue LED.
 *
 * This constant represents the pin number for the blue LED.
 */
const int blueLEDPin = D5;

/**
 * @brief Pin number for the green LED.
 *
 * This constant represents the pin number for the green LED.
 */
const int greenLEDPin = D6;

/**
 * @brief Pin number for the red LED.
 *
 * This constant represents the pin number for the red LED.
 */
const int redLEDPin = D7;

/**
 * @brief Pin number for the update Button.
 *
 * This constant represents the pin number for the update Button.
 * Used for fetching content from the web.
 */
const int btnUpdatePin = D1;

/**
 * @brief Pin number for the start Button.
 *
 * This constant represents the pin number for the start Button.
 */
const int btnStartPin = D2;

/**
 * @brief WiFi multi-client manager for ESP8266.
 *
 * This object is used for managing multiple WiFi connections on ESP8266.
 */
ESP8266WiFiMulti WiFiMulti;

/**
 * @brief Flag indicating whether an update is requested.
 *
 * This flag is set to true when an update is requested via the update button interrupt.
 */
volatile bool updateRequested = false;

/**
 * @brief Interrupt service routine for handling update button press.
 *
 * This function sets the updateRequested flag to true when the update button is pressed.
 */
void ICACHE_RAM_ATTR handleUpdateInterrupt() {
  updateRequested = true;
}

/**
 * @brief Handles authentication and loading of timeline data.
 * 
 * This function performs the process of checking for saved authentication tokens,
 * handling authentication, and loading timeline data. It prints messages to indicate
 * the status of these operations.
 */
void handleAuthenticationAndLoading() {
  Serial.println("Setup: Connection & Authentication");
  
  bool isAuthenticated = authentication.checkSavedToken();
  isAuthenticated = false; // test re-auth - todo: tie this to a button press - todo: not working without this?

  if (isAuthenticated) {
    Serial.println("Authentication found. Using saved.");
    if (loading.load()) {
      Serial.println("LOADED AND SAVED TIMELINE SUCCESSFULLY");
      if (playing.setup()) {
        Serial.println("SETUP COMPLETE");
      }
    } else {
      Serial.println("LOADING AND SAVING TIMELINE UNSUCCESSFUL");
    }
  } else {
    bool isAuthenticating = authentication.authenticate();
    if (isAuthenticating) {
      isAuthenticated = authentication.checkSavedToken();
      if (isAuthenticated) {
        Serial.println("Authenticated using login success. Should be saved now, using..");
        if (loading.load()) {
          Serial.println("LOADED AND SAVED TIMELINE SUCCESSFULLY");
          if (playing.setup()) {
            Serial.println("SETUP COMPLETE");
          }
        } else {
          Serial.println("LOADING AND SAVING TIMELINE UNSUCCESSFUL");
        }
      } else {
        Serial.println("Auth check saved JWT after password login failed");
      }
    } else {
      Serial.println("Failed to authenticate with password");
    }
  }
}

/**
 * @brief Sets up the system including WiFi connection, authentication, and loading and playing timeline data.
 * 
 * This function initializes the system by establishing a WiFi connection, setting up interrupt for update button,
 * and calling handleAuthenticationAndLoading() to handle authentication and loading processes.
 */
void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH); // HIGH is off for D1 mini

  // Set the digital pins as outputs
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
  pinMode(btnUpdatePin, INPUT_PULLUP);

  // WiFi connection
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi");
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up interrupt for update button
  attachInterrupt(digitalPinToInterrupt(btnUpdatePin), handleUpdateInterrupt, FALLING);

  // Handle authentication and loading
  handleAuthenticationAndLoading();
}

/**
 * @brief Main loop of the program.
 * 
 * This function is the main loop of the program. It checks for update requests and calls
 * handleAuthenticationAndLoading() if an update is requested. It also calls the play function
 * to execute the playing process, which involves changing LED colors over time according to
 * the timeline data. Additionally, it adds a delay of 5 seconds between each play iteration
 * to control the timing of the LED color changes.
 */
void loop() {
  if (updateRequested) {
    updateRequested = false;
    handleAuthenticationAndLoading();
  }

  playing.play();
  delay(5000); // Example: Delay for 5 seconds between each play
}
