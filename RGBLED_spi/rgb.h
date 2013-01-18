#ifndef RGB_H
#define RGB_H

#include <Arduino.h>

typedef struct {
  byte red;
  byte green;
  byte blue;
} RGB;

void hsv2rgb( uint16_t i, uint16_t h, uint16_t s, uint16_t v);
void clearRgb( uint16_t i );

#endif
