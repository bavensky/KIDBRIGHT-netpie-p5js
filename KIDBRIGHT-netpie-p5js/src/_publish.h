#include <MqttConnector.h>

extern int relayPinState;
extern MqttConnector* mqtt;
extern int relayPin;
extern char myName[];
extern uint32_t btCount;

static void readSensor(); 

extern String DEVICE_NAME;
extern int PUBLISH_EVERY;

void register_publish_hooks() {
  strcpy(myName, DEVICE_NAME.c_str());
  mqtt->on_prepare_data_once([&](void) {
    Serial.println("initializing sensor...");
  });

  mqtt->on_before_prepare_data([&](void) {
    readSensor();
  });

  mqtt->on_prepare_data([&](JsonObject *root) {
    JsonObject& data = (*root)["d"];
    JsonObject& info = (*root)["info"];
    data["myName"] = myName;
    data["millis"] = millis();
    data["relayState"] = relayPinState;
    data["updateInterval"] = PUBLISH_EVERY;
    // data["x"] = x;
    // data["y"] = y;
    data["btCount"] = btCount;
  }, PUBLISH_EVERY);
  mqtt->on_after_prepare_data([&](JsonObject * root) {
    /**************
      JsonObject& data = (*root)["d"];
      data.remove("version");
      data.remove("subscription");
    **************/
  });
}

static void readSensor() {
  // perform reading sensor 
  Serial.println("Perform reading sensor...");
}
