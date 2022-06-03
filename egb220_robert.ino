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

// Bad practise but other files can access these macros since they are in main
#define toggle(A, B) A ^= (1 << B)
#define set(A, B) A |= (1 << B)
#define clr(A, B) A &= ~(1 << B)

// #define NO_DEBUG_MODE

// Not a 1:1 mapping to ms
volatile int counter = 0;
volatile int slowCounter = 0;
volatile int idleCounter = 0;
volatile unsigned long globalCounter = 0;
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
  TRANSITION_MODE,
  SERIAL_SETTINGS_MODE,
  MEME_MODE,

  // Possibly add new/ seperate mapping mode and replace line detection with follow mapping
};

Mode robert_mode = DEBUG_MODE;

int counter_init()
{
  // //Timer setup
  TCCR3A = 0;
  // TCCR3B = 0x05; // 1024
  TCCR3B = 0x02; // 8

  // Setup CTC ()
  TCCR3B |= (1 << WGM12);
  TCNT3 = 0;
  OCR3A = 0xff;
  // OCR3A = 0x10; // TOP = 16 (T = 0.001024) (1 ms) (1/((16e6/1024)/16)

  OCR3AH = 0x07; // TOP = 2000 with 8 prescalar T = 0.001
  OCR3AL = 0xD0;

  // OCR3AH = 0x44; // TOP = 17408// T = 1.1
  // OCR3AL = 0x00;
  // TIMSK3 = (1 << OCIE2A);// interupt on Output compare match A
  TIMSK3 = 0x02;
}

ISR(TIMER3_COMPA_vect) // USE COMPA INSTEAD OF OVF WHICH STANDS FOR OVERFLOW
{
  globalCounter++;
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
  }
  if (slowCounter > 0)
  {
    slowCounter--;
  }
  // Possibly better to just use flag method (floats in ISR)
  if (globalCounter % SENSOR_TICK_DT_MS == 0)
  {
    sensor_tick();
  }
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
      int counter_saved = counter;
      String toPrint = String("Changing to Slow State") + counter_saved;
      Serial.println(toPrint);
      counter_state = SLOW_COUNTER;
      slowCounter = SLOW_COUNTER_MAX;
    }
    break;
  case SLOW_COUNTER:
    if (slowCounter <= 0)
    {
      Serial.println("Changing to DECR state");
      slowCounter = 0;
      counter_state = DECR_COUNTER;
      counter *= DEBUG_MULTIPLIER;
      start_motors();
    }
    break;
  case DECR_COUNTER:
    if (counter <= 0)
    {
      counter_state = IDLE_COUNTER;
      counter = 0;
      stop_motors();
      Serial.println("Changing to idle state");
    }
    break;
  }
}

void OnEnterLineDetect()
{
  Serial.println("Changing to TRANSITION state");
  robert_mode = TRANSITION_MODE;
  slowCounter = SLOW_COUNTER_MAX;
  reset_PID();
  // set(PORTB, 2);
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
      OnEnterLineDetect();
    }
    counter_state_machine();
    break;
  case TRANSITION_MODE:

    if (slowCounter <= 0)
    {
      Serial.println("Changing to LINE DETECTION state");
      slowCounter = 0;
      start_motors();
      robert_mode = LINE_DETECTION_MODE;
    }
    break;
  case LINE_DETECTION_MODE:
    if (is_justPressed)
    {
      Serial.println("Changing to Serial State");
      stop_motors();
      set(PORTB, 2);
      robert_mode = SERIAL_SETTINGS_MODE;
      SerialIntroMessage();
    }
    break;

  case SERIAL_SETTINGS_MODE:
    if (is_justPressed)
    {
      robert_mode = DEBUG_MODE;
      Serial.println("Changing to DBEUG State");
      clr(PORTB, 2);
      return;
    }
    if (acceptSerialInput())
    {
      robert_mode = DEBUG_MODE;
      Serial.println("Changing to DBEUG State");
      clr(PORTB, 2);
    };
    break;
  }
}

ISR(BADISR_vect)
{
  // toggle(PORTB, 1);
}

int main()
{
  //########################### Arduino initialisation functions ###########################
  init();
  USBDevice.attach();
  // For portc switch 1
  clr(DDRC, 7);
  // SW0
  clr(DDRC, 6);

  // LED0
  set(DDRE, 6);

  // LED2
  set(DDRB, 1);
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

  // while (!Serial)
  //   ;
  // unsigned long myTime;

  while (1)
  {
    main_state_machine();
    // music_play();
    // These loops may last longer than one ISR
    // if (globalCounter % SENSOR_TICK_DT_MS == 0)
    // {
    //   sensor_tick();
    // }
    // debug_print_sensors();
    // if (serialEventRun)
    //   serialEventRun();
  }
  return 0;
}
