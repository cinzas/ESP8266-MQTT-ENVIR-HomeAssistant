Arduino Power Monitor
=====================

This project consists of two separate programs to ease the monitoring of household electricity usage.

1. A simple C program for Arduino, which parses XML data from a Current Cost power meter & uploads to Xively and Thingspeak.
2. A website that uses the data from Thingspeak to create graphs & calculate energy usage.

## Parts List

1. Current Cost 128 power monitor.  http://www.currentcost.com/product-envi.html I got mine from: http://www.smartnow.com.au/
2. Arduino.  I used an Etherten from Freetronics. http://www.freetronics.com/products/etherten
3. Home made serial cable, RJ45->Arduino. Blue = Ground, Brown = Monitor output/Arduino input
4. Login account at http://www.thingspeak.com
5. This Git repo
7. Ino from http://inotool.org/

## Using a Current Cost Bridge

I attempted to use a hacked Current Cost network bridge as my Arduino. However I am not an expert at this and had mixed results. I got it sending data once but then it appears to stop. No serial or status LED's on the board makes diagnosing troublesome and I ended up giving up and just using my left over Etherten.

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

No extra libraries are required!

The web page uses a bunch of great Javascript libraries, notably Raphael & Raphy Charts for the graphs.

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

Arduino changes and stability by Daniel Hyles.

Website & Thingspeak upload capability by Michael Sproul.

Based on [original code](https://github.com/bleep1/CurrentCostToCosmViaArduino) by Brian Lee (bleep1)