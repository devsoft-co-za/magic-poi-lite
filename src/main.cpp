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
  // isAuthenticated = false; //test re-auth 
  //Check disk for saved JWT auth and use it:
  if (isAuthenticated)
  {
    Serial.println("Authentication found. Using saved.");
    // Loading data
    if(loading.load()){
          Serial.println("LOADED AND SAVED TIMELINE SUCCESSFULLY");
        } else{
          Serial.println("LOADING AND SAVING TIMELINE UNSUCCESSFUL");
        } //todo: loading needs to tell us if authentication is unsuccessful, so we can try password auth?
  }
  else //no saved JWT found: 
  {
    bool isAuthenticating = authentication.authenticate();
    if(isAuthenticating){ //auth success
      isAuthenticated = authentication.checkSavedToken(); //check again for testing, just for testing?
      if(isAuthenticated){ 
        Serial.println("Authenticated using login success. Should be saved now, using..");
        if(loading.load()){
          Serial.println("LOADED AND SAVED TIMELINE SUCCESSFULLY");
        } else{
          Serial.println("LOADING AND SAVING TIMELINE UNSUCCESSFUL");
        }
      } else
      {
        Serial.println("Auth check saved JWT after password login failed");
      }
    } else
    {
      Serial.println("Failed to authenticate with password");
    }
  }
}

void loop()
{
  // Playing
  playing.play();
  // Add delay if needed
  delay(5000); // Example: Delay for 1 second between each play
}
