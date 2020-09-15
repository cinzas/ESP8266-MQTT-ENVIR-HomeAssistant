#ifndef _RESULTPROC
#define _RESULTPROC

/* Cumulative data, gets averaged on upload */
unsigned long valueSum[10];
unsigned int valueCount[10];

/* working data */
float temp;
int sensor;
unsigned int powerch1;
unsigned int powerch2;
unsigned int powerch3;

#define HTTP_LENGTH 60 /* Maximum length for the HTTP request body */
char fdata[HTTP_LENGTH]; /* The formatted upload string */
unsigned long last_connect;

/* --------------------------- */

void reconnect_mqtt() {
  //while (!mqtt.connected()) {
  if (!mqtt.connected()) {
    DEBUG_PRINTLN();
    DEBUG_PRINT(F("MQTT connecting..."));
    // if (mqtt.connect("EnergyProxy")) {
    if (mqtt.connect("EnergyProxy", MQTT_USER, MQTT_PASSWORD)) {
      DEBUG_PRINTLN(F("connected"));
    } else {
      DEBUG_PRINTLN();
      DEBUG_PRINT(F("failed, rc="));
      DEBUG_PRINT(mqtt.state());
      DEBUG_PRINTLN(F(" try again in 3 seconds"));
      delay(3000);
    }
  }
}

void send_to_mqtt()
{
  if (!mqtt.connected()) {
    reconnect_mqtt();
  }
  
  DEBUG_PRINTLN();
  DEBUG_PRINT(F("[MQTT] PUSH TO MQTT "));
  
  /* Set the tempature */
  char fsensor[15];
  snprintf(fsensor, sizeof fsensor, "%s%d", MQTT_TOPIC, 0);
  snprintf(fdata, sizeof fdata, "%d.%02d", (int)temp, (int)(temp * 100) % 100);
  mqtt.publish(fsensor, fdata);
  DEBUG_PRINT(F("+"));
  
  /* Send the channels that have data in them */
  for (int i2 = 0; i2 < 10; i2++) {
    if (valueCount[i2] > 0) {      
      snprintf(fsensor, sizeof fsensor, "%s%d", MQTT_TOPIC, (i2 + 1));
      snprintf(fdata, sizeof fdata, "%d", (int)(valueSum[i2] / valueCount[i2]));
      mqtt.publish(fsensor, fdata);
	  DEBUG_PRINT(F("+"));
    }
  }
  
  DEBUG_PRINTLN(F(" OK"));
}

void send_to_xively()
{
  /* Set the tempature */
  snprintf(fdata, sizeof fdata, "%d,%d.%02d\n", 0, (int)temp, (int)(temp * 100) % 100);

  /* Send the channels that have data in them */
  for (int i2 = 0; i2 < 10; i2++) {
    if (valueCount[i2] > 0) {
      int buflen = strlen(fdata);
      snprintf(fdata + buflen, (sizeof fdata) - buflen, "%d,%d\n", (i2 + 1), (int)(valueSum[i2] / valueCount[i2]));
    }
  }

  DEBUG_PRINTLN();
  DEBUG_PRINT(F("[HTTP] PUSH TO XIVELY "));
  
  http.begin("http://api.xively.com/v2/feeds/"+String(XIVELY_FEEDID)+".csv");
  http.addHeader(F("Host"), F("pvoutput.org"));
  http.addHeader(F("X-ApiKey"), XIVELY_KEY);
  int resultCode = http.sendRequest("PUT", (uint8_t *)fdata, strlen(fdata));
  if(resultCode == HTTP_CODE_OK) {
    DEBUG_PRINTLN(F("OK"));
  } else {
    failed_connections++;
    dateFailed = now();
    DEBUG_PRINTLN(F("FAILED"));
    DEBUG_PRINTLN(http.errorToString(resultCode).c_str());
  }
  http.end();

  DEBUG_PRINTLN(F("Data:"));
  DEBUG_PRINTLN(fdata);
}

void send_to_pvoutput()
{
  //https://www.pvoutput.org/help.html#api-addstatus
  //Parameter Field       Required  Format  Unit        Example   Since
  //d   Date                Yes   yyyymmdd  date        20100830  r1
  //t   Time                Yes   hh:mm     time        14:12     r1
  //v1  Energy Generation   No    number    watt hours  10000     r1
  //v2  Power Generation    No    number    watts       2000      r1
  //v3  Energy Consumption  No    number    watt hours  10000     r1
  //v4  Power Consumption   No    number    watts       2000      r1
  //v5  Temperature         No    decimal   celsius     23.4      r2
  //v6  Voltage             No    decimal   volts       210.7     r2
  //c1  Cumulative Flag     No    number    -           1         r1
  //n   Net Flag            No    number    -           1         r2

  snprintf(fdata, sizeof fdata, "d=%04d%02d%02d&t=%02d:%02d", year(),month(),day(),hour(),minute()); // Date and Time
  int buflen = strlen(fdata);
  if (valueCount[PVOUTPUT_SOLARCHANNEL] != 0) // Only send Generation if there is data!
  {
    snprintf(fdata + buflen, (sizeof fdata) - buflen, "&v2=%d", (int)(valueSum[PVOUTPUT_SOLARCHANNEL] / valueCount[PVOUTPUT_SOLARCHANNEL])); // Power Generation
    buflen = strlen(fdata);
  }
  snprintf(fdata + buflen, (sizeof fdata) - buflen, "&v4=%d", (int)(valueSum[0] / valueCount[0])); // Power Consumption
  buflen = strlen(fdata);
  snprintf(fdata + buflen, (sizeof fdata) - buflen, "&v5=%d.%02d", (int)temp, (int)(temp * 100) % 100); // Temperature

  DEBUG_PRINTLN();
  DEBUG_PRINT(F("[HTTP] PUSH TO PVOUTPUT "));
  
  http.begin("http://pvoutput.org/service/r2/addstatus.jsp?"+String(fdata)+"&key="+PVOUTPUT_KEY+"&sid="+PVOUTPUT_SYSTEMID); 
  int resultCode = http.GET();
  if(resultCode == HTTP_CODE_OK) {
    DEBUG_PRINTLN(F("OK"));
  } else {
    failed_connections++;
    dateFailed = now();
    DEBUG_PRINTLN(F("FAILED"));
    DEBUG_PRINTLN(http.errorToString(resultCode).c_str());
  }
  http.end();

  DEBUG_PRINTLN(F("Data:"));
  DEBUG_PRINTLN(fdata);
}

void send_to_thingspeak()
{
  /* Set the tempature */
  snprintf(fdata, sizeof fdata, "field1=%d.%02d&", (int)temp, (int)(temp * 100) % 100);

  /* Send the channels that have data in them */
  for (int i2 = 0; i2 < 10; i2++) {
    if (valueCount[i2] > 0) {
      int buflen = strlen(fdata);
      snprintf(fdata + buflen, (sizeof fdata) - buflen, "field%d=%d&", (i2 + 2), (int)(valueSum[i2] / valueCount[i2]));
    }
  }

  DEBUG_PRINTLN();
  DEBUG_PRINT(F("[HTTP] PUSH TO THINKSPEAK "));
  
  http.begin("http://api.thingspeak.com/update");
  http.addHeader(F("Host"), F("api.thingspeak.com"));
  http.addHeader(F("X-THINGSPEAKAPIKEY"), THINGSPEAK_KEY);
  int resultCode = http.POST((uint8_t *)fdata, strlen(fdata));
  if(resultCode == HTTP_CODE_OK) {
    DEBUG_PRINTLN(F("OK"));
  } else {
    failed_connections++;
    dateFailed = now();
    DEBUG_PRINTLN(F("FAILED"));
    DEBUG_PRINTLN(http.errorToString(resultCode).c_str());
  }
  http.end();

  DEBUG_PRINT(F("Data:"));
  DEBUG_PRINTLN(fdata);
}


void process_result()
{
  if (millis() < last_connect) last_connect = millis();

  if ((millis() - last_connect) > UPDATE_INTERVAL) {

    #if MQTT_ENABLE == 1
    send_to_mqtt();
    #endif
    
    #if THINGSPEAK_ENABLE == 1
    send_to_thingspeak();
    #endif

    #if XIVELY_ENABLE == 1
    send_to_xively();
    #endif
    
    #if PVOUTPUT_ENABLE == 1
    send_to_pvoutput();
    #endif

    /* Reset for next run */
    for (int i3 = 0; i3 < 10; i3++) {
      valueSum[i3] = 0;
      valueCount[i3] = 0;
    }

#ifndef _ESP8266
    NETWORK.maintain(); /* Keep DHCP lease active */
#endif

    last_connect = millis();
  }
}

#endif
