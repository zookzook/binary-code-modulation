#include <avr/io.h>
#include <avr/interrupt.h>

// Arduino Uno
// Pin 8  <----> ST_CP of 74HC595
// Pin 12 <----> SH_CP of 74HC595
// Pin 11 <----> DS of 74HC595

#define LATCH_PIN  8
#define CLOCK_PIN  12
#define DATA_PIN   11

#define DELAY          2
#define MAX_LEDS       5
#define TIMEINT       50

#define MAX_LEDS   5

typedef struct {
    byte red;
    byte green;
    byte blue;
} RGB;

volatile RGB leds[MAX_LEDS];
volatile byte bit= 128;

void setup() {                    

  pinMode( LATCH_PIN, OUTPUT );
  pinMode( CLOCK_PIN, OUTPUT );
  pinMode( DATA_PIN,  OUTPUT );
  
  for( int i= 0; i < MAX_LEDS; i++ ) {
    leds[i].red  = 0;
    leds[i].green= 0;
    leds[i].blue = 0;
  } // for 
  
    cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
 
    // set compare match register to desired timer count:
    OCR1A = DELAY;
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    // Set CS12 bits for 256 prescaler:
    TCCR1B |= (1 << CS12);
    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);
    // enable global interrupts:
    sei();
}

ISR(TIMER1_COMPA_vect) {  
    
    bit   <<= 1;
    OCR1A <<= 1;
    
    if( bit == 0 ) {
      OCR1A= DELAY;
      bit  = 1;
    } // if 
    
    // Latch LOW
    PORTB &= ~B00000001;  
    
    int i= MAX_LEDS - 1;
    while( i != -1 ) {
            
      if( leds[ i ].red & bit )
          PORTB &= ~B00001000;
      else  
          PORTB |=  B00001000;
    
      PORTB |=  B00010000;
      PORTB &= ~B00010000;
    
      if( leds[ i ].green & bit )
          PORTB &= ~B00001000;
      else  
          PORTB |=  B00001000;
    
      PORTB |=  B00010000;
      PORTB &= ~B00010000;
    
      if( leds[ i ].blue & bit )
          PORTB &= ~B00001000;
      else  
          PORTB |=  B00001000;
    
      PORTB |=  B00010000;
      PORTB &= ~B00010000;
      		
      i--;
    } // while
        
    // Latch HIGH
    PORTB |= B00000001;  
    
}

void setHSV( uint16_t index, uint16_t h, uint16_t s, uint16_t v) {
  
  unsigned long _h= max( 0, min( 360, h));
  unsigned long _s= max( 0, min( 255, s));
  unsigned long _v= max( 0, min( 255, v));
  
  if(_s == 0) {
    leds[index].red = leds[index].green = leds[index].blue = v;
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
      leds[index].red  = _v;
      leds[index].green= t;
      leds[index].blue = p;
      break;
    case 1:
      p  = _v - vs;
      q  = (100L * _v - vfs) / 100L;
      leds[index].red  = q;
      leds[index].green= _v;
      leds[index].blue = p;
      break;
    case 2:
      p  = _v - vs;
      t  = _v - vs + vfs / 100L;    
      leds[index].red  = p;
      leds[index].green= _v;
      leds[index].blue = t;
      break;
    case 3:
      p  = _v - vs;
      q  = (100L * _v - vfs) / 100L;
      leds[index].red  = p;
      leds[index].green= q;
      leds[index].blue = _v;
      break;
    case 4:
      p  = _v - vs;
      q  = (100L * _v - vfs) / 100L;
      t  = _v - vs + vfs / 100L;    
      leds[index].red  = t;
      leds[index].green= p;
      leds[index].blue = _v;
      break;
    default: // case 5:
      p  = _v - vs;
      q  = (100L * _v - vfs) / 100L;
      leds[index].red  = _v;
      leds[index].green= p;
      leds[index].blue = q;
    }
}

int offset= 1;
int index= 0;
unsigned long t= millis();
unsigned long t2= millis();
int s= 0;

unsigned int h[MAX_LEDS]= { 360, 360, 360, 360, 360 };

void loop() {   
    
  if( millis() - t > TIMEINT ) {
    s= s + offset;
    if( s > 360 ) {
       s= 360;
      offset= offset * -1; 
    }
    else
    if( s < 0 ) {
       s= 0;
      offset= offset * -1; 
    }
    t= millis();    
  }

  int _s= s;
  setHSV( 0, _s, 255, 255 ); 
  
  _s= s + 10;
  if( _s > 360 )
    _s= _s -360;
  setHSV( 1, _s, 255, 255 );
  
  _s= s + 20;
  if( _s > 360 )
    _s= _s - 360;
    
  setHSV( 2, _s, 255, 255 ); 

  _s= s + 30;
  if( _s > 360 )
    _s= _s - 360;
    
  setHSV( 3, _s, 255, 255 ); 

  _s= s + 40;
  if( _s > 360 )
    _s= _s - 360;
    
  setHSV( 4, _s, 255, 255 ); 
  
}



