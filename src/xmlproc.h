#ifndef _XMLPROC
#define _XMLPROC

/* Maximum length for an xml tag or field */
#define XML_BUF 30

#define IN_START_TAG 0
#define IN_END_TAG 10
#define IN_TAG_BODY 20
#define DEFAULT_STATE 30
#define IN_GENERIC_TAG 40

/* Processing variables */
byte state = DEFAULT_STATE;
boolean in_good_tag = false;
char desired_data[XML_BUF];
char current_tag[XML_BUF];
int currentchan;

/* Variables to track the cycling through temp, peak, offpeak */
char temp_tag[] = "tmpr";
char sensor_tag[] = "sensor";
char power_tag[] = "watts";
char hist_tag[] = "hist";
char *desired_tag = temp_tag;

/* --------------------------- */

void change_state(int new_state)
{
  state = new_state;
}

void process_start_tag(char c)
{
  switch (c) {
    case '>':
      if (strcmp(current_tag, hist_tag) == 0) {
        // We want to ignore this line untill \n
        DEBUG_PRINTLN(F("HIST"));
        
        change_state(IN_END_TAG);
        return;
      }
      if (strcmp(current_tag, desired_tag) == 0) {
        in_good_tag = true;
      }
      current_tag[0] = '\0';
      change_state(DEFAULT_STATE);
      break;
    default:
      /* Add the character to the current tag name */
      if (strlen(current_tag) < XML_BUF) {
        strncat(current_tag, &c, 1);
      } else {
        // Tag buffer overflow!
      }
  }
}

void process_end_tag(char c)
{
  switch (c)
  {
    case '>':
      change_state(DEFAULT_STATE);
      break;
    default:
      break;
  }
}

void process_tag_body(char c)
{
  switch (c)
  {
    case '<':
      /* Data reporting, the good bit */
      if (in_good_tag) {
        /* Reset for the next run */
        in_good_tag = false;

        /* Temperature mode */
        if (desired_tag == temp_tag) {
          temp = atof(desired_data);
          desired_tag = sensor_tag;
        }
        /* Sensor mode */
        else if (desired_tag == sensor_tag) {
          sensor = atoi(desired_data);
          desired_tag = power_tag;
          powerch1 = 0;
          powerch2 = 0;
          powerch3 = 0;
          currentchan = 1;
        }
        else if ((desired_tag == power_tag) && (currentchan == 1)) {
          powerch1 = atoi(desired_data);
          currentchan = 2;
        }
        else if ((desired_tag == power_tag) && (currentchan == 2)) {
          powerch2 = atoi(desired_data);
          currentchan = 3;
        }
        else if ((desired_tag == power_tag) && (currentchan == 3)) {
          powerch3 = atoi(desired_data);
          currentchan = 4;
        }

        /* Reset data buffer */
        desired_data[0] = '\0';
      }
      change_state(IN_END_TAG);
      break;

    case '\n':
      if (sensor < 99)
      {
        valueSum[sensor] = valueSum[sensor] + powerch1 + powerch2 + powerch3;
        valueCount[sensor] = valueCount[sensor] + 1;
        
        //#if MQTT_ENABLE == 1
		//send_to_mqtt()
        //#endif
        
        DEBUG_PRINT(F("."));
        
        //DEBUG_PRINT(F("Sensor:"));
        //DEBUG_PRINT(sensor);
        //DEBUG_PRINT(F("="));
        //DEBUG_PRINT(valueSum[sensor]);
        //DEBUG_PRINT(F(" ("));
        //DEBUG_PRINT(valueCount[sensor]);
        //DEBUG_PRINTLN(F(")"));

        /* Reset for next run */
        sensor = 99;
        desired_tag = temp_tag;
      }
      break;

    default:
      if (in_good_tag) {
        strncat(desired_data, &c, 1);
      } else {
        break;
      }
  }
}

void process_generic_tag(char c)
{
  if (c == '<') {
    return;
  }
  if (c == '/') {
    change_state(IN_END_TAG);
  } else {
    change_state(IN_START_TAG);
    process_start_tag(c);
  }
}

void process_default(char c)
{
  switch (c)
  {
    case '\n':
      return;
    case '<':
      change_state(IN_GENERIC_TAG);
      return;
    default:
      change_state(IN_TAG_BODY);
      process_tag_body(c);
  }
}

int process_char(char c)
{
  //DEBUG_PRINT(c); // Print all the serial data we get
  switch (state)
  {
    case IN_START_TAG:
      process_start_tag(c);
      break;
    case IN_END_TAG:
      process_end_tag(c);
      break;
    case IN_TAG_BODY:
      process_tag_body(c);
      break;
    case IN_GENERIC_TAG:
      process_generic_tag(c);
      break;
    case DEFAULT_STATE:
    default:
      process_default(c);
  }
}

#endif
