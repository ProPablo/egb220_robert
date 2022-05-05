// This version has no PID control
// No float calculatuions
#define F_CPU 16000000UL

#define __AVR_ATmega32U4__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "line_detection.h"
#include "robert_link.h"
#include "music.h"

#define toggle(A, B) A ^= (1 << B)
#define set(A, B) A |= (1 << B)
#define clr(A, B) A &= ~(1 << B)

// Not a 1:1 mapping to ms
volatile int counter = 0;
volatile int slowCounter = 0;
volatile int idleCounter = 0;
char debounceMask = 0b00111111;
char emptyMask = 0x0;
volatile char debounceState = 0b00000000;
volatile char modeSwitchDebounceState = 0x0;



enum CounterState
{
  INCR_COUNTER,
  DECR_COUNTER,
  SLOW_COUNTER,
  IDLE_COUNTER
};

CounterState counter_state = IDLE_COUNTER;

enum Mode
{
  DEBUG_MODE,
  LINE_DETECTION_MODE,
  SERIAL_SETTINGS_MODE,
  MEME_MODE,
  // Possibly add new/ seperate mapping mode and replace line detection with follow mapping
};

Mode robert_mode = SERIAL_SETTINGS_MODE;

int counter_init()
{
  // //Timer setup
  TCCR3A = 0;
  TCCR3B = 0x05; // 1024
                 // Setup CTC ()
  TCCR3B |= (1 << WGM12);
  TCNT3 = 0;
  // OCR3A = 0xff;
  OCR3A = 0x10; // TOP = 16 (T = 0.001) (1 ms) (1/((16e6/1024)/16)

  // OCR3AH = 0x44; // TOP = 17408// T = 1.1
  // OCR3AL = 0x00;
  // TIMSK3 = (1 << OCIE2A);// interupt on Output compare match A
  TIMSK3 = 0x02;
}

#define DEBUG_MULTIPLIER 3
#define SLOW_COUNTER_MAX 400
int counter_state_machine()
{
  switch (counter_state)
  {
  case IDLE_COUNTER:
    if (debounceState == debounceMask)
    {
      counter_state = INCR_COUNTER;
      Serial.println("Changing to Increment State");
    }
    break;

  case INCR_COUNTER:
    if (debounceState == emptyMask)
    {
      String toPrint = String("Changing to Slow State") + counter;
      Serial.println(toPrint);
      counter_state = SLOW_COUNTER;
      slowCounter = SLOW_COUNTER_MAX;
    }
    break;
  // case SLOW_COUNTER:
  //   if (slowCounter <= 0)
  //   {
  //     Serial.println("Changing to DECR state");
  //     slowCounter = 0;
  //     counter_state = DECR_COUNTER;
  //     counter *= DEBUG_MULTIPLIER;
  //     TCCR0B = timer0BOn;
  //     TCCR0A = timer0AOn;
  //   }
  //   break;
  case DECR_COUNTER:
    if (counter <= 0)
    {
      counter_state = IDLE_COUNTER;
      counter = 0;
      TCCR0B = timerOff;
      // Not turning off A register leads to bug where the OUTPUT register is always on even if you change OCR
      TCCR0A = timerOff;
      TCNT0 = 0;
      Serial.println("Changing to idle state");
    }
    break;
  }
}

char debounce_prev = 0;
int main_state_machine()
{
  int pb_pressed = modeSwitchDebounceState == debounceMask; // if not previously pressed and pressed now
  int is_justPressed = pb_pressed && !debounce_prev;
  debounce_prev = pb_pressed;

  switch (robert_mode)
  {

  case DEBUG_MODE:
    if (is_justPressed)
    {
      robert_mode = LINE_DETECTION_MODE;
      slowCounter = SLOW_COUNTER_MAX;

      clr(PORTB, 2);
    }
    break;

  case LINE_DETECTION_MODE:
    if (is_justPressed)
    {
      robert_mode = DEBUG_MODE;
      set(PORTB, 2);
    }
    while(slowCounter >0) {}
    slowCounter = -1;

    break;
    // case SERIAL_SETTINGS_MODE:
    //   if (is_justPressed)
    //   {
    //     robert_mode = DEBUG_MODE;
    //     set(PORTB, 2);
    //   }
    //   if (Serial.available() != 0)
    //   {
    //     String read_response = Serial.readString();
    //   }
    //   break;
  }
}

ISR(TIMER3_COMPA_vect) // USE COMPA INSTEAD OF OVF WHICH STANDS FOR OVERFLOW
{
  debounceState = ((debounceState << 1) & debounceMask) | (PINC >> 7 & 1);
  modeSwitchDebounceState = ((modeSwitchDebounceState << 1) & debounceMask) | (PINC >> 6 & 1);
  switch (counter_state)
  {
  case INCR_COUNTER:
    counter++;
    break;
  case DECR_COUNTER:
    counter--;
    break;
  case SLOW_COUNTER:
    slowCounter--;
    break;
  }
}

ISR(BADISR_vect)
{
  // set(PORTB, 1);
}

int main()
{
  //########################### Arduino initialisation functions ###########################
  init();
  USBDevice.attach();

  set(DDRB, 1);
  // For portc switch 1
  clr(DDRC, 7);
  // SW0
  clr(DDRC, 6);

  // LED3
  set(DDRB, 2);

  cli();
  motor_init();
  counter_init();
  speaker_init();
  adc_setup();
  //########################### USBCON = 0 not needed if using USBDevice.attach(); ###########################
  // USBCON = 0;     //Turn off USB peripheral, including interrupt
  sei(); // Enable global interrupts
  Serial.begin(57600);

  // clr(PORTB, 1);
  //########################### use while(!Serial); to pause code untill serial monontor opened ###########################
  // if you add this to you code and then disconect from you computer your code will not run past here.
  // while (!Serial)
  //   ;

  while (1)
  {
    //########################### serial print as required ###########################
    counter_state_machine();
    main_state_machine();

    // music_play();
    sensor_tick();

    // debug_print_sensors();
    if (serialEventRun)
      serialEventRun();
  }
  return 0;
}
