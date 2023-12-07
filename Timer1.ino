// This is just a sketch that will start Timer 1 with OC1A and OC1B enabled.


#include <avr/io.h>
#include <avr/interrupt.h>

// Useful Macros
#define bitset(byte,nbit)   ((byte) |=  (1<<(nbit)))
#define bitclear(byte,nbit) ((byte) &= ~(1<<(nbit)))
#define bitflip(byte,nbit)  ((byte) ^=  (1<<(nbit)))
#define bitcheck(byte,nbit) ((byte) &   (1<<(nbit)))

// Clock Prescaler
#define CLK_PS 1 // Divide by 1, timer runs at system oscillator speed.

// Variables to hold LED timing
static unsigned long prevLED;
const unsigned int invlLED = 500;

void setup() {
  // set up pins
  DDRB |= 0x20;  // PB5(D13) LED
  PORTB |= 0x20; // Set PB5(D13) high.
  DDRB |= 0x02;  // PB1/OCR1A(D9)
  DDRB |= 0x04;  // PB1/OCR1B(D10)
 
  // Set up Timer1
  // (see ATMEGA 328 datasheet for more details)
  //TCCR1A = 0x80 | 0x30 | 0x02;  // Enable OC1A and OC1B, Clear on compare match, set at BOTTOM.  Fast PWM mode 14, ICR1 is Top
  //TCCR1B = 0x18 | CLK_PS;       // Fast PWM mode 14, ICR1 is Top, set clock prescaler/enable timer.
  
  //TCCR1A = 0x80 | 0x30 | 0x02;  // Enable OC1A and OC1B, Clear on compare match, set at BOTTOM.  PWM, phase correct mode 10, ICR1 is Top
  //TCCR1B = 0x10 | CLK_PS;       //  PWM, phase correct mode 10, ICR1 is Top, set clock prescaler/enable timer.
  
  TCCR1A = 0x80 | 0x30 | 0x00;  // Enable OC1A and OC1B, Clear on compare match, set at BOTTOM.  PWM, phase and freq correct mode 8, ICR1 is Top
  TCCR1B = 0x10 | CLK_PS;       //  PWM, phase and freq correct mode 8, ICR1 is Top, set clock prescaler/enable timer.
  
  // Set TOP to achieve desired frequency
  ICR1 = 0x0140;                // 0x0280 - 25kHz, 0x0320 - 20kHz,  0x042A - 15kHz, 0x0640 - 10kHz when CLK_PS = 1 and mode is 14
  // Set BOTTOM for pulse duration
  OCR1A = 0x0008;               // Set Compare A Value    (0x000F = should be 1 microsecond pulse in mode 14)
  OCR1B = 0x000A;               // Set Compare B Value    (0x000F = should be 1 microsecond pulse in mode 14)

  TCNT1 = 0;                    // reset timer - count from zero  
}  

void loop() {
  // Blink LED at 1Hz, Timer1 runs without futher instruction.
  if ((millis() - prevLED) >= invlLED) {
    prevLED = millis();
    PINB |= 0x20; // Toggle LED
  }
}
