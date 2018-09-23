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

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
Adafruit_8x16matrix matrix = Adafruit_8x16matrix();

#include "init_mqtt.h"
#include "_publish.h"
#include "_receive.h"
#include "_config.h"

// #define rxPin (16)
// #define txPin (17)
#define S1 16
#define S2 14

MqttConnector *mqtt;

bool statusDisplay = false;

int relayPin = 26;
int relayPinState = HIGH;
char myName[40];
int x, y;
uint32_t btCount = 0;
uint32_t direction = 50;

uint32_t timenow, time1;
int up_com, lr_com;
int countEye = 0;

// Matrix Display
static const uint8_t PROGMEM
    smile_bmp[] =
        {B00111100,
         B01000010,
         B10100101,
         B10000001,
         B10100101,
         B10011001,
         B01000010,
         B00111100},
    neutral_bmp[] =
        {B00111100,
         B01000010,
         B10100101,
         B10000001,
         B10111101,
         B10000001,
         B01000010,
         B00111100},
    frown_bmp[] =
        {B00111100,
         B01000010,
         B10100101,
         B10000001,
         B10011001,
         B10100101,
         B01000010,
         B00111100},
    go_forward_bmp[] =
        {B00000000,
         B00011000,
         B00111100,
         B01111110,
         B00011000,
         B00011000,
         B00011000,
         B00000000},
    go_backward_bmp[] =
        {B00000000,
         B00011000,
         B00011000,
         B00011000,
         B01111110,
         B00111100,
         B00011000,
         B00000000},
    go_left_bmp[] =
        {B00000000,
         B00010000,
         B00110000,
         B01111100,
         B01111110,
         B00110110,
         B00010110,
         B00000000},
    go_right_bmp[] =
        {B00000000,
         B00001000,
         B00001100,
         B00111110,
         B01111110,
         B01101100,
         B01101000,
         B00000000},
    go_stop_bmp[] =
        {B00000000,
         B01100110,
         B00111100,
         B00011000,
         B00011000,
         B00111100,
         B01100110,
         B00000000},
    go_bank_bmp[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    go_ok_bmp[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    eye_1l[] =
        {B00000000,
         B01111100,
         B11111110,
         B11000110,
         B11000110,
         B11111110,
         B01111100,
         B00000000},
    eye_1r[] =
        {B00000000,
         B00111110,
         B01111111,
         B01100011,
         B01100011,
         B01111111,
         B00111110,
         B00000000},
    eye_2l[] =
        {B00000000,
         B01111100,
         B11111110,
         B00011110,
         B00011110,
         B11111110,
         B01111100,
         B00000000},
    eye_2r[] =
        {B00000000,
         B00111110,
         B01111111,
         B00001111,
         B00001111,
         B01111111,
         B00111110,
         B00000000},
    eye_3l[] =
        {B00000000,
         B01111100,
         B11111110,
         B11110000,
         B11110000,
         B11111110,
         B01111100,
         B00000000},
    eye_3r[] =
        {B00000000,
         B00111110,
         B01111111,
         B01111000,
         B01111000,
         B01111111,
         B00111110,
         B00000000},
    eye_22l[] =
        {B00000000,
         B01111100,
         B11111110,
         B10001110,
         B10001110,
         B11111110,
         B01111100,
         B00000000},
    eye_22r[] =
        {B00000000,
         B00111110,
         B01111111,
         B01000111,
         B01000111,
         B01111111,
         B00111110,
         B00000000},
    eye_32l[] =
        {B00000000,
         B01111100,
         B11111110,
         B11100010,
         B11100010,
         B11111110,
         B01111100,
         B00000000},
    eye_32r[] =
        {B00000000,
         B00111110,
         B01111111,
         B01110001,
         B01110001,
         B01111111,
         B00111110,
         B00000000},
    eye_4l[] =
        {B00000000,
         B01111100,
         B11000110,
         B11000110,
         B11111110,
         B11111110,
         B01111100,
         B00000000},
    eye_4r[] =
        {B00000000,
         B00111110,
         B01100011,
         B01100011,
         B01111111,
         B01111111,
         B00111110,
         B00000000},
    eye_5l[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    eye_5r[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    R_eye11[] =
        {B00000000,
         B00000000,
         B11111110,
         B11000110,
         B11000110,
         B11111110,
         B01111100,
         B00000000},
    L_eye11[] =
        {B00000000,
         B00000000,
         B01111111,
         B01100011,
         B01100011,
         B01111111,
         B00111110,
         B00000000},
    R_eye22[] =
        {B00000000,
         B00000000,
         B00000000,
         B11000110,
         B11000110,
         B11111110,
         B01111100,
         B00000000},
    L_eye22[] =
        {B00000000,
         B00000000,
         B00000000,
         B01100011,
         B01100011,
         B01111111,
         B00111110,
         B00000000},
    R_eye33[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B11000110,
         B11111110,
         B01111100,
         B00000000},
    L_eye33[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B01100011,
         B01111111,
         B00111110,
         B00000000},
    R_eye44[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B11111110,
         B01111100,
         B00000000},
    L_eye44[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B01111111,
         B00111110,
         B00000000},
    R_eye55[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B11111110,
         B00000000},
    L_eye55[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B01111111,
         B00000000},
    R_eye66[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B11111110,
         B00000000},
    L_eye66[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B01111111,
         B00000000},
    R_eye77[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B11111110,
         B01111100,
         B00000000},
    L_eye77[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B01111111,
         B00111110,
         B00000000},
    R_eye88[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B11000110,
         B11111110,
         B01111100,
         B00000000},
    L_eye88[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B01100011,
         B01111111,
         B00111110,
         B00000000},
    R_eye99[] =
        {B00000000,
         B00000000,
         B00000000,
         B11000110,
         B11000110,
         B11111110,
         B01111100,
         B00000000},
    L_eye99[] =
        {B00000000,
         B00000000,
         B00000000,
         B01100011,
         B01100011,
         B01111111,
         B00111110,
         B00000000},
    R_eye111[] =
        {B00000000,
         B00000000,
         B11111110,
         B11000110,
         B11000110,
         B11111110,
         B01111100,
         B00000000},
    L_eye111[] =
        {B00000000,
         B00000000,
         B01111111,
         B01100011,
         B01100011,
         B01111111,
         B00111110,
         B00000000};

uint8_t mem_instuct[] =
    {B00000000,
     B00000000,
     B00000000,
     B00000000,
     B00000000,
     B00000000,
     B00000000,
     B00000000};

uint8_t wh_eye[] =
    {B00111110,
     B01111111,
     B01111111,
     B01111111,
     B01111111,
     B00111110,
     B00000000,
     B00000000};

uint8_t BL_eye[] =
    {B00000000,
     B00000000,
     B00000000,
     B00000000,
     B00000000,
     B00000000,
     B00000000,
     B00000000};

void init_hardware()
{
  pinMode(relayPin, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);

  digitalWrite(relayPin, relayPinState);

  // serial port initialization
  Serial.begin(115200);
  // Serial1.begin(9600, SERIAL_8N1, rxPin, txPin, false);
  delay(10);
  Serial.println();
  Serial.println("Starting...");

  matrix.begin(0x70);
  matrix.setRotation(2);
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

void stopBlinkEye()
{
  matrix.drawBitmap(0, 8, eye_1l, 8, 8, LED_ON);
  matrix.drawBitmap(0, 0, eye_1r, 8, 8, LED_ON);
  matrix.writeDisplay();
}

uint32_t curTime = 0;
int wink_eye = 0;

void blinkEye()
{
  // Serial.print("time loop = ");
  // Serial.println((millis() - curTime));
  // curTime = millis();

  timenow = millis();
  if (timenow - time1 >= 50)
  {
    matrix.clear();
    time1 = timenow;

    wink_eye++;
    switch (wink_eye)
    {
    case 1: // forward
      matrix.drawBitmap(0, 8, R_eye11, 8, 8, LED_ON);
      matrix.drawBitmap(0, 0, L_eye11, 8, 8, LED_ON);
      break;
    case 2: // backward
      matrix.drawBitmap(0, 8, R_eye22, 8, 8, LED_ON);
      matrix.drawBitmap(0, 0, L_eye22, 8, 8, LED_ON);
      break;
    case 3: // left
      matrix.drawBitmap(0, 8, R_eye33, 8, 8, LED_ON);
      matrix.drawBitmap(0, 0, L_eye33, 8, 8, LED_ON);
      break;
    case 4: // right
      matrix.drawBitmap(0, 8, R_eye44, 8, 8, LED_ON);
      matrix.drawBitmap(0, 0, L_eye44, 8, 8, LED_ON);
      break;
    case 5: // wink
      matrix.drawBitmap(0, 8, R_eye55, 8, 8, LED_ON);
      matrix.drawBitmap(0, 0, L_eye55, 8, 8, LED_ON);
      break;
    case 6: // forward
      matrix.drawBitmap(0, 8, R_eye66, 8, 8, LED_ON);
      matrix.drawBitmap(0, 0, L_eye66, 8, 8, LED_ON);
      break;
    case 7: // backward
      matrix.drawBitmap(0, 8, R_eye77, 8, 8, LED_ON);
      matrix.drawBitmap(0, 0, L_eye77, 8, 8, LED_ON);
      break;
    case 8: // left
      matrix.drawBitmap(0, 8, R_eye88, 8, 8, LED_ON);
      matrix.drawBitmap(0, 0, L_eye88, 8, 8, LED_ON);
      break;
    case 9: // right
      matrix.drawBitmap(0, 8, R_eye99, 8, 8, LED_ON);
      matrix.drawBitmap(0, 0, L_eye99, 8, 8, LED_ON);
      break;
    case 10: // wink
      matrix.drawBitmap(0, 8, R_eye111, 8, 8, LED_ON);
      matrix.drawBitmap(0, 0, L_eye111, 8, 8, LED_ON);
      break;
    default:
      wink_eye = 0;
      statusDisplay = false;
      break;
    }
    matrix.writeDisplay();
  }

  if (wink_eye > 10)
  {
    wink_eye = 0;
    statusDisplay = false;
    matrix.clear();
    stopBlinkEye();
    // delay(1000);
  }
}

void setup()
{
  init_hardware();
  init_wifi();
  init_mqtt();
}

//   //    sync_advpub("prefix", "topic", "payload", "retain")
//   mqtt->sync_advpub("", "/KIDBRIGHT/gearname/KB001", String(btCount), false);
unsigned long previousMillis = 0;
void loop()
{
  if (statusDisplay == false)
  {
    stopBlinkEye();
  }
  if (statusDisplay == true)
  {
    blinkEye();
  }

  mqtt->loop();
} // END LOOP
