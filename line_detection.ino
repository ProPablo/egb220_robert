#include "line_detection.h"

int myValue = 0;

void debug_print_sensors()
{
    Serial.println(myValue);
}

void setup_sensors(int input)
{
    myValue = input;
}