#include "rgb.h"
#include "globals.h"

/**
 * hsv to rgb without floating point arithmentic...
 */
void hsv2rgb( uint16_t idx, uint16_t h, uint16_t s, uint16_t v) {
  
  unsigned long _h= max( 0, min( 360, h));
  unsigned long _s= max( 0, min( 255, s));
  unsigned long _v= max( 0, min( 255, v));
  
  if(_s == 0) {
    leds[idx].red = leds[idx].green = leds[idx].blue = v;
    return;
  }
 
  unsigned long i  = _h / 60L;
  unsigned long f  = ((_h - 60L * i) * 10L) / 6L;
  unsigned long vs = (_v * _s) / 255L;
  unsigned long vfs= (_v * _s * f) / 255L;
  unsigned long p;
  unsigned long q;
  unsigned long t;

  switch(i) {
    case 6:
    case 0:
      p  = _v - vs;
      t  = _v - vs + vfs / 100L;    
      leds[idx].red  = _v;
      leds[idx].green= t;
      leds[idx].blue = p;
      break;
    case 1:
      p  = _v - vs;
      q  = (100L * _v - vfs) / 100L;
      leds[idx].red  = q;
      leds[idx].green= _v;
      leds[idx].blue = p;
      break;
    case 2:
      p  = _v - vs;
      t  = _v - vs + vfs / 100L;    
      leds[idx].red  = p;
      leds[idx].green= _v;
      leds[idx].blue = t;
      break;
    case 3:
      p  = _v - vs;
      q  = (100L * _v - vfs) / 100L;
      leds[idx].red  = p;
      leds[idx].green= q;
      leds[idx].blue = _v;
      break;
    case 4:
      p  = _v - vs;
      q  = (100L * _v - vfs) / 100L;
      t  = _v - vs + vfs / 100L;    
      leds[idx].red  = t;
      leds[idx].green= p;
      leds[idx].blue = _v;
      break;
    default: // case 5:
      p  = _v - vs;
      q  = (100L * _v - vfs) / 100L;
      leds[idx].red  = _v;
      leds[idx].green= p;
      leds[idx].blue = q;
    }
}

/**
 * set all values to 0
 */
void clearRgb( uint16_t idx ) { 
  leds[idx].red  = 0;
  leds[idx].green= 0;
  leds[idx].blue = 0; 
}
