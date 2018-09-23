#include <Arduino.h> 

/* BOARD INFO */
String DEVICE_NAME      = "KB_EYE"; 

/* WIFI INFO */ 
String WIFI_SSID        = "cmmcWIFI";
String WIFI_PASSWORD    = "espertap";

/* MQTT INFO */ 
String MQTT_HOST        = "gb.netpie.io";
String MQTT_USERNAME    = "1JGmU67tQSxQTK6";
String MQTT_PASSWORD    = "XOeQfxKgKbv2yiEGe0c2sBEJtqg=";
String MQTT_CLIENT_ID   = "SSKMIgWsICcMNeeN";
String MQTT_PREFIX      = "/nectecAce2018/gearname/";
int    MQTT_PORT        = 1883;
int PUBLISH_EVERY       = 1L * 1000;
int MQTT_CONNECT_TIMEOUT= 120; 


/*
* nectecAce2018
*   mosquitto_sub -t "/nectecAce2018/gearname/#" -h gb.netpie.io -i SSKMIgWsICcMNeeN -u "1JGmU67tQSxQTK6" -P "XOeQfxKgKbv2yiEGe0c2sBEJtqg=" -p 1883 -d
*/