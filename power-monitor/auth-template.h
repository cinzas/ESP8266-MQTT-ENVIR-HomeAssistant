/* Add your ThingSpeak write API key & rename this file to auth.h */

#ifndef _AUTH_DETAILS
#define _AUTH_DETAILS

//#define UPDATE_INTERVAL     60000   // 1 min - interval used to update data, in milliseconds
//#define UPDATE_INTERVAL     150000    // 2.5 min - interval used to update data, in milliseconds
#define UPDATE_INTERVAL     300000    // 5 min - interval used to update data, in milliseconds

#define UTC_OFFSET_HOURS      10    // Sydney Australia

#define THINGSPEAK_URL "api.thingspeak.com"
#define THINGSPEAK_KEY ""

#define XIVELY_URL "api.xively.com"
#define XIVELY_FEEDID "your feed id"
#define XIVELY_KEY ""

#define PVOUTPUT_URL "pvoutput.org"
#define PVOUTPUT_SYSTEMID "your system id"
#define PVOUTPUT_KEY ""
#define PVOUTPUT_SOLARCHANNEL 1

#endif
