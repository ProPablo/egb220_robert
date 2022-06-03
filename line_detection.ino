#include "line_detection.h"
#include <avr/io.h>

Sensor line_sensors[8] = {
    {8, -2},    // S8
    {9, -1.8},  // S7
    {10, -1.3}, // S6
    {11, -0.5}, // S5
    {7, 0.5},   // S4
    {6, 1.3},   // S3
    {5, 1.8},   // S2
    {4, 2}      // S1
};

#define MOTOR_MAX 130
#define MOTOR_MIN 90

#define THRESHOLD 215
int speed_penalty = 70;
int motor_speed = MOTOR_MAX;

int current_sensor = 0;
#define INTEGRAL_MAX 0.2

extern volatile unsigned long globalCounter;
// PID
float Kp = 0.65;  // P gain for PID control
float Ki = 0.09; // I gain for PID control
float Kd = 0.15;  // D gain for PID control

// initialize e_i, e_d
float cum_heuristic = 0;  // integral
float last_heuristic = 0; // For derivatice

bool d_initialized = false;

int sensor_values[8] = {0, 0, 0, 0, 0, 0, 0, 0};
volatile float heuristic = 0.0;

volatile int adcLeft = 0;
volatile int adcRight = 0;

char timer0AOn = 0x0;
char timer0BOn = 0x0;
char timerOff = 0x0;

void sensor_tick()
{
    // if negative set left motor 2 points lower to move left (keep right at max)
    // int normalized_huerstic = ((sensor_hueristic * 255) / overall_motor_divisor) / SENSOR_HEURISTIC_MAX; // Value from 0 to 255
    // int totalhueristic = 0;

    // String toPrint = String("Done single sensor loop") + globalCounter + String(",") + heuristic;
    // Serial.println(toPrint);

    // debug_print_sensors();
    // Serial.println(heuristic);

    // bang_bang_controller();
    PID_controller();
    colour_sensor_subsystem();
}

void reset_PID()
{
    d_initialized = false;
}

void bang_bang_controller()
{
    if (heuristic > 0)
    {
        OCR0B = motor_speed - speed_penalty;
        OCR0A = motor_speed;
    }
    else if (heuristic < 0)
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

enum COLOUR_SUBSYSTEM
{

};

bool isOnCorner = false;
char whiteDebounceMask = 0b00111111;
char whiteDebounce = 0x00;
bool whitePrev = false;

#define WHITE_SENSOR_THRESHOLD 990

#define RED_THRESHOLD 1000
#define GREEN_THRESHOLD 950

void colour_sensor_subsystem()
{
    // debounce adc input, if consistent for n bits,
    //
    bool isCurrentlyWhite = adcLeft < WHITE_SENSOR_THRESHOLD;
    whiteDebounce = ((whiteDebounce << 1) & whiteDebounceMask) | isCurrentlyWhite;

    bool isConfirmedWhite = (whiteDebounce == whiteDebounceMask);

    bool isJustInWhite = isConfirmedWhite && !whitePrev;
    if (isJustInWhite)
    {
        isOnCorner = !isOnCorner;
        if (isOnCorner)
        {
            set(PORTE, 6);
            // set music OCR here
            motor_speed = MOTOR_MAX;
        }
        else
        {
            clr(PORTE, 6);
            motor_speed = MOTOR_MIN;
        }
    }

    // If on corner state go slower
}
void PID_controller()
{
    // totalHeuristic -> PID value -> speed_penalty
    // Determine if speed_penalty needs to be scalar i.e motor_max - motor_max * speed_penalty

    // Changing the sensor tick should not effect the PID variables (the P might become way too aggressive if tick freq increased)
    float dt = SENSOR_TICK_DT_MS / 1000.0;
    // Serial.println(dt);

    // integral of error adds up over time
    cum_heuristic += heuristic * dt;
    cum_heuristic = constrain(cum_heuristic, -INTEGRAL_MAX, INTEGRAL_MAX);

    float derivative = (last_heuristic - heuristic) / dt;
    if (!d_initialized)
    {
        derivative = 0;
        d_initialized = true;
    }
    last_heuristic = heuristic;

    float PID = Kp * heuristic + Ki * cum_heuristic + Kd * derivative;

    // Serial.println("Pid:" + String(PID) + " cum: " + String(cum_heuristic));
    int result = (int)((PID)*motor_speed);

    if (result > 0)
    {
        // OCR0B = constrain(motor_speed - (int)abs(PID), 0, 255);
        OCR0B = constrain(motor_speed - abs(result), 0, 255);
        OCR0A = motor_speed;
    }
    else if (PID < 0)
    {

        OCR0A = constrain(motor_speed - abs(result), 0, 255);
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

enum ADC_STATE
{
    ADC_SENSE_LINE,
    ADC_LEFT,
    ADC_RIGHT
};

ADC_STATE current_ADC = ADC_SENSE_LINE;

ISR(ADC_vect)
{
    switch (current_ADC)
    {
        // Store all sensor values and run done compute
    case ADC_SENSE_LINE:
        sensor_values[current_sensor] = ADCH;
        current_sensor = (current_sensor + 1) % 8;
        setup_next_sensor();
        if (current_sensor == 0)
        {
            compute_heuristic();
            clr(ADMUX, 5);
            setup_sensor(0);
            current_ADC = ADC_LEFT;
        }
        break;
    case ADC_LEFT:
        adcLeft = read_sensor_full();
        setup_sensor(1);
        current_ADC = ADC_RIGHT;
        break;
    case ADC_RIGHT:
        adcRight = read_sensor_full();
        set(ADMUX, 5);
        setup_next_sensor();
        current_ADC = ADC_SENSE_LINE;
        break;

    default:
        break;
    }
    set(ADCSRA, 6);
}

void setup_sensor(int reference)
{
    char carry = ADMUX & 0b11100000; // Select ref bits only
    int mux_register = 0b00000111 & reference;
    int mux5 = (reference >> 3) & 1;
    ADMUX = carry | mux_register;
    ADCSRB = (mux5 << 5);
}

void setup_next_sensor()
{
    int current_reference = line_sensors[current_sensor].reference;
    setup_sensor(current_reference);
}

int read_sensor_full()
{
    int sensor_low = ADCL;
    int sensor_high = ADCH;
    int result = (sensor_high << 8) | (sensor_low);
    return result;
}

void compute_heuristic()
{

    // // Compute the huerisitc here
    float leftH = 0;
    float rightH = 0;
#if SINGLEHUERISTIC
    for (int i = 3; i >= 0; i--)
    {
        // Serial.print(i);
        // Serial.print(",");
        if (sensor_values[i] < THRESHOLD)
            leftH = line_sensors[i].value;
    }
    // Serial.println("");
    for (int i = 4; i < 8; i++)
    {
        if (sensor_values[i] < THRESHOLD)
            rightH = line_sensors[i].value;
    }

#endif

    int selectedSensors = 0;
    for (int i = 0; i < 4; i++)
    {
        if (sensor_values[i] < THRESHOLD)
        {
            leftH += line_sensors[i].value;
            selectedSensors++;
        }
    }
    if (leftH != 0)
        leftH = leftH / (float)selectedSensors;

    selectedSensors = 0;
    for (int i = 4; i < 8; i++)
    {
        if (sensor_values[i] < THRESHOLD)
        {
            rightH += line_sensors[i].value;
            selectedSensors++;
        }
    }

    if (rightH != 0)
        rightH = rightH / (float)selectedSensors;

    // Serial.println(String(leftH) + String(",") + String(rightH));

    if (abs(leftH) > abs(rightH))
        heuristic = leftH;
    else if (abs(rightH) > abs(leftH))
        heuristic = rightH;
    else if (rightH == 0 && leftH == 0)
    {
        Serial.println("WOAH THERE COWBOAY");
        stop_motors();
        heuristic = 0;
    }
    else
    {
        heuristic = 0;
    }
    // heuristic = 0;
    // for (int i = 0; i < 4; i++)
    // {
    //     heuristic += sensor_values[i];
    // }
    // for (int i = 4; i < 8; i++)
    // {
    //     heuristic -= sensor_values[i];
    // }

    // String toPrint = String("Done single ADC loop") + globalCounter;
    // Serial.println(toPrint);
}

void debug_print_sensors()
{
    // _delay_ms(200);
    String sensorString = "Here are sensors:";
    for (int i = 0; i < 8; i++)
    {
        sensorString += String(sensor_values[i]) += " , ";
        // Serial.print(sensor_values[i]);
        // Serial.print(" , ");
    }
    sensorString += String("Left: ") + adcLeft;
    sensorString += String(", Right:") + adcRight;
    Serial.println(sensorString);
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

// void print_motor_speed() {
//     Serial.print()

// }

int set_motor_speed(int input)
{
    int prev = motor_speed;
    motor_speed = input;
    return prev;
}

void set_PID_constants(float p, float i, float d)
{

    Kp = p;
    Ki = i;
    Kd = d;
}

void print_PID()
{
    String toPrint = String("P: ") + String(Kp) + String(", I: ") + String(Ki) + String(", D: ") + String(Kd);
    Serial.println(toPrint);
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

void print_motor_speed()
{
    String toPrint = String("Motor speed: ") + motor_speed;
    Serial.println(toPrint);
}
