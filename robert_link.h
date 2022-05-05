#ifndef ROBERT_LINK_H
#define ROBERT_LINK_H 


int serialHelpMessage();
//non-blocking (keeps internal state)
void acceptSerialInput();

char *binString(unsigned short n);
#endif
