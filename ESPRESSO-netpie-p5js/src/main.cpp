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

#include "init_mqtt.h"
#include "_publish.h"
#include "_receive.h"
#include "_config.h"


#define rxPin (16)
#define txPin (17)

#define MODE 0

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

  pinMode(MODE, INPUT_PULLUP);

  digitalWrite(relayPin, relayPinState);
  ;
  // serial port initialization
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, rxPin, txPin, false);
  Serial1.print("50\n");
  delay(10);
  Serial.println();
  Serial.println("Starting...");
  
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
  if (digitalRead(MODE) == 0)
  {
    delay(200);
    // btCount += 1;
    // Serial.println(btCount);
    //    sync_advpub("prefix", "topic", "payload", "retain")
    mqtt->sync_advpub("","/devSuperman/gearname/espRobot", String(10), false);
  }
  mqtt->loop();
}