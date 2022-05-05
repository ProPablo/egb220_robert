#ifndef LINE__DETECTION_H
#define LINE__DETECTION_H

typedef struct
{
    int reference;
    int value;
} Sensor;

void sensors_init();
void debug_print_sensors();
void setup_sensors(int input);

#endif