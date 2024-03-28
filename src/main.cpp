#include <Arduino.h>
// #include <LittleFS.h>
// #include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
// #include <ESP8266HTTPClient.h>
// #include <ArduinoJson.h>
#include <EEPROM.h>

#include "secrets.h"
#include "Authentication.h"
#include "Loading.h"
#include "Playing.h"

Authentication authentication;
Loading loading;
Playing playing;

#define led D4 // built in LED on my D1 mini


// WiFiClient client;
ESP8266WiFiMulti WiFiMulti;

void setup()
{
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH); // HIGH is off for D1 mini

  // WiFi connection: 
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi");
  while (WiFiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup: Connection & Authentication
  Serial.println("Setup: Connection & Authentication");

  // Connecting to WiFi and authenticating
  bool isAuthenticated = authentication.checkSavedToken();

  //Check disk for saved auth:
  if (isAuthenticated)
  {
    Serial.println("Authentication found. Use saved.");
    // Loading data
    loading.load();
  }
  else //no saved found: 
  {
    bool isAuthenticating = authentication.authenticate();
    if(isAuthenticating){ //auth success
      if(isAuthenticated){ //check disk again
        Serial.println("Authenticated on with Server. Should be saved now");
      } else
      {
        Serial.println("too soon?. Should be saved here, maybe delay first");
      }
    } else
    {
      Serial.println("Failed to authenticate.");
    }
  }
}

void loop()
{
  // Playing
  playing.play();
  // Add delay if needed
  delay(1000); // Example: Delay for 1 second between each play
}
