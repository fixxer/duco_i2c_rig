/*
  DuinoCoin_Pool.ino
  created 31 07 2021
  by Luiz H. Cassettari
  
  Modified by JK-Rolling
*/
#include <Arduino.h>

#if ESP8266
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#endif
#if ESP32
#include <HTTPClient.h>
#endif

#include <ArduinoJson.h>

#include "DuinoCoin_AsyncWebServer.h"
#include "DuinoCoin_Clients.h"
#include "DuinoCoin_Esp_Async_Master.h"

const char * urlPool = "https://server.duinocoin.com/getPool";
const char * urlMiningKeyStatus = "https://server.duinocoin.com/mining_key";

extern const char* ducouser;

extern String mining_key;

static void CheckMiningKey(String input);
static String httpGetString(String URL);

void UpdateHostPort(String input)
{
  DynamicJsonDocument doc(256);
  deserializeJson(doc, input);

  const char* name = doc["name"];
  const char* ip = doc["ip"];
  int port = doc["port"];

  Serial.println("[ ]Update " + String(name) + " " + String(ip) + " " + String(port));
  SetHostPort(String(ip), port);
}

void UpdatePool()
{
  String input = httpGetString(urlPool);
  if (input == "") return;
  
  UpdateHostPort(input);
}

static String httpGetString(String URL)
{
  String payload = "";
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  if (http.begin(client, URL))
  {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)
    {
      payload = http.getString();
    }
    else
    {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  return payload;
}

void UpdateMiningKey()
{
    String url = String(urlMiningKeyStatus) + "?u=" + String(ducouser) + "&k=" + mining_key;
    String input = httpGetString(url);
    if (input == "") return;

    CheckMiningKey(input);
}

static void CheckMiningKey(String input) 
{
    Serial.println("[ ] CheckMiningKey " + input);
    DynamicJsonDocument doc(128);
    deserializeJson(doc, input);

    //input::{"has_key":false,"success":true}
    bool has_key = doc["has_key"];
    bool success = doc["success"];

    Serial.println("[ ] mining_key has_key: " + String(has_key) + "  success: " + String(success));

    if (success && !has_key) {
        Serial.println("[ ] Wallet does not have a mining key. Proceed..");
        SetMiningKey("None");
    }
    else if (!success) {
        if (mining_key == "None") {
            Serial.println("[ ] Update mining_key to proceed. Halt..");
            ws_sendAll("Update mining_key to proceed. Halt..");
            for(;;);
        }
        else {
            Serial.println("[ ] Invalid mining_key. Halt..");
            ws_sendAll("Invalid mining_key. Halt..");
            for(;;);
        }
    }
    else {
        Serial.println("[ ] Updated mining_key..");
        SetMiningKey(mining_key);
    }
}
