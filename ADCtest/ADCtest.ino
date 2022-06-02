#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
// int state 0b00000;

#define set(A, B) A |= (1 << B)

volatile int sensor_value = 0;
ISR(ADC_vect)
{
  sensor_values = ADCH;
  set(ADCSRA, 6);
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
  setup_sensor(0);
}
void setup_sensor(int reference)
{
  char carry = ADMUX & 0b11100000; // Select ref bits only
  int mux_register = 0b00000111 & reference;
  int mux5 = (reference >> 3) & 1;
  ADMUX = carry | mux_register;
  ADCSRB = (mux5 << 5);
}

int main()
{
  init();
  USBDevice.attach();
  Serial.begin(57600);
  adc_init();
  while (1)
  {
    delay(100);
    Serial.println(String(sensor_value));
  }

  return 0;
}