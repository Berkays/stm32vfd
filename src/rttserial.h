#ifndef __RTTSERIAL_H
#define __RTTSERIAL_H

#define SERIAL_OUTPUT

#if defined(SERIAL_OUTPUT)
#include "RTT/SEGGER_RTT.h"
void sendString(const char *);
void sendInt(const char *, int);
#else
#define sendString(a) ;
#define sendInt(a, b) ;
#endif
#endif