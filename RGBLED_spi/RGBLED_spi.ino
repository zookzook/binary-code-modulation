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
  
  digitalWrite( SCK, LOW );
  digitalWrite( MOSI, LOW );
  
  pinMode( 10, OUTPUT );
  
  // init the RGB values to 0
  for( int i= 0; i < MAX_LEDS; i++ ) {
    clearRgb( i );
  } // for 

  SPCR |= ( (1<<SPE) | (1<<MSTR) | (1<< DORD) ); // enable SPI as master and LSBFIRST
  SPCR &= ~( (1<<SPR1) | (1<<SPR0) );            // clear prescaler bits
  SPSR |= (1<<SPI2X);                            // set prescaler bits

  delay(10);
  
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

  SPDR = 0; // write bogus byte to the SPI, because in the loop there is a receive before send.
    
  int i         = MAX_LEDS - 1;
  byte output   = 0;
  byte outputBit= 1;
  while( i != -1 ) {
            
    if( !(leds[ i ].red & bit) ) {
      output |= outputBit;
    } // if     
    outputBit<<=1;
    // byte is ready to sent throu spi
    if( outputBit == 0 ) {
      while (!(SPSR & _BV(SPIF)));
      SPDR     = output;
      output   = 0;
      outputBit= 1;
    } // if 
    
    if( !(leds[ i ].green & bit) ) {
      output |= outputBit;
    } // if     
    outputBit<<=1;
    // byte is ready to sent throu spi
    if( outputBit == 0 ) {
      while (!(SPSR & _BV(SPIF)));
      SPDR     = output;
      output   = 0;
      outputBit= 1;
    } // if 
    
    if( !(leds[ i ].blue & bit) ) {
      output |= outputBit;
    } // if     
    outputBit<<=1;
    // byte is ready to sent throu spi
    if( outputBit == 0 ) {
      while (!(SPSR & _BV(SPIF)));
      SPDR     = output;
      output   = 0;
      outputBit= 1;
    } // if 
    i--;
  } // while
  
  // some bits to left?
  if( outputBit != 0 ) {
      while (!(SPSR & _BV(SPIF)));
      SPDR= output;    
  }
     
  // wait for the rest
  while (!(SPSR & _BV(SPIF)));
  
  // Latch HIGH
  PORTB |= B00000001;    
}

Animation hsvAnimation= { 1, millis(), 0, 70 };

void loop() {       
  
  loopWithHSV( &hsvAnimation );
  
}



