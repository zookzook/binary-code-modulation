#include <avr/io.h>
#include <avr/interrupt.h>
#include "globals.h"
#include "animation.h"

volatile byte bit= 128;
volatile RGB  leds[MAX_LEDS];

void setup() {                    

  // define the output function for the pins
  pinMode( LATCH_PIN, OUTPUT );
  pinMode( CLOCK_PIN, OUTPUT );
  pinMode( DATA_PIN,  OUTPUT );
  
  // init the RGB values to 0
  for( int i= 0; i < MAX_LEDS; i++ ) {
    clearRgb( i );
  } // for 
  
  // define the time1 interrupt
  
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

// binary code modulation
ISR(TIMER1_COMPA_vect) {  
    
  // shift to the next bit and time interval
  bit   <<= 1;
  OCR1A <<= 1;
  
  // if we reached the end begin again  
  if( bit == 0 ) {
    OCR1A= DELAY;
    bit  = 1;
  } // if 
    
  // Latch LOW
  PORTB &= ~B00000001;  
    
  int i= MAX_LEDS - 1;
  while( i != -1 ) {
            
    // red 
    if( leds[ i ].red & bit )
        PORTB &= ~B00001000;
    else  
        PORTB |=  B00001000;
    
    // clock high/low
    PORTB |=  B00010000;
    PORTB &= ~B00010000;
    
    // green
    if( leds[ i ].green & bit )
        PORTB &= ~B00001000;
    else  
        PORTB |=  B00001000;
    
    // clock high/low
    PORTB |=  B00010000;
    PORTB &= ~B00010000;
    
    // blue
    if( leds[ i ].blue & bit )
        PORTB &= ~B00001000;
    else  
        PORTB |=  B00001000;

    // clock high/low
    PORTB |=  B00010000;
    PORTB &= ~B00010000;
      		
    i--;
  } // while
        
  // Latch HIGH
  PORTB |= B00000001;    
}

Animation hsvAnimation= { 1, millis(), 0, 50 };

void loop() {       
  loopWithHSV( &hsvAnimation );
}



