binary-code-modulation
======================

Pulse Width Modulation is definitely the preferred method of dimming an LED with a microcontroller. If you
want to control the color of an RGB-LED then you need a lot of PWM Pins: three for each RGB-LED. The
next step is to use a shift register to address each pin of the RGB LED. To control eight RGBs you need 
8x3 = 24 pins or three shift register with eight output pins.

With a brightness of 256 levels you need to divide the interval into 256 time units to switch the
LED on or off. By using 75Hz to avoid flickering this must be done 19.200 times in one second. As you
can see within the time unit the microcontroller checks for each pink if it must be switch on or off. For
most cases the result is like the previous result, so the work does not change the output. It will
be mutch better if we know when it is time the toggle the output. And here the BCM comes. Instead using
a cyclical on/off cycle (40% on, 60%off) the value of bit tells the time and the on/off state of the cycle.

Check http://www.batsocks.co.uk/readme/art_bcm_1.htm for more information.

The implementation is an example who to use the BCM on the arduino with 16 Mhz and the famous 74HC595 shift registers. 
There are a few optimizations:

* Instead of digitalWrite the programm uses direct port manipulation. This is much faster.
* The setHSV implementation avoids float point arithmetic. So the parameters should be in [0..255] instead of [0..1].
* Using SPI instead of bit banging. This feature will follow.

In the main loop there is a little animation for five RGB-LEDs to demonstrate 

