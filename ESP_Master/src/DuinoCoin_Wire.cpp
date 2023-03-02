/*
  DuinoCoin_Wire.ino
  created 10 05 2021
  by Luiz H. Cassettari
*/
#include <Arduino.h>
#include <Wire.h>

#include "DuinoCoin_Esp_Async_Master.h"
#include "DuinoCoin_Wire.h"

#ifdef ESP01
#define SDA 0 // GPIO0
#define SCL 2 // GPIO2
#elif ESP8266
#define SDA 4 // D2 - A4 - GPIO4
#define SCL 5 // D1 - A5 - GPIO5
#endif

#if ESP32
#define SDA 21
#define SCL 22
#endif

#define WIRE_CLOCK 100000
#define WIRE_MAX 32

static void wire_start();
static void wire_readAll();
static void Wire_send(byte address, String message);
static boolean wire_runEvery(unsigned long interval);
static boolean wire_runEveryMicro(unsigned long interval);

void wire_setup() {
  // pinMode(SDA, INPUT_PULLUP);
  // pinMode(SCL, INPUT_PULLUP);
  wire_start();
  wire_readAll();
}

static void wire_start() {
  Wire.begin(SDA, SCL);
  Wire.setClock(WIRE_CLOCK);
}

static void wire_readAll() {
  for (byte address = 1; address < WIRE_MAX; address++) {
    if (wire_exists(address)) {
      Serial.print("Address: ");
      Serial.println(address);
      wire_readLine(address);
    }
  }
}

void wire_SendAll(String message) {
  for (byte address = 1; address < WIRE_MAX; address++) {
    if (wire_exists(address)) {
      Serial.print("Address: ");
      Serial.println(address);
      Wire_sendln(address, message);
    }
  }
}

boolean wire_exists(byte address) {
  wire_start();
  Wire.beginTransmission(address);
  byte error = Wire.endTransmission();
  return (error == 0);
}

void wire_sendJob(byte address, String lastblockhash, String newblockhash,
                  int difficulty) {
  String job = lastblockhash + "," + newblockhash + "," + difficulty;
  Wire_sendln(address, job);
}

void Wire_sendln(byte address, String message) {
  Wire_send(address, message + "\n");
}

void Wire_sendCmd(byte address, String message) {
  Wire_send(address, message + '$');
}

static void Wire_send(byte address, String message) {
  wire_start();

  int i = 0;
  while (i < message.length()) {
    if (wire_runEveryMicro(500)) {
      Wire.beginTransmission(address);
      for (int j = 0; j < REPEATED_WIRE_SEND_COUNT; j++) {
        Wire.write(message.charAt(i));
      }
      Wire.endTransmission();
      i++;
    }
  }
}

String wire_readLine(int address) {
  wire_runEvery(0);
  char end = '\n';
  String str = "";
  boolean done = false;
  wire_start();
  while (!done) {
    Wire.requestFrom(address, 1);
    if (Wire.available()) {
      char c = Wire.read();
      // Serial.print(c);
      if (c == end) {
        break;
        done = true;
      }
      str += c;
    }
    if (wire_runEvery(2000))
      break;
  }
  // str += end;
  return str;
}

static boolean wire_runEvery(unsigned long interval) {
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}

static boolean wire_runEveryMicro(unsigned long interval) {
  static unsigned long previousMicros = 0;
  unsigned long currentMicros = micros();
  if (currentMicros - previousMicros >= interval) {
    previousMicros = currentMicros;
    return true;
  }
  return false;
}
