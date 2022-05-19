#include "line_detection.h"
#include <avr/io.h>

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
int speed_penalty = 50;
int motor_speed = 100;
int current_sensor = 0;

extern volatile unsigned long globalCounter;
// PID
float Kp = 1;    // P gain for PID control
float Ki = 0.5;  // I gain for PID control
float Kd = 0.01; // D gain for PID control

// initialize e_i, e_d
float cum_hueristic = 0;  // integral
float last_heuristic = 0; // For derivatice

int sensor_values[8] = {0, 0, 0, 0, 0, 0, 0, 0};

char timer0AOn = 0x0;
char timer0BOn = 0x0;
char timerOff = 0x0;

void sensor_tick()
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

    String toPrint = String("Done single sensor loop") + globalCounter;
    Serial.println(toPrint);
    bang_bang_controller(totalhueristic);
    // millis();
}

void bang_bang_controller(int heuristic)
{
    if (heuristic < 0)
    {
        OCR0B = motor_speed - speed_penalty;
        OCR0A = motor_speed;
    }
    else if (heuristic > 0)
    {

        OCR0A = motor_speed - speed_penalty;
        OCR0B = motor_speed;
    }
    else
    {
        OCR0B = motor_speed;
        OCR0A = motor_speed;
    }
}
void PID_controller(int heuristic)
{
    // totalHeuristic -> PID value -> speed_penalty
    // Determine if speed_penalty needs to be scalar i.e motor_max - motor_max * speed_penalty

    float dt = SENSOR_TICK_DT_MS / 1000.0;

    // integral of error adds up over time
    cum_hueristic += heuristic * dt;
    float derivative = (last_heuristic - heuristic) / dt;
    last_heuristic = heuristic;

    float PID = Kp * heuristic + Ki * cum_hueristic + Kd * derivative;

    if (PID < 0)
    {
        OCR0B = motor_speed - (int)abs(PID);
        OCR0A = motor_speed;
    }
    else if (PID > 0)
    {

        OCR0A = motor_speed - (int)abs(PID);
        OCR0B = motor_speed;
    }
    else
    {
        OCR0B = motor_speed;
        OCR0A = motor_speed;
    }

    // String toPrint = String("Done single sensor loop") + globalCounter;
    // Serial.println(toPrint);

    // derivative of error is the rate of change
}

void motor_init()
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
    // if (current_sensor == 0)
    // {
    //     String toPrint = String("Done single ADC loop") + globalCounter;
    //     Serial.println(toPrint);
    // }
    set(ADCSRA, 6);
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
    Serial.print("Finished line at ");
    Serial.println(globalCounter);
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

void setup_sensors()
{
    adc_setup();
}

void set_motor_speed(int input)
{
    motor_speed = input;
}

void set_PID_constants(float p, float i, float d)
{
    Kp = p;
    Ki = i;
    Kd = d;
}

void stop_motors()
{
    TCCR0B = timerOff;
    // Not turning off A register leads to bug where the OUTPUT register is always on even if you change OCR
    TCCR0A = timerOff;
    TCNT0 = 0;
}

void start_motors()
{

    TCCR0B = timer0BOn;
    TCCR0A = timer0AOn;
}
