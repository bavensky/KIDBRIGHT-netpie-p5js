#include <Arduino.h>
#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <ArduinoJson.h>
#include <MqttConnector.h>
#include <Wire.h>
#include <SPI.h>
#include "ht16k33.h"

#include "init_mqtt.h"
#include "_publish.h"
#include "_receive.h"
#include "_config.h"

#define rxPin (16)
#define txPin (17)

HT16K33 HT;
MqttConnector *mqtt;

int relayPin = 26;
int relayPinState = HIGH;
char myName[40];
int x, y;
uint32_t btCount = 0;
uint32_t direction = 50;

void init_hardware()
{
  pinMode(relayPin, OUTPUT);
  pinMode(2, OUTPUT);

  digitalWrite(relayPin, relayPinState);

  // serial port initialization
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, rxPin, txPin, false);
  delay(10);
  Serial.println();
  Serial.println("Starting...");

  // HT.begin(0x00);
  // // flash the LEDs, first turn them on
  // Serial.println(F("Turn on all LEDs"));
  // for (led = 0; led < 128; led++)
  // {
  //   HT.setLedNow(led);
  //   delay(10);
  // } // for led

  // //Next clear them
  // Serial.println(F("Clear all LEDs"));
  // for (led = 0; led < 128; led++)
  // {
  //   HT.clearLedNow(led);
  //   delay(10);
  // } // for led
  
}

void init_wifi()
{
  WiFi.disconnect();
  delay(20);
  WiFi.mode(WIFI_STA);
  delay(50);
  const char *ssid = WIFI_SSID.c_str();
  const char *pass = WIFI_PASSWORD.c_str();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.printf("Connecting to %s:%s\r\n", ssid, pass);
    delay(300);
  }
  Serial.println("WiFi Connected.");
  digitalWrite(2, HIGH);
}

void setup()
{
  init_hardware();
  init_wifi();
  init_mqtt();
}

void loop()
{
  if (digitalRead(S1) == 0)
  {
    delay(200);
    btCount += 1;
    Serial.println(btCount);
    //    sync_advpub("prefix", "topic", "payload", "retain")
    mqtt->sync_advpub("", "/KIDBRIGHT/gearname/KB001", String(btCount), false);
  }
  mqtt->loop();
}