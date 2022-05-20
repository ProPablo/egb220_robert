#ifndef LINE__DETECTION_H
#define LINE__DETECTION_H

typedef struct
{
    int reference;
    int value;
} Sensor;


#define SENSOR_TICK_DT_MS 8

extern volatile unsigned long globalCounter; 

void sensors_init();
void motor_init();
void sensor_tick();
void debug_print_sensors();
void setup_sensors();

#define MAX_MOTOR_SPEED 255

// extern int speed_penalty;
// https://wokwi.com/projects/330808386186642002

void set_motor_speed(int );
void set_speed_penalty(int input);
void set_PID_constants(float p, float i, float d);

void start_motors();
void stop_motors();

#endif