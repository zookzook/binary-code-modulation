binary-code-modulation
======================

Pulse Width Modulation is definitely the preferred method of dimming an LED with a microcontroller. If you
want to control the color of an RGB-LED then you need a lot of PWM Pins: three for each RGB-LED. The
next step is to use a shift register to address each pin of the RGB LED. To control eight RGBs you need 
8x3 = 24 pins or three shift register with eight output pins.

With a brightness of 256 levels you need to divide the interval into 256 time units to switch the
LED on or off. By using 75Hz to avoid flickering this must be done 19.200 times in one second. As you
can see within the time unit the microcontroller checks for each pin if it must be switch on or off. For
most cases the result is like the previous result, so the work does not change the output. It will
be much better if we know when it is time the toggle the output. And here the BCM comes. Instead using
a cyclical on/off cycle (40% on, 60%off) the bit position of the value tells the time and the on/off state of the cycle.

Check http://www.batsocks.co.uk/readme/art_bcm_1.htm for more information.

The implementation is an example to use the BCM on the arduino with 16 Mhz and the famous 74HC595 shift registers. 
There are a few optimizations:

* Instead of digitalWrite the programm uses direct port manipulation. This is much faster.
* The hsv2rgb implementation avoids float point arithmetic. So the parameters should be in [0..255] instead of [0..1].
* Using SPI instead of bit banging. 

In the main loop there is a little animation for five RGB-LEDs to demonstrate the BCM in action. 

The code
========

The code for BCM lives in `RGBLED.ino`. The other header and code files are used to separate 
the data structure and animation code from the BCM. The `rgb.cpp` contains a hsv to rgb conversion and
the rgb datastructure. The `animation.cpp` file is for the animation of the leds which are in `globals.h`
defined. 

The SPI version is in `RGBLED_spi.ino`. You can grab the preferred version and play with it. The
helper files are nearly the same and only duplicated in order to have two Ardiuno IDE projects which
can be compiled and uploaded separately.

This is not a library. You can grab that code and modify for your own needs. 

Arduni x and x != Uno
=====================

You need to adjust the port bits in the interrupt function.