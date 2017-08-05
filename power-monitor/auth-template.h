/* Add your keys & rename this file to auth.h */

#ifndef _AUTH_DETAILS
#define _AUTH_DETAILS

//#define _CCBRIDGE
#define _ESP8266 // Must not define _DEBUG
//#define _ETHERTEN
#define _DEBUG // uncomment this line for extra debug information


//#define UPDATE_INTERVAL     60000   // 1 min - interval used to update data, in milliseconds
//#define UPDATE_INTERVAL     150000    // 2.5 min - interval used to update data, in milliseconds
#define UPDATE_INTERVAL     300000    // 5 min - interval used to update data, in milliseconds

#define UTC_OFFSET_HOURS      10    // Sydney Australia

#define WIFI_SSID "ssid"
#define WIFI_PASSWORD "password"


#define MQTT_ENABLE 0
#define MQTT_SERVER "mqtt server ip"
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_TOPIC "envir/power/" // MQTT topic where values are published

#define PVOUTPUT_ENABLE 0
//#define PVOUTPUT_URL "pvoutput.org"
#define PVOUTPUT_SYSTEMID "your system id"
#define PVOUTPUT_KEY ""
#define PVOUTPUT_SOLARCHANNEL 1

#define THINGSPEAK_ENABLE 0
//#define THINGSPEAK_URL "api.thingspeak.com"
#define THINGSPEAK_KEY ""

#define XIVELY_ENABLE 0
//#define XIVELY_URL "api.xively.com"
#define XIVELY_FEEDID "your feed id"
#define XIVELY_KEY ""

#endif
