#include <Arduino.h>
#include <MqttConnector.h>

extern MqttConnector *mqtt;

extern String MQTT_CLIENT_ID;
extern String MQTT_PREFIX;
extern bool statusDisplay;
extern int relayPin;
extern int relayPinState;
extern char myName[];
extern uint32_t direction;

extern int countEye;

void register_receive_hooks()
{
  mqtt->on_subscribe([&](MQTT::Subscribe *sub) -> void {
    Serial.printf("myName = %s \r\n", myName);
    // sub->add_topic(MQTT_PREFIX + myName + "/$/+");
    // sub->add_topic(MQTT_PREFIX + MQTT_CLIENT_ID + "/$/+");
    sub->add_topic("/nectecAce2018/gearname/info/power");
  });

  mqtt->on_before_message_arrived_once([&](void) {});

  mqtt->on_message([&](const MQTT::Publish &pub) {});

  mqtt->on_after_message_arrived([&](String topic, String cmd, String payload) {
    Serial.printf("topic: %s\r\n", topic.c_str());
    Serial.printf("cmd: %s\r\n", cmd.c_str());
    Serial.printf("payload: %s\r\n", payload.c_str());

    

    if (cmd == "power")
    {
      statusDisplay = true;
      // countEye = payload.toInt();
      // Serial.print("countEye = ");
      // Serial.println(countEye);
      // Serial1.print(String(direction) + "\n");

      // if (payload)
      // {
      //   statusDisplay = true;
      // }
      // else if (!payload == "STOP")
      // {
      //   statusDisplay = false;
      // }

      // if (payload == "ON")
      // {
      //   digitalWrite(relayPin, HIGH);
      //   digitalWrite(LED_BUILTIN, LOW);
      //   relayPinState = HIGH;
      // }
      // else if (payload == "OFF")
      // {
      //   digitalWrite(relayPin, LOW);
      //   digitalWrite(LED_BUILTIN, HIGH);
      //   relayPinState = LOW;
      // }
    }
    else if (cmd == "$/reboot")
    {
      ESP.restart();
    }
    else
    {
      // another message.
    }
  });
}
