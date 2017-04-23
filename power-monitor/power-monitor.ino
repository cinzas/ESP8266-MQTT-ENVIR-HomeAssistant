#define VERSION "v6.1 - EnergyProxy - http://dotnetdan.info"

#include <SPI.h> //http://playground.arduino.cc/Code/Spi
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <TimeLib.h> //http://playground.arduino.cc/Code/time

#include "auth.h"

//#define _BRIDGE
#define _DEBUG // uncomment this line for extra debug information

/* Initialise serial appropriately */
#define CC_BAUD 57600

#ifdef _BRIDGE
#define CC_SERIAL Serial
#define ETH_PIN 7
#else
#include <SoftwareSerial.h>
#define SERIAL_RX 2
#define SERIAL_TX 3
SoftwareSerial ccSerial(SERIAL_RX, SERIAL_TX);
#define CC_SERIAL ccSerial
#endif

/* The size of the buffer that messages are read onto */
/* (should fit a whole message) */
/* 166-288 (One is 332 and history ones are larger) */
#define BUFFER_SIZE 350

/* Max time to wait between bits of a message (milliseconds) */
#define MSG_DELAY 400

/* ~~~~~~~~~~~~~~~~ */
/* Global Variables */
/* ~~~~~~~~~~~~~~~~ */
int    lastHour;
time_t dateStarted;
time_t dateFailed;

/* Networking details (add your ethernet shield's MAC address) */
byte mac[] = {0x90, 0xA2, 0xDA, 0x02, 0x03, 0xC5};
byte ip[] = {192, 168, 30, 200};
byte gateway[] = {192, 168, 30, 1};
EthernetClient client;
EthernetServer server(80);              // the web server is used to serve status calls
EthernetUDP Udp;

/* Message buffer & its counter */
char buffer[BUFFER_SIZE];
int i = 0;

/* The time of the last read from the serial */
unsigned long t_lastread = 0;

/* Are we waiting for a message? Did a buffer overflow? */
boolean waiting = true;
boolean overflowed = false;

/* Watch the connection quality */
int failed_connections = 0;

/* ~~~~~~~~~~~~ */
/* Program Body */
/* ~~~~~~~~~~~~ */

#ifdef _DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

/* All the real work is in the xml processor */
#include "resultproc.h"
#include "xmlproc.h"
#include "ntp-time.h"
#include "web-client.h"

void setup()
{
  /* Initialise Arduino to CurrentCost meter serial */
  CC_SERIAL.begin(CC_BAUD);

#ifdef _DEBUG
  /* Opens debug serial port */
  Serial.begin(CC_BAUD);
  DEBUG_PRINTLN(F("Starting.."));
#endif

#ifdef _BRIDGE
  pinMode(ETH_PIN, OUTPUT);
  digitalWrite(ETH_PIN, HIGH);
#endif

  /* Connect to the network */
  if (Ethernet.begin(mac) == 0)
  {
    DEBUG_PRINTLN(F("DHCP failed!"));
    Ethernet.begin(mac, ip, gateway, gateway);
  }

  Ethernet.maintain();
  DEBUG_PRINT(F("Local IP: "));
  DEBUG_PRINTLN(Ethernet.localIP());
  
  // initialize time server
  Udp.begin(8888);
  DEBUG_PRINTLN(F("Setting time using NTP"));
  while(!UpdateTime()); // wait until time is set
  DEBUG_PRINTLN(F("Time is set"));
  
  lastHour = hour();
  dateStarted = now();
  dateFailed = now();
}


void loop()
{
  /* Read and send data */
  ReadMeter();
  
  /* Reconnect to the network if neccessary */
  if (failed_connections > 3) {
    DEBUG_PRINT(F("Failed Connections - Reset"));
    failed_connections = 0;
    client.stop();

    if (Ethernet.begin(mac) == 0) {
      DEBUG_PRINTLN(F("DHCP failed!"));
      Ethernet.begin(mac, ip, gateway, gateway);
    }
  }

  /* Update local time/date */
  if(hour()!=lastHour) {
    lastHour=hour();
    if(lastHour==10 || lastHour==22) {
      UpdateTime();
    }
  }

  /* Server Webpage */
  ServeWebClients();
  delay(50);
}


void ReadMeter()
{
  /*
     The incoming message appears on the SoftwareSerial buffer
     in several parts separated by small time intervals.
     Wait for the whole message to arrive before processing
  */
  while ((millis() - t_lastread < MSG_DELAY || waiting) && !overflowed)
  {
    if (CC_SERIAL.available())
    {
      while (CC_SERIAL.available())
      {
        if (i == BUFFER_SIZE) {
          overflowed = true;
          DEBUG_PRINT(F("x"));
          break;
        }

        buffer[i] = (char)CC_SERIAL.read();
        i++;
      }

      if (waiting) {
        waiting = false;
      }

      t_lastread = millis();
    }
  }

  CC_SERIAL.flush(); // Clear any remaining serial?

  /* If the buffer hasn't overflowed, process the message */
  if (!overflowed) {
    //DEBUG_PRINTLN(i); // See how big the payload is (166-288)
    
    /* Process the message */
    for (int j = 0; j < i; j++) {
      process_char(buffer[j]);
    }
    process_result();
  }

  /* Reset */
  i = 0;
  waiting = true;
  overflowed = false;  
}
