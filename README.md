Arduino Power Monitor
=====================

This project consists of firmware to ease the monitoring of household electricity usage.

A simple C program for Arduino, which parses XML data from a Current Cost power meter & uploads to MQTT, PVOutput, Xively and Thingspeak.

This is now used to push data to Home Assistant via MQTT (https://home-assistant.io/)

![alt text](https://github.com/DotNetDann/arduino-power-monitor/raw/master/EnviR.jpg)

## Parts List

1. Current Cost 128 power monitor.  http://www.currentcost.com/product-envi.html I got mine from: (http://www.smartnow.com.au/)
2. NodeMCU.  I got a nodeMCU from ebay. Less than $10. (http://nodemcu.com/index_en.html)
3. Home made serial cable, RJ45->NodeMCU. Serial to D7 and D8 on the NodeMCU. Ensure that you send the 3.3v line to the EnviR
4. This Git repo

![alt text](https://github.com/DotNetDann/arduino-power-monitor/raw/master/EnviR_WithNodeMCU.jpg)

## Using a Etherten

> **Note:**
>The latest version of the code has broken the HTTP submission for this build.

I used an Etherten from Freetronics for years. (http://www.freetronics.com/products/etherten)
But seriously, why would you not get a NodeMCU.


## Using a Current Cost Bridge

> **Note:**
>The latest version of the code has broken the HTTP submission for this build.

I attempted to use a hacked Current Cost network bridge as my Arduino. However I am not an expert at this and had mixed results. I got it sending data once but then it appears to hang. No serial or status LED's on the board makes diagnosing troublesome and I ended up giving up and just using my left over Etherten.

I uploaded code using an AVR programmer (the USBTiny ISP) and avrdude. The necessary settings are in the Makefile, so you can just run `ino build && make burn`.

I roughly followed [these instructions](http://john.crouchley.com/blog/archives/722) to hack the bridge.

To program the bridge board:
* Uncomment "#define _BRIDGE"
* Bridge board - Bridge across jumper 1 to disable the Wiznet W5100 Ethernet chip
* Programmer (USBtinyISP) - ensure power is not supplied from this (https://learn.adafruit.com/usbtinyisp/use-it)
* Tools -> Board -> "Arduino Pro or Pro Mini"
* Tools -> Processor -> "ATmega328 (3.3v, 8 MHz)"
* Sketch -> Upload Using Programmer
* Bridge board - un bridge JP1 when done


## Libraries

SPI - http://playground.arduino.cc/Code/Spi
TimeLib - http://playground.arduino.cc/Code/time


## Building

Add your ThingSpeak API key in the necessary place(s), modify ino.ini to reflect the Arduino you've 
got, and run `ino build && ino upload && ino serial`.

You can also edit `~/.inorc` to set your Ino preferences globally.

## Testing

If you want to test your setup without parsing the data or uploading it to ThingSpeak you can build and run the project in the serial-test directory. Run `ino build && ino upload && ino serial` from the serial-test directory and you'll see the XML output from the meter (hopefully).

## Reference material

* [Current Cost XML specification](http://www.currentcost.com/download/Envi%20XML%20v19%20-%202011-01-11.pdf)
* [Another guy who did something similar](http://mungbean.org/blog/?p=477)
* [Some wicked CC bridge hacking](http://john.crouchley.com/blog/archives/722)

## Authors

Arduino changes, stability, mqtt, pvoutput by Daniel Hyles.

Original which has been massively changed by Michael Sproul.

Based on [original code](https://github.com/bleep1/CurrentCostToCosmViaArduino) by Brian Lee (bleep1)
