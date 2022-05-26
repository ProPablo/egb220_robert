#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
// int state 0b00000;

#define set(A, B) A |= (1 << B)

ISR(ADC_vect)
{
}

ISR(BADISR_vect)
{

  set(PORTB, 0);
}

void adc_init()
{

  // Vcc ref voltage with Left adjusted
  ADMUX |= (1 << 6);
  // left adjusted means you only get 8 bits of precision and only use ADCH
  ADMUX |= (1 << 5);
  // Enable adc (without auto trigger)
  ADCSRA |= (1 << 7) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | 1;
  // ADCSRA |= (1 << 7) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | 1;
  ADCSRB = 0;
  // Start adc (done last)
  ADCSRA |= (1 << 6);
}

int main()
{
  init();
  USBDevice.attach();
  Serial.begin(57600);
  while (1)
  {
  }

  return 0;
}