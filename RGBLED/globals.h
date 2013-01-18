#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include "rgb.h"

// Arduino Uno
// Pin 8  <----> ST_CP of 74HC595
// Pin 12 <----> SH_CP of 74HC595
// Pin 11 <----> DS of 74HC595

#define LATCH_PIN  8
#define CLOCK_PIN  12
#define DATA_PIN   11

// the start time interval for bit 1. For the frequence of 75Hz we have a duration of 0.0133s. The duration
// is divided into 256 intervals. That means an interval is 0.0133s/256 = 0,0000520833333333333s. The Arduino comes
// with 16MHz.
// T = 0,0000000625 (1/16Mhz)
// We use Time1 which is 16bit. Therefore 0,0040959375 = T * 65535. 
// With a prescale of 256 we have a resolution of 0,000016  (1/16Mhz)/256. 0,000016s is the time interval 
// for our the counter tick. 
// 0,000052083 / 0,000016 = 2,25520. So we choose 2 as start value of the comparator register.
// The first interrupt will occure after 0,000032s. And 0,000032s < 0,000052 which is needed to have the frequence of 75Hz. We will have
// 256 time intervals so the complete time will be 0,00816s ( < 0.0133s ) which is sufficient for 75Hz at all.

#define DELAY      2
// max number of RGB LEDs
#define MAX_LEDS   5
// time interval for the animation loop
#define TIMEINT    50
// we have five RGB LEDs
#define MAX_LEDS   5

extern volatile byte bit;
extern volatile RGB  leds[];

#endif

