#ifndef LINE__DETECTION_H
#define LINE__DETECTION_H

typedef struct
{
    int reference;
    float value;
} Sensor;

#define SENSOR_TICK_DT_MS 1

//assigned in main
extern volatile unsigned long globalCounter;

extern char slowMarker;
extern char stopMarker;

void sensors_init();
void motor_init();
void sensor_tick();
void debug_print_sensors();
void setup_sensors();
void reset_PID();

#define MAX_MOTOR_SPEED 255

// extern int speed_penalty;
// https://wokwi.com/projects/330808386186642002

int set_motor_speed(int);
void set_speed_penalty(int input);
void set_PID_constants(float p, float i, float d);
void print_PID();
void print_motor_speed();

void start_motors();
void stop_motors();

#endif