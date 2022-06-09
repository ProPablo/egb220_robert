#ifndef ROBERT_LINK_H
#define ROBERT_LINK_H 

#include "line_detection.h"

int SerialIntroMessage();
//non-blocking (keeps internal state)
int acceptSerialInput();

void get_eeprom_vars();

char *binString(unsigned short n);
#endif
