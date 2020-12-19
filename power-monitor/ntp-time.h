const int NTP_PACKET_SIZE= 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets 

// send an NTP request to the time server at the given address 
void sendNTPpacket()
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE); 
  // Initialize values needed to form NTP request
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49; 
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:        
  Udp.beginPacket("ntp02.oal.ul.pt", 123); //NTP requests are to port 123
  Udp.write(packetBuffer,NTP_PACKET_SIZE);
  Udp.endPacket(); 
}

unsigned long getNtpTime()
{
    while(Udp.parsePacket()) Udp.flush(); // make sure udp buffer is empty
    sendNTPpacket();
    delay(1000);
    if(Udp.parsePacket())
    {
        Udp.read(packetBuffer,NTP_PACKET_SIZE);  // read the packet into the buffer
        // the timestamp starts at byte 40 of the received packet and is four bytes,
        // or two words, long. First, esxtract the two words:
        unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
        unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  
        // combine the four bytes (two words) into a long integer
        // this is NTP time (seconds since Jan 1 1900):
        unsigned long secsSince1900 = highWord << 16 | lowWord;
        unsigned long now = secsSince1900 - 2208988800UL;  // GMT
        
        // DST == DaySavingTime == Zomertijd
        boolean dst = false;
        int m = month(now);
        dst = !(m < 3 || m > 10); // between october and march
        if (dst) 
        {
            if (m == 3) 
            {
                //  starts last sunday of march
                dst = (day(now) >= ((31 - (5 * year(now) /4 + 5) % 7)));
            } 
            else if (m== 10) 
            {
                //last sunday of october
                dst = (day(now) < ((31 - (5 * year(now) /4 + 2) % 7)));
            }
            now +=(dst?3600:0);
        }
        
        now -= (3600L*UTC_OFFSET_HOURS); // TimeZone adjustment for Northern Hemisphere
        Udp.flush();
        return now;
    }
    return 0; // return 0 if unable to get the time
}

bool UpdateTime()
{
  unsigned long newTime = getNtpTime();
  if (newTime>0) {
    setTime(newTime);
    return true;
  }
  return false;
}
