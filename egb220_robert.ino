// This version has no PID control
// No float calculatuions
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "line_detection.h"
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

int overall_motor_divisor = 1;

char timer0AOn = 0x0;
char timer0BOn = 0x0;
char timerOff = 0x0;

#pragma region Configurable variables
//Use dictionary for help message, and enum relating to where it is stored in an array
int MAX_MOTOR_SPEED = 100;

#pragma endregion

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
  SERIAL_SETTINGS_MODE,
  MEME_MODE,
  DEBUG_MODE,
  LINE_DETECTION_MODE
  // Possibly add new/ seperate mapping mode and replace line detection with follow mapping
};

Mode robert_mode = SERIAL_SETTINGS_MODE;

int motor_init()
{

  set(DDRB, 7); // PWM pin set to output
  set(DDRD, 0); // PWM pin set to output

  DDRE |= (1 << 6); // Motor B Direction
  DDRB |= (1 << 0); // Motor A Direction

  TCCR0A |= (1 << 7) | (1 << 5) | (1 << 1) | (1 << 0); // Clear on compare, fast PWM mode
  timer0AOn = TCCR0A;

  //########################### Serial print works with a clock prescaler of 64 ###########################
  TCCR0B |= (1 << 1) | (1 << 0); // Select Clock (64), Turn timer on //Min f = 250000hz, TOP f @ 256 =1/(256/(16e6/64)) = 1000hz
  timer0BOn = TCCR0B;

  TCCR0A = timerOff;
  TCCR0B = timerOff;

  //########################### Serial print works with a clock prescaler of 64 ###########################

  // TIMSK0 |= (1 << 0); // Enable timer overflow interrupt
  OCR0A = 254;
  OCR0B = 254;

  return 0;
}

int adc_setup()
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

#define PRESCALER_1 0x19
#define PRESCALER_8 0x1A
#define PRESCALER_64 0x1B
#define PRESCALAR_256 0x1C
#define PRESCALAR_1024 0x1D
int speaker_init()
{

  TCCR1A = 0x33; // fast PWM, clear on CMP, TOP in OCR1A
  // Mode 15 on datasheet
  TCCR1B = PRESCALER_8;
  // TOP = 500  (1/4e3)/(1/(16e6/8))
  // OCR1A = 500;
  DDRB |= (1 << PINB6);
  OCR1AH = 0x01;
  OCR1AL = 0xf4;
  OCR1B = 250;
  TIMSK1 = 0x01; // Enable timer overflow ISR
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
  case SLOW_COUNTER:
    if (slowCounter <= 0)
    {
      Serial.println("Changing to DECR state");
      slowCounter = 0;
      counter_state = DECR_COUNTER;
      counter *= DEBUG_MULTIPLIER;
      TCCR0B = timer0BOn;
      TCCR0A = timer0AOn;
    }
    break;
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
  case SERIAL_SETTINGS_MODE:
    if (is_justPressed)
    {
      robert_mode = DEBUG_MODE;
      set(PORTB, 2);
    }
    if (Serial.available() != 0) {
      String read_response = Serial.readString();
    }
    break;

  case DEBUG_MODE:
    if (is_justPressed)
    {
      robert_mode = SERIAL_SETTINGS_MODE;
      clr(PORTB, 2);
    }
    break;
  }
}

int SerialHelpMessage() {
  Serial.println("Welcome to Robert uwu...."); 
  Serial.println("adjust max speed: m <float from 0 to 1>"); 
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

// Mapping ADC registers to Switch positions
// Slightly incorrect first one is wrong but all are recognised(just in wrong order)
// Sensor line_sensors[8] = {
//     {8, -4},  // S8
//     {9, -3},  // S7
//     {10, -2}, // S6
//     {11, -1}, // S5
//     {7, 1},   // S4
//     {6, 2},   // S3
//     {5, 3},   // S2
//     {4, 4}    // S1
// };

// Left to right sensors
Sensor line_sensors[8] = {
    {9, -4},  // S8
    {10, -3}, // S7
    {11, -2}, // S6
    {7, -1},  // S5
    {6, 1},   // S4
    {5, 2},   // S3
    {4, 3},   // S2
    {8, 4}    // S1
};
#define THRESHOLD 215
#define DETECTLOWER 50
int current_sensor = 0;

int sensor_values[8] = {0, 0, 0, 0, 0, 0, 0, 0};

#define SENSOR_HEURISTIC_MAX 250
// Might need to slow down the larger the hueristic is so that
//  #define TURN_SLOWING_FACTOR
// int sensor_hueristic = 0;

// Happens at guaranteed interval when ADC is done with all sensor (place motor logic in here)
int sensor_tick()
{
  // if negative set left motor 2 points lower to move left (keep right at max)

  // int normalized_huerstic = ((sensor_hueristic * 255) / overall_motor_divisor) / SENSOR_HEURISTIC_MAX; // Value from 0 to 255

  int totalhueristic = 0;

  for (int i = 0; i < 4; i++)
  {
    totalhueristic -= sensor_values[i];
  }
  for (int i = 4; i < 8; i++)
  {
    totalhueristic += sensor_values[i];
  }

  if (totalhueristic < 0)
  {
    OCR0B = MAX_MOTOR_SPEED - DETECTLOWER;
    OCR0A = MAX_MOTOR_SPEED;
  }
  else if (totalhueristic > 0)
  {

    OCR0A = MAX_MOTOR_SPEED - DETECTLOWER;
    OCR0B = MAX_MOTOR_SPEED;
  }
  else
  {
    OCR0B = MAX_MOTOR_SPEED;
    OCR0A = MAX_MOTOR_SPEED;
  }

  // if (sensor_values[1] < THRESHOLD && sensor_values[6] > THRESHOLD)
  // {
  //   OCR0A = MAX_MOTOR_SPEED - DETECTLOWER;
  //   OCR0B = MAX_MOTOR_SPEED;
  // }
  // else if (sensor_values[6] < THRESHOLD && sensor_values[1] > THRESHOLD)
  // {

  //   OCR0B = MAX_MOTOR_SPEED - DETECTLOWER;
  //   OCR0A = MAX_MOTOR_SPEED;
  // }
  // else
  // {
  //   OCR0A = MAX_MOTOR_SPEED;
  //   OCR0B = MAX_MOTOR_SPEED;
  // }
  // delay(100);
}

ISR(ADC_vect)
{
  // Store all sensor values and run done compute
  char carry = ADMUX & 0b11100000; // Select ref bits only
  // char  = 0;
  sensor_values[current_sensor] = ADCH;
  int current_reference = line_sensors[current_sensor].reference;
  int mux_register = 0b00000111 & current_reference;
  int mux5 = (current_reference >> 3) & 1;
  ADMUX = carry | mux_register;
  ADCSRB = (mux5 << 5);
  current_sensor = (current_sensor + 1) % 8;
  set(ADCSRA, 6);
}

ISR(BADISR_vect)
{
  // set(PORTB, 1);
}

char *binString(unsigned short n)
{
  static char bin[17];
  int x;

  for (x = 0; x < 16; x++)
  {
    bin[x] = n & 0x8000 ? '1' : '0';
    n <<= 1;
  }
  bin[16] = '\0';

  return (bin);
}

void debug_print_sensors()
{
  _delay_ms(200);
  // String sensorString = "Here are sensors:";
  for (int i = 0; i < 8; i++)
  {
    // sensorString += String(sensor_values[i]) += " , ";
    Serial.print(sensor_values[i]);
    Serial.print(" , ");
  }
  Serial.println("Finished line");
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
