#ifndef ROBERT_LINK_H
#define ROBERT_LINK_H 

#include "line_detection.h"

int serialHelpMessage();
//non-blocking (keeps internal state)
void acceptSerialInput();

char *binString(unsigned short n);
#endif
