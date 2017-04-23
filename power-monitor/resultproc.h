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


void checkResponse(EthernetClient client)
{
  if (client.connected()) {
    failed_connections = 0;
    
    //Read response
    byte maxReads = 20;   //Seconds
    while ((maxReads-- > 0) && client.connected()) {
      delay(500);
      while (client.available()) {
        DEBUG_PRINT((char)client.read());
      }
    }
    DEBUG_PRINTLN();
    delay(10);
    client.flush();
    delay(10);
  }
  else
  {
    failed_connections++;
    dateFailed = now();
    DEBUG_PRINTLN(F("ERROR: Failed Connected"));
  }
}


void send_to_xively()
{
  /* Set the tempature */
  snprintf(fdata, sizeof fdata, "%d,%d.%02d\n", 0, (int)temp, (int)(temp * 100) % 100);

  /* Send the channels that have data in them */
  for (int i2 = 0; i2 < 10; i2++) {
    if (valueCount[i2] > 0) {
      int buflen = strlen(fdata);
      snprintf(fdata + buflen, (sizeof fdata) - buflen, "%d,%d\n%", (i2 + 1), (valueSum[i2] / valueCount[i2]));
    }
  }

  DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F("Submitting data:"));
  DEBUG_PRINTLN(fdata);

  int data_length = strlen(fdata);

  if (client.connect(XIVELY_URL, 80))
  {
    //https://www.arduino.cc/en/Tutorial/GSMExamplesXivelyClient
    client.print(F("PUT /v2/feeds/"));
    client.print(XIVELY_FEEDID);
    client.println(F(".csv HTTP/1.1"));
    client.print(F("Host: "));
    client.println(XIVELY_URL);
    client.print(F("X-ApiKey: "));
    client.println(XIVELY_KEY);
    /*client.print(F("User-Agent: "));
      client.println(USERAGENT);*/
    client.print(F("Content-Length: "));
    client.println(data_length);
    client.println(F("Connection: close"));
    client.println();
    client.println(fdata);

    checkResponse(client);
  }
  else
  {
    failed_connections++;
    dateFailed = now();
    DEBUG_PRINTLN(F("ERROR: Failed Connect"));
  }
  client.stop();
}

void send_to_pvoutput()
{
  //https://www.pvoutput.org/help.html#api-addstatus
  
  //Parameter  Field Required  Format  Unit  Example Since 
  //d Date  Yes yyyymmdd  date  20100830  r1  
  //t Time  Yes hh:mm time  14:12 r1  
  //v1  Energy Generation No1 number  watt hours  10000 r1  
  //v2  Power Generation  No  number  watts 2000  r1  
  //v3  Energy Consumption  No  number  watt hours  10000 r1  
  //v4  Power Consumption No  number  watts 2000  r1  
  //v5  Temperature No  decimal celsius 23.4  r2  
  //v6  Voltage No  decimal volts 210.7 r2  
  //c1  Cumulative Flag No  number  - 1 r1  
  //n Net Flag  No  number  - 1 r2  

  snprintf(fdata, sizeof fdata, "d=%04d%02d%02d&t=%02d:%02d", year(),month(),day(),hour(),minute()); // Date and Time
  int buflen = strlen(fdata);
  snprintf(fdata + buflen, (sizeof fdata) - buflen, "&v2=%d%", (valueSum[PVOUTPUT_SOLARCHANNEL] / valueCount[PVOUTPUT_SOLARCHANNEL])); // Power Generation
  buflen = strlen(fdata);
  snprintf(fdata + buflen, (sizeof fdata) - buflen, "&v4=%d%", (valueSum[0] / valueCount[0])); // Power Consumption
  buflen = strlen(fdata);
  snprintf(fdata + buflen, (sizeof fdata) - buflen, "&v5=%d.%02d", (int)temp, (int)(temp * 100) % 100); // Temperature

  DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F("Submitting data:"));
  DEBUG_PRINTLN(fdata);

  if (client.connect(PVOUTPUT_URL, 80))
  {
    client.print(F("POST /service/r2/addstatus.jsp?"));
    client.print(fdata);
    client.println(F(" HTTP/1.1"));
    client.print(F("Host: "));
    client.println(PVOUTPUT_URL);
    client.print(F("X-Pvoutput-Apikey: "));
    client.println(PVOUTPUT_KEY);
    client.print(F("X-Pvoutput-SystemId: "));
    client.println(PVOUTPUT_SYSTEMID);
    client.println(F("Connection: close"));
    client.println();

    checkResponse(client);
  }
  else
  {
    failed_connections++;
    dateFailed = now();
    DEBUG_PRINTLN(F("ERROR: Failed Connect"));
  }
  client.stop();    
}

void send_to_thingspeak()
{
  /* Set the tempature */
  snprintf(fdata, sizeof fdata, "field1=%d.%02d&", (int)temp, (int)(temp * 100) % 100);

  /* Send the channels that have data in them */
  for (int i2 = 0; i2 < 10; i2++) {
    if (valueCount[i2] > 0) {
      int buflen = strlen(fdata);
      snprintf(fdata + buflen, (sizeof fdata) - buflen, "field%d=%d&", (i2 + 2), (valueSum[i2] / valueCount[i2]));
    }
  }

  DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F("Submitting data:"));
  DEBUG_PRINTLN(fdata);

  int data_length = strlen(fdata);

  if (client.connect(THINGSPEAK_URL, 80))
  {
    client.println(F("POST /update HTTP/1.1"));
    client.print(F("Host: "));
    client.println(THINGSPEAK_URL);
    client.println(F("Connection: close"));
    client.print(F("X-THINGSPEAKAPIKEY: "));
    client.println(THINGSPEAK_KEY);
    client.println(F("Content-Type: application/x-www-form-urlencoded"));
    client.print(F("Content-Length: "));
    client.println(data_length);
    client.println();
    client.println(fdata);

    checkResponse(client);
  }
  else
  {
    failed_connections++;
    dateFailed = now();
    DEBUG_PRINTLN(F("ERROR: Failed Connect"));
  }
  client.stop();    
}


void process_result()
{
  if (millis() < last_connect) last_connect = millis();

  if ((millis() - last_connect) > UPDATE_INTERVAL) {

    if (strlen(THINGSPEAK_KEY) > 0) {
      send_to_thingspeak();
    }

    if (strlen(XIVELY_KEY) > 0) {
      send_to_xively();
    }

    if (strlen(PVOUTPUT_KEY) > 0) {
      send_to_pvoutput();
    }
    
    /* Reset for next run */
    for (int i3 = 0; i3 < 10; i3++) {
      valueSum[i3] = 0;
      valueCount[i3] = 0;
    }

    Ethernet.maintain(); /* Keep DHCP lease active */

    last_connect = millis();
  }
}

#endif
