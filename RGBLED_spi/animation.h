#ifndef ANIMATION_H
#define ANIMATION_H

#include <Arduino.h>

/**
 * simple animation struct
 */
typedef struct {  
  int           offset;
  unsigned long t;
  int           s;
  unsigned long timeInterval;
} Animation;

/**
 * Loops through an HSV animation from 0 to 360 degrees and back.
 */
void loopWithHSV( Animation* animation );

#endif
