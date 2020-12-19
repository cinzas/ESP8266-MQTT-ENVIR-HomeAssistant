/* Add your keys & rename this file to auth.h */

#ifndef _AUTH_DETAILS
#define _AUTH_DETAILS

//#define _CCBRIDGE
#define _ESP8266 // Must not define _DEBUG
//#define _ETHERTEN
//#define _DEBUG // uncomment this line for extra debug information
#define _STATUSLED // This will blink when recieving data (once every 6 seconds for each device)


//#define UPDATE_INTERVAL     60000   // 1 min - interval used to update data, in milliseconds
//#define UPDATE_INTERVAL     150000    // 2.5 min - interval used to update data, in milliseconds
//#define UPDATE_INTERVAL     300000    // 5 min - interval used to update data, in milliseconds
#define UPDATE_INTERVAL 15000 // 15 sec - interval used to update data, in milliseconds for MQTT and Thingspeak
#define UPDATE_INTERVAL_PVOUTPUT     60000 // 60 sec - interval to update data, in milliseconds to PVOutput (1/minute - 60 times/hour)

#define UTC_OFFSET_HOURS      0    // Portugal

#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"

#define OTApassword "CHANGEME" //the password you will need to enter to upload remotely via the ArduinoIDE (OTA)
#define OTAport 8266

#define MQTT_ENABLE 1
#define MQTT_SERVER "YOUR_MQQTT_SERVER_LOCAL_IP" 
#define MQTT_USER "YOUR_MQTT_USER"
#define MQTT_PASSWORD "YOUR_MQTT_PASSWORD"
#define MQTT_TOPIC "envir/"   

//  MQTT topic where values are published e.g. envir
//  envir/temp -> Temperature
//  envir/0/power1   -> Power from channel 1 on sensor 0
//  envir/0/power2   -> Power from channel 2 on sensor 0
//  envir/0/power3   -> Power from channel 2 on sensor 0
//  envir/0/powersum -> Total power on sensor 0
//  envir/1/power1   -> Power from channel 1 on sensor 1
//  envir/1/power2   -> Power from channel 2 on sensor 1
//  envir/1/power3   -> Power from channel 2 on sensor 0
//  envir/1/powersum -> Total power on sensor 1

#define PVOUTPUT_ENABLE 0                 // CHANGE TO 1 TO ENABLE
#define PVOUTPUT_SYSTEMID "XXXXX"         // YOUR XXXX PVOUTPUT SYSTEM ID
#define PVOUTPUT_KEY "PV__OUTPUT_KEY"
#define PVOUTPUT_SOLARCHANNEL 0

#define THINGSPEAK_ENABLE 0
#define THINGSPEAK_KEY "thingspeak_key"

#define XIVELY_ENABLE 0
#define XIVELY_FEEDID "your feed id"
#define XIVELY_KEY ""

#endif 
