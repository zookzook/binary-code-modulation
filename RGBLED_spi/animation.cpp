#include "animation.h"
#include "globals.h"

void loopWithHSV( Animation* animation ) {   
    
  if( millis() - animation->t > animation->timeInterval ) {
    animation->s= animation->s + animation->offset;
    if( animation->s > 360 ) {
      animation->s= 0;
    }
    animation->t= millis();    
  }

  for( int i= 0; i < MAX_LEDS; i++ ) {
    int _s= animation->s - i * 10;
    if( _s < 0 )
      _s= _s + 360;
    hsv2rgb( i, _s, 255, 255 );
  }
}

