#include <Arduino.h> 

/* BOARD INFO */
String DEVICE_NAME      = "KB001"; 

/* WIFI INFO */ 
String WIFI_SSID        = "ampere";
String WIFI_PASSWORD    = "espertap";

/* MQTT INFO */ 
String MQTT_HOST        = "gb.netpie.io";
String MQTT_USERNAME    = "CfiQvoEctV9uk92";
String MQTT_PASSWORD    = "YHMys6WMI8lsksN5eBT1vxMnQFs=";
String MQTT_CLIENT_ID   = "BwLyt0Qs33aaTQle";
String MQTT_PREFIX      = "/devSuperman/gearname/";
int    MQTT_PORT        = 1883;
int PUBLISH_EVERY       = 5L * 1000;
int MQTT_CONNECT_TIMEOUT= 120; 


/*
* board
*   mosquitto_sub -t "/devSuperman/gearname/#" -h gb.netpie.io -i BwLyt0Qs33aaTQle -u "CfiQvoEctV9uk92" -P "YHMys6WMI8lsksN5eBT1vxMnQFs=" -p 1883 -d
*/